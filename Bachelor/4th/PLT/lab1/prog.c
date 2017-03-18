#include <stdio.h>
#include <stdlib.h>

enum state { H, C, D, CS, DS, S };
enum state ST;

int main()
{
	FILE *fin;

	int cstr;
	int inc = 0;

	if (!(fin = fopen("file.in", "r"))) {
		printf("Error: cannot open the file.\n");
		return -1;
	}

//	fseek(fin, 0L, SEEK_END);
//	cstr = ftell(fin);

//	printf("Num of str: %d\n\n", cstr);

//	rewind(fin);

	for (int j = 1; j <= 5; ++j) {

		char ch;
		char buf[256];
		ST = H;

		for (int i = 0; ((ch = fgetc(fin)) != '\n'); ++i) {
			if (ch == '1' && ST == H) {
				ST = C;
				buf[i] = ch;
			}
			else if (ch == '0' && ST == H) {
				ST = D;
				buf[i] = ch;
			}
			else if (ch == '0' && ST == C) {
				ST = DS;
				buf[i] = ch;
			}
			else if (ch == '1' && ST == D) {
				ST = CS;
				buf[i] = ch;
			}
			else if (ch == '1' && ST == CS) {
				ST = S;
				buf[i] = ch;
			}
			else if (ch == '0' && ST == CS) {
				ST = DS;
				buf[i] = ch;
			}
			else if (ch == '0' && ST == DS) {
				ST = S;
				buf[i] = ch;
			}
			else if (ch == '1' && ST == DS) {
				ST = CS;
				buf[i] = ch;
			}
			else if (ch == '1' && ST == S) {
				ST = S;
				buf[i] = ch;
			}
			else if (ch == '0' && ST == S) {
				ST = S;
				buf[i] = ch;
			}
			else {
//				printf("String #%d is incorrect! (%s) FOR\n", j, buf);
				inc = 1;
			}
		}

		if ((ST == S || ST == DS || ST == CS) && (inc == 0)) printf("String #%d is correct. \n", j);
		else printf("String #%d is incorrect! \n", j);

		inc = 0;
	}

	fclose(fin);

	return 0;
}
