#include <stdio.h>
#include "output.h"


void output(char m[MAXCNT][MAXPATH], char delim)
{
    int i=0, j;        
    for(i=0;i<MAXCNT;i++)
    {
        if ((i!=0)&&(m[i][1]!='\0'))
        {
            printf("%c", delim);
        }
        if((m[i][0] == '\0') && (m[i][1] == '1'))
        {
            printf("ERROR1");
        }
        if((m[i][0] == '\0') && (m[i][1] == '2'))
        {
            printf("ERROR2");
        }  
        if((m[i][0] == '\0') && (m[i][1] == '3'))
        {
            printf("ERROR3");
        }      
        if(m[i][0] != '\0')
        {
            printf("%s", m[i]);
        }
    }

    printf("\n\n\n\t F.A.Q.\n");
    printf("\nERROR1 - '/cygdrive/' missing... Path must starts with /cygdrive/...");
    printf("\nERROR2 - forbidden symbols in the path! Don't use these symbols: *?<>\"|:");
    printf("\nERROR3 - forbidden combination of the symbols in the path! Maybe '//'...");
    printf("\n");

}