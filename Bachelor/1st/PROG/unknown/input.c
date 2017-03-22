#include <stdio.h>
#include "input.h"
#define MAXPATH 261
#define MAXCNT 10
#define MAXSIZE (MAXPATH*MAXCNT)


int input(char* delim, char s[])
{
    printf("Input delimiter: ");
    scanf("%c%*c",delim);
    if(*delim == '\n')
    return -1;
        
        printf("Input paths: ");
        fgets(s,MAXPATH*MAXCNT,stdin);
        if(*delim == ' ')
        {
            printf("\nDelimiter is <SPACE>\n");
        }
        else
        {
            printf("\nDelimiter is %c\n",*delim);
        }
     return 0;
}
