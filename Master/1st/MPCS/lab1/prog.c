#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main()
{
	srand((unsigned)time(NULL));

	char alphabet[10] = "zxcvbyouai";
	char vowel[5] = "youai";
	char consonant[5] = "zxcvb";

	char buf1[11], buf2[11], buf3[11], buf4[11];

	int i, p, p1, p2, p3, p4;
	p = p1 = p2 = p3 = p4 = 0;

	/***** routine *****/
	while (0x1) {

//		printf("| %2d | %2d | %2d | %2d | %2d |\n",
//			p, p1, p2, p3, p4);

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
			// all
			buf1[p1] = alphabet[p1];
			p1++;
			break;
		case 2:
			// vowel
			for (i = 0; i < 5; i++) {
				if (alphabet[p2] == vowel[i]) {
					buf2[i] = alphabet[p2];
					break;
				}
			}
			p2++;
			break;
		case 3:
			// consonant
			for (i = 0; i < 5; i++) {
				if (alphabet[p3] == consonant[i]) {
					buf3[i] = alphabet[p3];
					break;
				}
			}
			p3++;
			break;
		case 4:
			// asterisk
			buf4[p4] = '*';
			p4++;
			break;
		}

		// check if done
		if ((p1 == 10) && (p2 == 10) && (p3 == 10) && (p4 == 10)) {
			buf1[10] = '\0';
			buf2[5]  = '\0';
			buf3[5]  = '\0';
			buf4[10] = '\0';
			break;
		}

	}

	printf("p1: %d\np2: %d\np3: %d\np4: %d\n\n", p1, p2, p3, p4);
	printf("buf1: %s\nbuf2: %s\nbuf3: %s\nbuf4: %s\n\n",
		buf1, buf2, buf3, buf4);
	printf("Elapsed time: %.6f ms.\n",
		(float)clock() / CLOCKS_PER_SEC * 1000.0f);

	return 0;
}
