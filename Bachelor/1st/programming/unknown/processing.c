#include <stdio.h>

#define MAXPATH 261
#define MAXCNT 10
#define MAXSIZE (MAXPATH*MAXCNT)

#define T 1
#define F 0


void process(char m[MAXCNT][MAXPATH])
{     
    int i, j;
    for(i=0;i<MAXCNT;i++)
    {
        if(m[i][0] != '\0')
        {
            m[i][0] = m[i][10];
            m[i][1] = ':';
            m[i][2] = '\\';
            for(j=3;m[i][j+9] != 0;j++)
            {
                m[i][j]=m[i][j+9];
            }
            m[i][j]=0;
        }
    }
    for(i=0;i<(MAXCNT);i++)
    {
        for(j=0;m[i][j] != '\0';j++)
        {
            if(m[i][j] == '/')
            {
                m[i][j] = '\\';
            }
        }
    }   
}

void process2(char m[MAXPATH])
{     
    int j;
    
    {
        if(m[0] != '\0')
        {
            m[0] = m[10];
            m[1] = ':';
            m[2] = '\\';
            for(j=3;m[j+9] != 0;j++)
            {
                m[j]=m[j+9];
            }
            m[j]=0;
        }
    }
    {
        for(j=0;m[j] != '\0';j++)
        {
            if(m[j] == '/')
            {
                m[j] = '\\';
            }
        }
    }   
}


void token(char delim, char s[MAXSIZE], char s1[MAXCNT][MAXPATH]) 
{
    int i, j=0, k=0;
    for(i=0;s[i] != '\0';i++)
    {
        if (k==MAXPATH-1)
        {
            printf("Your path is too long! max symbols - 260");
            s[i]='\0';
            s[i+1]='\0';
            j=-1;
        }
        s1[j][k] = s[i];
        k++; 
        if(delim==s[i])
        {
            k--;
            s1[j][k]='\0';
            if(k!=0)
            {
                j++;
                k=0;
            }
        }
    }     
    s1[j][k] = '\0';
    if (s1[j][0]=='\0')
        j--;
    if (j>=MAXCNT)
    {
        j=-1;
        printf("Too many paths... max 10!");
    }
    j++;
    for(;j<MAXCNT;j++)
    {
        s1[j][0]='\0';
        s1[j][1]='\0';
    }
}