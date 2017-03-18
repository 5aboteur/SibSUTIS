#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define BUFSIZE 80

char* TW[2] = { "DO", "WHILE" };
char* TD[3] = { "(", ")", ";" };

char TID[10][BUFSIZE];
int id_cnt = 0;

int TNUM[BUFSIZE];
int num_cnt = 0;

char buf[BUFSIZE];
int buf_cnt = 0;

int c, d, d2, line = 1;
char e_c, s_c;

enum state { H, ID, NUM, FNUM, ASN, CSN, DLM, ER, END };
enum state CS; // cur state

FILE* fp;

void gch(void) { c = fgetc(fp); }

void clear(void)
{
	memset(buf, 0, sizeof(buf));
	buf_cnt = 0;
}

void add(void)
{
	buf[buf_cnt++] = c;
}

void makelex(int lineno, char* l, int flg)
{
	switch (flg) {
	case 0:
		printf("frr.pi:%d %s %s\n", lineno, l, buf);
		break;
	case 1:
		printf("frr.pi:%d %s %d\n", lineno, l, d);
		break;
	case 2:
		printf("frr.pi:%d %s %d%c%d\n", lineno, l, d, e_c, d2);
		break;
	case 3:
		printf("frr.pi:%d %s %d%c%c%d\n", lineno, l, d, e_c, s_c, d2);
		break;
	default:
		printf("Something is going wrong!\n");
		break;
	}
}

void id_or_word(void)
{
	for (int i = 0; i < 2; ++i) {
		if ( !(strcmp(TW[i], buf)) ) {
			makelex(line, "KEYWD", 0);
			return;
		}
	}

	makelex(line, "IDENT", 0);
}

int is_dlm(void)
{
	for (int i = 0; i < 3; ++i) {
		if ( !(strcmp(TD[i], buf)) )
			return 1;
	}

	return 0;
}

void scan (void)
{
	CS = H;

	fp = fopen("frr.pi","r");
	gch();

	do {
		switch (CS) {
		case H:
			if (c == ' ' || c == '\t') gch();
			else if ( isalpha(c) ) {
				clear();
				add();
				gch();
				CS = ID;
			}
			else if ( isdigit(c) ) {
				d = c - '0';
				gch();
				CS = NUM;
			}
			else if (c == ':') {
				clear();
				add();
				gch();
				CS = ASN;
			}
			else if (c == '<' || c == '>') {
				clear();
				add();
				gch();
				CS = CSN;
			}
			else if (c == '\n') {
				line++;
				gch();
			}
			else if (c == '^') {
				clear();
				add();
				makelex(line, "EOF", 0);
				CS = END;
			}
			else CS = DLM;
			break;
		case ID:
			while ( isalpha(c) || isdigit(c) ) {
				add();
				gch();
			}

			id_or_word();
			CS = H;
			break;
		case NUM:
			while ( isdigit(c) ) {
				d = d * 10 + (c - '0');
				gch();
			}
			if (c == '.' || c == 'e' || c == 'E') {
				e_c = c;
				gch();
				d2 = c - '0';
				CS = FNUM;
			}
			else {
				makelex(line, "NUMBER", 1);
				CS = H;
			}
			break;
		case FNUM:
			if ( isdigit(c) ) {
				while ( isdigit(c) ) {
					d2 = d2 * 10 + (c - '0');
					gch();
				}

				makelex(line, "FNUMBER", 2);
				CS = H;
			}
			else if (c == '+' || c == '-') {
				s_c = c;
				gch();
				if ( isdigit(c) ) {
					while ( isdigit(c) ) {
						d2 = d2 * 10 + (c - '0');
						gch();
					}

					makelex(line, "FNUMBER", 3);
					CS = H;
				}
				else CS = ER;
			}
			else {
				makelex(line, "NUMBER", 1);
				clear();
				buf[0] = e_c;
				CS = ID;
			}
			break;
		case ASN:
			if (c == '=') {
				add();
				gch();
				makelex(line, "ASSIGN", 0);
			}
			else {
				add();
				gch();
				makelex(line, "Unknown character", 0);
			}
			CS = H;
			break;
		case CSN:
			if (c == '=') {
				add();
				gch();
				makelex(line, "CMPSIGN", 0);
			}
			else {
				add();
				gch();
				makelex(line, "Unknown character", 0);
			}
			CS = H;
			break;
		case DLM:
			clear();
			add();

			if ( is_dlm() ) {
				makelex(line, "DELIM", 0);
				gch();
				CS = H;
			}
			else {
				makelex(line, "Unknown character", 0);
				gch();
				CS = H;
			}
		}
	} while (CS != END && CS != ER);

	if (CS == ER) printf("ERROR\n");
	else printf("OK\n");
}

int main()
{
	scan();
	return 0;
}
