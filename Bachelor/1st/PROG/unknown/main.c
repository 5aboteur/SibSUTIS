#include <stdio.h>
#include "input.h"
#include "processing.h"
#include "check.h"
#include "output.h"

#define MAXPATH 261
#define MAXCNT 10
#define MAXSIZE (MAXPATH*MAXCNT)

#define T 1
#define F 0



int main()
{
    int z=0, i, totallines=0;
    char paths[MAXSIZE];
    char delim;
    char* bsymbols="*?<>\"\\|:";
    char allpaths[MAXCNT][MAXPATH];
	char rawpaths[MAXCNT*2][MAXPATH];
	char actualpath[MAXCNT][MAXPATH];

	FILE *fin;
	FILE *fout;
	fin=fopen("test.ini", "r");
	for(i=0;i<MAXCNT*2;i++)
	{
		if(feof(fin))
		{
			break;
		}
		else
		{
			fgets(rawpaths[i], MAXPATH, fin);
			totallines++;
		}
	}

	fclose(fin);
	fin=fopen("test.ini", "w");
	for(i=0;i<totallines;i++)
	{
		if(contains(rawpaths[i], "/cygdrive/"))
		{
			process2(rawpaths[i]+6);
			fputs(rawpaths[i], fin);
		}
		else
		{
			fputs(rawpaths[i], fin);
		}
	}
	fclose(fin);


    
    /*z=input(&delim, paths);
    if(z == -1){
         printf("Delimiter is not found!\n");
         system("PAUSE");
         return 0;
    }*/


    /*token(delim, paths, allpaths);*/
    check(allpaths, bsymbols);
    process(allpaths);
    output(allpaths, delim);
    system("PAUSE");
}