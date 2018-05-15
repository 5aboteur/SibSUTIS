#include "dump.h"

int dump(void)
{
	int i;
	char fname[NAMESIZ];

	// Prefix
	strncpy(fname, "umqtracker_", 11);

	time_t t = time(NULL);
	struct tm lt = *localtime(&t);
	char tmp[64];

	strftime(tmp, sizeof(tmp), "%c", lt);

	for (i = 0; i < 63 && tmp[i] != '\0'; i++) {
		if (tmp[i] == ' ')
			tmp[i] = '-';
	}

	tmp[i] = '\0';

	// + localtime = dirname
	strcpy(fname + 11, tmp);

	char cmd[CMDSIZ];

	// Create dir with this name
	strncpy(cmd, "mkdir ", 6);
	strcpy(cmd + 6, fname);
	system(cmd);

	printf(" = %s\n", cmd);
	printf("Directory '%s' successfully created.\n", fname);

	// Get the img name
	char iname[NAMESIZ];
	DIR *dir_fp;
	struct dirent *de;

	dir_fp = opendir(".");
	if (dir_fp == NULL) {
		fprintf(stderr, "Cannot open this directory.\n");
		return -1;
	}

	while ((de = readdir(dir_fp)) != NULL) {
		if (strncmp(".png", de->d_name, 4) == 0) {
			strcpy(iname, de->d_name);
			break;
		}
	}

	closedir(dir_fp);

	printf("Image name: %s\n", iname);

	long len = strlen(fname);

	// + .html extension = filename
	strncpy(fname + len, ".html", 5);

	FILE *html_fp;

	html_fp = fopen(fname, "w");
	if (html_fp == NULL) {
		fprintf(stderr, "Cannot open the file '%s'.\n", fname);
		return -1;
	}

	// Fill HTML page with info
	fprintf(html_fp,
		"<!DOCTYPE html>\n<html>\n<body>\n\n"
		"<img src=\"%s\" alt=\"UMQ size\">"
		"</body></html>", iname);

	fclose(html_fp);

	printf("File '%s' successfully created.\n", fname);

	memset(cmd, 0, CMDSIZ];

	// Move .png and .html files into created dir
	strncpy(cmd, "mv ", 3);
	strcpy(cmd + 3, fname);
	strncpy(cmd + 3 + len, " ", 1);
	strcpy(cmd + 4 + len, iname);
	system(cmd);

	printf(" = %s\n", cmd);

	return 0;
}
