#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int main()
{
	srand((unsigned)time(NULL));

	char alphabet[10] = "zyxocuvabi";
	char vowel[5] = "youai";
	char consonant[5] = "zxcvb";

	char buf1[10], buf2[10], buf3[10], buf4[10];

	memset(buf1, 0, sizeof(char) + 10);
	memset(buf2, 0, sizeof(char) + 10);
	memset(buf3, 0, sizeof(char) + 10);
	memset(buf4, 0, sizeof(char) + 10);

	int i, p, p1, p2, p3, p4, cnt2 = 0, cnt3 = 0;
	p = p1 = p2 = p3 = p4 = 0;

	/***** routine *****/
	while (0x1) {

		do {
			p = 1 + rand() % 4;

				 if ((p == 1) && (p1 < 10)) break;
			else if ((p == 2) && (p2 < 10)) break;
			else if ((p == 3) && (p3 < 10)) break;
			else if ((p == 4) && (p4 < 10)) break;
			else continue;

		} while (0x1);

		switch (p) {
		case 1:
			// random
			buf1[p1] = alphabet[p1];
			p1++;
			break;
		case 2:
			// vowel
			for (i = 0; i < 5; i++) {
				if (alphabet[p2] == vowel[i]) {
					buf2[cnt2++] = alphabet[p2];
					break;
				}
			}
			p2++;
			break;
		case 3:
			// consonant
			for (i = 0; i < 5; i++) {
				if (alphabet[p3] == consonant[i]) {
					buf3[cnt3++] = alphabet[p3];
					break;
				}
			}
			p3++;
			break;
		case 4:
			// asterisk
			alphabet[p4] = '*';
			p4++;
			break;
		}

		if ((p1 == 10) && (p2 == 10) && (p3 == 10) && (p4 == 10))
			break;

	}

	printf("p1: %d\np2: %d\np3: %d\np4: %d\n\n", p1, p2, p3, p4);
	printf("buf1: %s\nbuf2: %s\nbuf3: %s\n\n",
		buf1, buf2, buf3);
	printf("Elapsed time: %.6f ms.\n",
		(float)clock() / CLOCKS_PER_SEC * 1000.0f);

	return 0;
}
