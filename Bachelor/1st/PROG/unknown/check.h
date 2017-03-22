#ifndef CHECK_H
#define CHECK_H

#define MAXPATH 261
#define MAXCNT 10
#define MAXSIZE (MAXPATH*MAXCNT)

#define T 1
#define F 0


void check(char m[MAXCNT][MAXPATH], char* bsymbols);
int startswith(char* str, char* prefix);
int symb(char symbol, char bsymbols[]);
void shift(char m[MAXCNT][MAXPATH], int i);
int contains(char* str1, char* str2);

#endif
