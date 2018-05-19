#include "dump.h"

extern int iters;

extern unsigned long long umq_bs_tot;
extern unsigned long long umq_bs_min;
extern unsigned long long umq_bs_max;

void dump(void)
{
	int i;
	char dname[NAMESIZ];

	time_t t = time(NULL);
	struct tm lt = *localtime(&t);
	char current_date[64];
	char tmp[64];

	strftime(current_date, sizeof(current_date), "%c", &lt);
	strftime(tmp, sizeof(tmp), "%c", &lt);

	for (i = 0; i < 63 && tmp[i] != '\0'; i++) {
		if (tmp[i] == ' ')
			tmp[i] = '-';
	}

	tmp[i] = '\0';

	// prefix + localtime = dirname
	sprintf(dname, "umqtracker_%s", tmp);

	char cmd[CMDSIZ];

	// Create dir with this name
	sprintf(cmd, "mkdir %s", dname);
	if (system(cmd) < 0) return;

	printf(" => %s\n", cmd);
	printf("Directory '%s' successfully created.\n", dname);

	char iname[3][NAMESIZ];
	DIR *dir_fp;
	struct dirent *de;

	dir_fp = opendir(".");
	if (dir_fp == NULL) {
		fprintf(stderr, "Cannot open this directory.\n");
		return;
	}

	i = 0;

	// Get the img name
	while ((de = readdir(dir_fp)) != NULL) {
		int ext;

		for (ext = 0; de->d_name[ext] != '\0'; ext++)
			if (de->d_name[ext] == '.') break;

		if (strncmp(".png", de->d_name + ext, 4) == 0) {
			printf(" %s %zu\n", de->d_name, strlen(de->d_name));
			sprintf(iname[i], "%s", de->d_name);
			iname[i][strlen(de->d_name)] = '\0';
			i++;
		}
	}

	closedir(dir_fp);

	printf("Image names: %s, %s, %s\n", iname[0], iname[1], iname[2]);

	char fname[NAMESIZ];

	// prefix + localtime + .html extension = filename
	sprintf(fname, "%s.html", dname);

	FILE *html_fp;

	html_fp = fopen(fname, "w");
	if (html_fp == NULL) {
		fprintf(stderr, "Cannot open the file '%s'.\n", fname);
		return;
	}

	char task_name[NAMESIZ];
	char desc_name[NAMESIZ];

	memset(&task_name, 0, sizeof(task_name));
	memset(&desc_name, 0, sizeof(desc_name));

	char ch;

	i = 0;

	printf("Input task name: ");
	while (((ch = fgetc(stdin)) != '\n') && (i < NAMESIZ))
		task_name[i++] = ch;

	i = 0;

	printf("Input description: ");
	while (((ch = fgetc(stdin)) != '\n') && (i < NAMESIZ))
		desc_name[i++] = ch;

	// Fill HTML page with info
	fprintf(html_fp,
		"<!DOCTYPE html>\n"
		"<html>\n"
		"<body>\n"
		"<header>\n\n"
		"<h1 align=\"center\">MPI RECVQ Tracker<sup style=\"font-size:15px\"> beta </sup></h1>"
		"<hr><br>\n</header>\n"
		"<section>\n"
		"<strong>TASK:</strong> %s <br><strong>DESC:</strong> %s <br></p>\n"
		"<img width=630 height=475 src=\"%s\" alt=\"image1\">"
		"<img width=630 height=475 src=\"%s\" alt=\"image2\">"
		"<img width=630 height=475 src=\"%s\" alt=\"image3\">\n"
		"</section>\n"
		"<section><br>\n"
		"<strong>Unexpected queue allocated buffer size ~> </strong>"
		"min = %llu, max = %llu, avg = %llu (%.3lf Mb)\n"
		"</section>\n"
		"<footer>\n"
		"<br><hr><br><div align=right>%s<br>made by 5aboteur</div>"
		"</footer>\n"
		"</body>\n"
		"</html>",
		task_name, desc_name, iname[0], iname[1], iname[2],
		umq_bs_min, umq_bs_max, (umq_bs_tot / iters),
		(umq_bs_tot / iters / 1024.0 / 1024.0), current_date);

	fclose(html_fp);

	printf("File '%s' successfully created.\n", fname);

	memset(cmd, 0, CMDSIZ);

	// Move .png and .html files into created dir
	sprintf(cmd, "mv %s %s %s %s %s", fname, iname[0], iname[1], iname[2], dname);

	if (system(cmd) < 0) return;

	printf(" => %s\n", cmd);
}
