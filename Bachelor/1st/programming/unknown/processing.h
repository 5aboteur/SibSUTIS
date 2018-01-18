#ifndef PROCESSING_H
#define PROCESSING_H

#define MAXPATH 261
#define MAXCNT 10
#define MAXSIZE (MAXPATH*MAXCNT)

void process2(char m[MAXPATH]);
void process(char m[MAXCNT][MAXPATH]);
void token(char delim, char s[MAXSIZE], char s1[MAXCNT][MAXPATH]);

#endif
