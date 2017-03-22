#include "check.h"

void check(char m[MAXCNT][MAXPATH], char* bsymbols)
{
    int i, j;    
    for(i=0;i<MAXCNT;i++)
    {
        if(((startswith(m[i], "/cygdrive/") == F) && (m[i][0] != '\0')))
        {
            shift(m, i);     
            m[i][0]='\0';
            m[i][1]='1';
        }
    }
    for(i=0;i<MAXCNT;i++)
    {
        for(j=0;m[i][j] != 0;j++)
        {
            if((((symb(m[i][j], bsymbols)) == T) && (m[i][0] != '\0')))
            {
                shift(m, i);
                m[i][0]='\0';
                m[i][1]='2';
            }
        }
    }
    for(i=0;i<MAXCNT;i++)
    {
        for(j=0;m[i][j] != 0;j++)
        {
            if(((m[i][j]=='/')&&(m[i][j+1]=='/')) && (m[i][0] != '\0'))
            {
                shift(m, i);
                m[i][0]='\0';
                m[i][1]='3';
            }
        }
    }
}


int startswith(char* str, char* prefix)
{
    int i=0;
    while(1)
    {
        if(str[i] != 0 && prefix[i] != 0)
        {
            if(str[i] != prefix[i])
            {
                return F;
            }
            else
            {
            }
        }
        else
        {
            if(str[i] == 0 && prefix[i] == 0)
            {
                return T;
            }
            else
            {
                if(prefix[i] == 0)
                {
                    return T;
                }
                else
                {
                    return F;
                }
            }
        }
        i++;
    }
}

int symb(char symbol, char bsymbols[])
{
    int i;
    for(i=0;bsymbols[i] != 0;i++)
    {
        if(symbol==bsymbols[i])
        {
            return T;
        }
    }
    return F;
}

void shift(char m[MAXCNT][MAXPATH], int i)
{
    int l;
    for(l=0;m[i][l] != '\0';l++)
    {
    }
    for(;l>=0;l--)
    {
        m[i][l+2] = m[i][l];
    }
}


int contains(char* str1, char* str2)
{
	int i, j, ok;
	for(i=0;str1[i] != 0;i++)
	{
		if(str1[i] == str2[0])
		{
			for(j=1;str2[j] != 0;j++)
			{
				if(str1[i+j] != str2[j])
				{
					break;
				}
			}
			if(str2[j] == 0)
			{
				return T;
			}
		}
	}
	return F;
}
