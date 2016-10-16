#include<stdio.h>
#include<string.h>
#define MAXPATH 261
#define MAXCNT 10
#define MAXSIZE (MAXPATH*MAXCNT)

int main()
{
	char cygwin;
	scanf("%s", cygwin);
	fgets(cygwin, MAXSIZE, stdin);
