#include "dump.h"

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

	printf(" = %s\n", cmd);
	printf("Directory '%s' successfully created.\n", dname);

	char iname[NAMESIZ];
	DIR *dir_fp;
	struct dirent *de;

	dir_fp = opendir(".");
	if (dir_fp == NULL) {
		fprintf(stderr, "Cannot open this directory.\n");
		return;
	}

	// Get the img name
	while ((de = readdir(dir_fp)) != NULL) {
		int ext;

		for (ext = 0; de->d_name[ext] != '\0'; ext++)
			if (de->d_name[ext] == '.') break;

		if (strncmp(".png", de->d_name + ext, 4) == 0) {
			printf("> %s %zu\n", de->d_name, strlen(de->d_name));
			sprintf(iname, "%s", de->d_name);
			iname[strlen(de->d_name)] = '\0';
//			break;
		}
	}

	closedir(dir_fp);

	printf("Image name: %s\n", iname);

	char fname[NAMESIZ];

	// prefix + localtime + .html extension = filename
	sprintf(fname, "%s.html", dname);

	FILE *html_fp;

	html_fp = fopen(fname, "w");
	if (html_fp == NULL) {
		fprintf(stderr, "Cannot open the file '%s'.\n", fname);
		return;
	}

	// Fill HTML page with info
	fprintf(html_fp,
		"<!DOCTYPE html>\n<html>\n<body>\n\n<h1>MPI RECVQ Tracker</h1>"
		"<strong>Nto1 task</strong> - N processes simultaneously "
		"send NITERS messages to one receiving process.</p>"
		"<img src=\"%s\" alt=\"queue sizes compare\">"
		"<br><hr><br><div align=right>%s<br>made by 5aboteur</div>"
		"</body></html>",
		iname, current_date);

	fclose(html_fp);

	printf("File '%s' successfully created.\n", fname);

	memset(cmd, 0, CMDSIZ);

	// Move .png and .html files into created dir
	sprintf(cmd, "mv %s %s %s", fname, iname, dname);

	if (system(cmd) < 0) return;

	printf(" = %s\n", cmd);
}
