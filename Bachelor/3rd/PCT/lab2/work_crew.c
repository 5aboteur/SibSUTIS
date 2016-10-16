#include <pthread.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <time.h>
#include <signal.h>
#include "errors.h"

#define CREW_SIZE 16			// Max crew size

/*
 * Queued items of work for the crew. One is queued by
 * crew_start, and each worker may queue additional items.
 */
typedef struct work_tag {
	struct work_tag	*next;		// Next work item
	char		*path;		// Directory or file
} work_t, *work_p;

/*
 * One of these is initialized for each worker thread in the
 * crew. It contains the "identity" of each worker.
 */
typedef struct worker_tag {
	int		index;		// Thread's index
	pthread_t	thread;		// Thread for stage
	struct crew_tag *crew;		// Pointer to crew
} worker_t, *worker_p;

/*
 * The external "handle" for a work crew. Contains the
 * crew synchronization state and staging area.
 */
typedef struct crew_tag {
	int		crew_size;	// Size of array
	worker_t	crew[CREW_SIZE];// Crew members
	long		work_count;	// Count of work items
	work_t		*first, *last;	// First & last work item

	pthread_mutex_t	mutex;		// Mutex for crew data
	pthread_cond_t	done;		// Wait for crew done
	pthread_cond_t	go;		// Wait for work
} crew_t, *crew_p;

size_t path_max;			// Filepath length
size_t name_max;			// Name length

int is_Shell;				// Check for the shell command
int is_Finished;			// Flag for deferred quit

pthread_t sig;				// Signal thread
sigset_t mask;				// Mask for a signal

pthread_mutex_t lock =
	PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t wait =
	PTHREAD_COND_INITIALIZER;

///////////////////////////////////////////////////////////////////////
struct timeval tv1, tv2, dtv;
struct timezone tz;
void time_start() { gettimeofday (&tv1, &tz); }
long time_stop() {
	gettimeofday (&tv2, &tz);
	dtv.tv_sec = tv2.tv_sec - tv1.tv_sec;
	dtv.tv_usec = tv2.tv_usec - tv1.tv_usec;
	if (dtv.tv_usec < 0) { dtv.tv_sec--; dtv.tv_usec += 1000000; }
	return dtv.tv_sec * 1000 + dtv.tv_usec / 1000;
}
///////////////////////////////////////////////////////////////////////

/*
 * Handle the input signal.
 */
void *signal_thread (void *arg) {
	int err, signo, i;
	crew_t *crew = (crew_t *) arg;
	while (1) {
		err = sigwait (&mask, &signo);
		if (err != 0) {
			fprintf (stderr, "Sigwait failed");
			exit (1);
		}
		switch (signo) {
		case SIGQUIT:
			printf ("[[ SIGQUIT received ]]\n");
			is_Finished = 1;
			break;
		case SIGINT:
			printf ("[[ SIGINT received ]]\n");
			//
			for (i = 0; i < CREW_SIZE; i++) {
				err = pthread_cancel (crew->crew[i].thread);
				if (err != 0) errno_abort ("Canceling");
			}
			exit (0);
			break;
		default:
			fprintf (stderr, "Unknown signal %d\n", signo);
			exit (1);
		}
	}
}

/*
 * Unlock the mutex when thread is canceled.
 */
void cleanup_lock (void *arg) {
	printf ("Cleanup lock\n");
	pthread_mutex_t *lock = (pthread_mutex_t *) arg;
	pthread_mutex_unlock (lock);
}

/*
 * Close the file when thread is canceled.
 */
void cleanup_file (void *arg) {
	printf ("Cleanup log\n");
	FILE *file = (FILE *) arg;
	fclose (file);
}

/*
 * Free data.
 */
void cleanup_entry (void *arg) {
	printf ("Cleanup entry\n");
	struct dirent *ent = (struct dirent *) arg;
	free (ent);
}

/*
 * Close current directory.
 */
void cleanup_dir (void *arg) {
	printf ("Cleanup dir\n");
	DIR *dir = (DIR *) arg;
	closedir (dir);
}

/*
 * The thread start routine for crew threads. Waits until "go"
 * command, processes work items until requested to shut down.
 */
void *worker_routine (void *arg) {
	pthread_setcanceltype (PTHREAD_CANCEL_DEFERRED, NULL);
	pthread_setcancelstate (PTHREAD_CANCEL_ENABLE, NULL);

	worker_p mine = (worker_t *) arg;
	crew_p crew = mine->crew;
	work_p work, new_work;

	struct stat filestat;
	struct dirent *entry;

	int status;

	FILE *log_fp;

	time_t t = time(NULL);
	struct tm* aTm = localtime(&t);

	/*
	 * Huge buffer.
	 */
	entry = (struct dirent*) malloc (sizeof(struct dirent) + name_max);
	if (entry == NULL) errno_abort ("Allocating dirent");
	pthread_cleanup_push (cleanup_entry, NULL);

	status = pthread_mutex_lock (&crew->mutex);
	if (status != 0) err_abort (status, "Lock crew mutex");
	pthread_cleanup_push (cleanup_lock, NULL);

	/*
	 * There won't be any work when the crew is created, so wait
	 * until something's put on the queue.
	 */
	while (crew->work_count == 0) {
		status = pthread_cond_wait (&crew->go, &crew->mutex);
		if (status != 0) err_abort (status, "Wait for go");
	}
	pthread_cleanup_pop (0);
	status = pthread_mutex_unlock (&crew->mutex);
	if (status != 0) err_abort (status, "Unlock mutex");

	DPRINTF (("Crew %d starting\n", mine->index));

	/*
	 * Now, as long as there's work, keep doing it.
	 */
	while (1) {
		/*
		 * Wait while there is nothing to do, and
		 * the hope of something coming along later. If
		 * crew->first is NULL, there's no work. But if
		 * crew->work_count goes to zero, we're done.
		 */
		status = pthread_mutex_lock (&crew->mutex);
		if (status != 0) err_abort (status, "Lock crew mutex");
		pthread_cleanup_push (cleanup_lock, NULL);

		DPRINTF (("Crew %d top: first is %#lx, count is %d\n",
			mine->index, crew->first, crew->work_count));

		while (crew->first == NULL) {
			status = pthread_cond_wait (&crew->go, &crew->mutex);
			if (status != 0) err_abort (status, "Wait for work");
		}

		DPRINTF (("Crew %d woke: %#lx, %d\n",
			mine->index, crew->first, crew->work_count));

		/*
		 * Remove and process a work item.
		 */
		work = crew->first;
		crew->first = work->next;
		if (crew->first == NULL)
			crew->last = NULL;

		DPRINTF (("Crew %d took %#lx, leaves first %#lx, last %#lx\n",
			mine->index, work, crew->first, crew->last));

		pthread_cleanup_pop (0);
		status = pthread_mutex_unlock (&crew->mutex);
		if (status != 0) err_abort (status, "Unlock mutex");

		/*
		 * We have a work item. Process it, which may involve
		 * queuing new work items.
		 */
		status = lstat (work->path, &filestat);

		if (S_ISREG (filestat.st_mode) && is_Shell == 0) {
			int pos, i, j, k, len;
			char name[256] = "", cut_name[256] = "";
			char format[256] = "", cur_path[256] = "";
			char compil[256] = "", launch[256] = "";

			/*
			 * It's file. So try to compile it as C-file and then try to run it.
			 * Then put the result of the executing in the file.
			 */
			len = strlen (work->path);

			for (i = 0; i < len; i++) {
				if (work->path[i] == '/')
					pos = i;
			}
			for (i = 0; i < pos; i++)
				cur_path[i] = work->path[i];
			cur_path[i] = '\0';

			pos++;

			for (i = pos, j = 0; i < len; i++, j++)
				name[j] = work->path[i];
			name[j] = '\0';

			for (i = 0, j = 0; name[i] != '\0'; i++) {
				if (name[i] == '.') {
					for (j = i + 1, k = 0; name[j] != '\0'; j++, k++)
						format[k] = name[j];
					break;
				}
				cut_name[i] = name[i];
			}
			format[k] = '\0';
			cut_name[i] = '\0';

			strcat (compil, "gcc ");
			strcat (compil, work->path);
			strcat (compil, " -o ");
			strcat (compil, cur_path);
			strcat (compil, "/");
			strcat (compil, cut_name);
			status = system (compil);

			/*
			 * Fill the log file.
			 */
			if (status == 0) {
				if ((log_fp = fopen ("log.txt", "a")) == NULL) {
					errno_abort ("Opening file");
				}
				pthread_cleanup_push (cleanup_file, NULL);
				fprintf (log_fp, "[%04d/%02d/%02d %02d:%02d:%02d] ", aTm->tm_year + 1900,
					aTm->tm_mon + 1, aTm->tm_mday, aTm->tm_hour, aTm->tm_min, aTm->tm_sec);
				fprintf (log_fp, "Result of compiling (%s): SUCCESS\n", work->path);
				pthread_cleanup_pop (0);
				fclose (log_fp);

				strcat (launch, cur_path);
				strcat (launch, "/");
				strcat (launch, cut_name);

				char new_file[256] = "";
				char __buff[256] = "";

				FILE *fptr;
				FILE *pptr;

				strcat (new_file, launch);
				strcat (new_file, "_result.txt");
				if ((fptr = fopen (new_file, "w")) != NULL) {
					if ((pptr = popen (launch, "r")) != NULL) {
						status = 0;
						while ((fgets (__buff, 256, pptr)) != NULL) {
							(void)fprintf (fptr, "%s", __buff);
						}
					} else {
						status = -1;
						errno_abort ("Opening pipe");
					}
				} else {
					status = -1;
					errno_abort ("Opening file");
				}
				fclose (fptr);
				pclose (pptr);

				/*
				 * Fill the log file.
				 */
				if (status == 0) {
					if ((log_fp = fopen ("log.txt", "a")) == NULL) {
						errno_abort ("Opening file");
					}
					pthread_cleanup_push (cleanup_file, NULL);
					fprintf (log_fp, "[%04d/%02d/%02d %02d:%02d:%02d] ",
							aTm->tm_year + 1900, aTm->tm_mon + 1, aTm->tm_mday,
							aTm->tm_hour, aTm->tm_min, aTm->tm_sec);
					fprintf (log_fp, "Result of launching (%s): SUCCESS\n", launch);
					pthread_cleanup_pop (0);
					fclose (log_fp);

					char rmv[256] = "";

					/*
					 * Remove created compile object.
					 */
					strcat (rmv, "rm ");
					strcat (rmv, launch);
					status = system (rmv);

					// Keep filling.
					if (status == 0) {
						if ((log_fp = fopen ("log.txt", "a")) == NULL) {
							errno_abort ("Opening file");
						}
						pthread_cleanup_push (cleanup_file, NULL);
						fprintf (log_fp, "[%04d/%02d/%02d %02d:%02d:%02d] ",
							aTm->tm_year + 1900, aTm->tm_mon + 1, aTm->tm_mday,
							aTm->tm_hour, aTm->tm_min, aTm->tm_sec);
						fprintf (log_fp, "Result of removing  (%s): SUCCESS\n", launch);
						pthread_cleanup_pop (0);
						fclose (log_fp);
					} else {
						if ((log_fp = fopen ("log.txt", "a")) == NULL) {
							errno_abort ("Opening file");
						}
						pthread_cleanup_push (cleanup_file, NULL);
						fprintf (log_fp, "[%04d/%02d/%02d %02d:%02d:%02d] ",
							aTm->tm_year + 1900, aTm->tm_mon + 1, aTm->tm_mday,
							aTm->tm_hour, aTm->tm_min, aTm->tm_sec);
						fprintf (log_fp, "Result of removing  (%s): FAILURE\n", launch);
						pthread_cleanup_pop (0);
						fclose (log_fp);
					}
				} else {
					if ((log_fp = fopen ("log.txt", "a")) == NULL) {
						errno_abort ("Opening file");
					}
					pthread_cleanup_push (cleanup_file, NULL);
					fprintf (log_fp, "[%04d/%02d/%02d %02d:%02d:%02d] ",
						aTm->tm_year + 1900, aTm->tm_mon + 1, aTm->tm_mday,
						aTm->tm_hour, aTm->tm_min, aTm->tm_sec);
					fprintf (log_fp, "Result of launching (%s): FAILURE\n", launch);
					pthread_cleanup_pop (0);
					fclose (log_fp);
				}
			} else {
				if ((log_fp = fopen ("log.txt", "a")) == NULL) {
					errno_abort ("Opening file");
				}
				pthread_cleanup_push (cleanup_file, NULL);
				fprintf (log_fp, "[%04d/%02d/%02d %02d:%02d:%02d] ", aTm->tm_year + 1900,
					aTm->tm_mon + 1, aTm->tm_mday, aTm->tm_hour, aTm->tm_min, aTm->tm_sec);
				fprintf (log_fp, "Result of compiling (%s): FAILURE\n", work->path);
				pthread_cleanup_pop (0);
				fclose (log_fp);
			}
		} else if (S_ISDIR (filestat.st_mode)) {
			DIR *directory;
			struct dirent *result;

			/*
			 * If the file is a directory, search it and place
			 * all files onto the queue as new work items.
			 */
			directory = opendir (work->path);
			if (directory == NULL) {
				fprintf(stderr, "Unable to open directory %s: %d (%s)\n",
						work->path, errno, strerror (errno));
				continue;
			}
			pthread_cleanup_push (cleanup_dir, NULL);

			while (1) {
				status = readdir_r (directory, entry, &result);
				if (status != 0) {
					fprintf(stderr, "Unable to read directory %s: %d (%s)\n",
						work->path, status, strerror (status));
					break;
				}
				if (result == NULL) break;	// End of directory
				/*
				 * Ignore "." and ".." entries.
				 */
				if (strcmp (entry->d_name, ".") == 0) continue;
				if (strcmp (entry->d_name, "..") == 0) continue;

				new_work = (work_p) malloc (sizeof(work_t));
				if (new_work == NULL) errno_abort ("Unable to allocate space");

				new_work->path = (char*) malloc (path_max);
				if (new_work->path == NULL) errno_abort ("Unable to allocate path");

				strcpy (new_work->path, work->path);
				strcat (new_work->path, "/");
				strcat (new_work->path, entry->d_name);
				new_work->next = NULL;

				status = pthread_mutex_lock (&crew->mutex);
				if (status != 0) err_abort (status, "Lock mutex");
				pthread_cleanup_push (cleanup_lock, NULL);

				if (crew->first == NULL) {
					crew->first = new_work;
					crew->last = new_work;
				} else {
					crew->last->next = new_work;
					crew->last = new_work;
				}
				crew->work_count++;

				DPRINTF (("Crew %d: add %#lx, first %#lx, last %#lx, %d\n",
					mine->index, new_work, crew->first, crew->last, crew->work_count));

				status = pthread_cond_signal (&crew->go);
				pthread_cleanup_pop (0);
				status = pthread_mutex_unlock (&crew->mutex);
				if (status != 0) err_abort (status, "Unlock mutex");
				pthread_testcancel();
			}
			pthread_cleanup_pop (0);
			closedir (directory);
		} else {
			FILE *bash_pp;
			FILE *bash_fp;
			char res_buf[256] = "";

			/*
			 * It's bash script, so try to run it and put the result into the file.
			 */
			if ((bash_fp = fopen ("thascryptz.txt", "a")) == NULL) {
				errno_abort ("Opening file");
			}
			pthread_cleanup_push (cleanup_file, NULL);
			if ((bash_pp = popen (work->path, "r")) != NULL) {
				fgets (res_buf, 256, bash_pp);
				fprintf (bash_fp, "%s >>> %s", work->path, res_buf);
			} else errno_abort ("Opening pipe");

			pclose (bash_pp);
			pthread_cleanup_pop (0);
			fclose (bash_fp);
		}

		free (work->path);		// Free path buffer
		free (work);			// We're done with this

		/*
		 * Decrement count of outstanding work items, and wake
		 * waiters (trying to collect results or start a new
		 * calculation) if the crew is now idle.
		 *
		 * It's important that the count be decremented AFTER
		 * processing the current work item. That ensures the
		 * count won't go to 0 until we're really done.
		 */
		status = pthread_mutex_lock (&crew->mutex);
		if (status != 0) err_abort (status, "Lock crew mutex");
		pthread_cleanup_push (cleanup_lock, NULL);

		crew->work_count--;
		DPRINTF (("Crew thread %d decremented work to %d\n", mine->index, crew->work_count));

		if (crew->work_count <= 0) {
			DPRINTF (("Crew thread %d done\n", mine->index));

			status = pthread_cond_broadcast (&crew->done);
			if (status != 0) err_abort (status, "Wake waiters");

			status = pthread_mutex_unlock (&crew->mutex);
			if (status != 0) err_abort (status, "Unlock mutex");

			break;
		}
		pthread_cleanup_pop (0);
		status = pthread_mutex_unlock (&crew->mutex);
		if (status != 0) err_abort (status, "Unlock mutex");
		pthread_testcancel();
	}

	pthread_cleanup_pop (0);
	free (entry);
	return NULL;
}

/*
 * Create a work crew.
 */
int crew_create (crew_t *crew, int crew_size) {
	int crew_index;
	int status;

	/*
	 * We won't create more than CREW_SIZE members.
	 */
	if (crew_size > CREW_SIZE) return EINVAL;

	crew->crew_size = crew_size;
	crew->work_count = 0;
	crew->first = crew->last = NULL;

	/*
	 * Initialize synchronization objects.
	 */
	status = pthread_mutex_init (&crew->mutex, NULL);
	if (status != 0) return status;

	status = pthread_cond_init (&crew->done, NULL);
	if (status != 0) return status;

	status = pthread_cond_init (&crew->go, NULL);
	if (status != 0) return status;

	/*
	 * Create the worker threads.
	 */
	for (crew_index = 0; crew_index < CREW_SIZE; crew_index++) {
		crew->crew[crew_index].index = crew_index;
		crew->crew[crew_index].crew = crew;
		status = pthread_create (&crew->crew[crew_index].thread,
			NULL, worker_routine, (void*) &crew->crew[crew_index]);
		if (status != 0) err_abort (status, "Create worker");
	}
	status = pthread_create (&sig, NULL, signal_thread, crew);
	if (status != 0) {
		fprintf (stderr, "Cannot create thread");
		exit (1);
	}

	return 0;
}

/*
 * Pass a file path to a work crew previously created
 * using crew_create.
 */
int crew_start (crew_p crew, char *filepath) {
	int status;
	work_p request;

	status = pthread_mutex_lock (&crew->mutex);
	if (status != 0) return status;
	pthread_cleanup_push (cleanup_lock, NULL);

	/*
	 * If the crew is busy, wait for them to finish.
	 */
	while (crew->work_count > 0) {
		status = pthread_cond_wait (&crew->done, &crew->mutex);
		if (status != 0) {
			pthread_mutex_unlock (&crew->mutex);
			return status;
		}
	}

	if (is_Finished == 1) {
		printf ("Current crew have finished its work\n");
		return 0;
	}

	errno = 0;
	path_max = pathconf (filepath, _PC_PATH_MAX);
	if (path_max == -1) {
		if (errno == 0) path_max = 1024;	// No limit
		else errno_abort ("Unable to get PATH_MAX");
	}
//	if (is_Shell == 0) {
//		errno = 0;
//		name_max = pathconf (filepath, _PC_NAME_MAX);
//		if (name_max == -1) {
//			if (errno == 0) name_max = 256;	// No limit
//			else errno_abort ("Unable to get NAME_MAX");
//		}
//	} else name_max = 256;				// No limit
name_max = 256;
	DPRINTF (("PATH_MAX for %s is %ld, NAME_MAX is %ld\n", filepath, path_max, name_max));

	path_max++;					// Add null byte
	name_max++;					// Add null byte

	request = (work_p) malloc (sizeof(work_t));
	if (request == NULL) errno_abort ("Unable to allocate request");

	DPRINTF (("Requesting %s\n", filepath));

	request->path = (char*) malloc (path_max);
	if (request->path == NULL) errno_abort ("Unable to allocate path");

	strcpy (request->path, filepath);
	request->next = NULL;

	if (crew->first == NULL) {
		crew->first = request;
		crew->last = request;
	} else {
		crew->last->next = request;
		crew->last = request;
	}

	crew->work_count++;
	status = pthread_cond_signal (&crew->go);
	if (status != 0) {
		free (crew->first);
		crew->first = NULL;
		crew->work_count = 0;
		pthread_mutex_unlock (&crew->mutex);
		return status;
	}

	while (crew->work_count > 0) {
		status = pthread_cond_wait (&crew->done, &crew->mutex);
		if (status != 0)
			err_abort (status, "Waiting for crew to finish");
	}
	pthread_cleanup_pop (0);
	status = pthread_mutex_unlock (&crew->mutex);
	if (status != 0) err_abort (status, "Unlock crew mutex");

	return 0;
}

/*
 * The main program to "drive" the crew...
 */
int main (int argc, char *argv[]) {
	char _buf[1024];
	int i, status, err;

	crew_t my_crew;

	FILE *fpaths;

	sigset_t oldmask;

	/*
	 * Set mask for every signal.
	 */
	sigemptyset (&mask);
	sigaddset (&mask, SIGINT);
	sigaddset (&mask, SIGQUIT);

	is_Finished = 0;

	if (argc < 2) {
		fprintf (stderr, "Usage: %s string path\n", argv[0]);
		return -1;
	}

	if ((err = pthread_sigmask (SIG_BLOCK, &mask, &oldmask)) != 0) {
		fprintf (stderr, "SIG_BLOCK failed");
		exit (1);
	}

	status = crew_create (&my_crew, CREW_SIZE);
	if (status != 0) err_abort (status, "Create crew");

	if ((fpaths = fopen (argv[1], "r")) == NULL) {
		errno_abort ("Opening file");
	}
	pthread_cleanup_push (cleanup_file, NULL);

	time_start();
	while (fgets (_buf, 1024, fpaths)) {
		if (_buf[0] == '\0' || _buf[0] == '\n') continue;
		else if (_buf[0] == '/') {
			is_Shell = 0;			// Path
			_buf[strlen (_buf)] = '\0';
			status = crew_start (&my_crew, _buf);
			if (status != 0) err_abort (status, "Start crew");
		} else {
			is_Shell = 1;			// Bash script
			_buf[strlen (_buf)] = '\0';
			status = crew_start (&my_crew, _buf);
			if (status != 0) err_abort (status, "Start crew");
		}
	}
	printf ("\033[33mElapsed time\033[0m %ld (ms)..\n", time_stop());

	pthread_cleanup_pop (0);
	fclose (fpaths);
	return 0;
}
