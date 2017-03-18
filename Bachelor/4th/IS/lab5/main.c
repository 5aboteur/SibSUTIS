#include "lib.h"

int udosh = 31337;
int edosh = 555555;
int ddosh = 0;

void main_menu()
{
	printf("\033[1;32m\n\nYOU: $%d ESTORE: $%d \033[1;33m(DEAL: $%d)\033[34m\n", udosh, edosh, ddosh);
	puts("****************************");
	puts("*** WELCOME 2 THE ESTORE ***");
	puts("****************************\n");

	puts("*** 1. Buy items\t ***");
	puts("*** 2. Make a deal\t ***");
	puts("*** 3. Quit\t\t ***");

	puts("\n****************************");
	puts("****************************");
	printf(">\033[0m");
}


void items_menu()
{
	printf("\033[1;32m\n\nYOU: $%d ESTORE: $%d \033[1;33m(DEAL: $%d)\033[34m\n", udosh, edosh, ddosh);
	puts("******************************");
	puts("*** 1. Sexy bear ($100)\t   ***");
	puts("*** 2. Holy bible ($10000) ***");
	puts("*** 3. XXX VHS tape (250)  ***");
	puts("*** 4. Devil mask ($500)   ***");
	puts("*** 5. Back\t\t   ***");
	printf(">\033[0m");
}


int main()
{
	FILE *fin;

	int choice = 0, num = 0, cnt = 0, i = 0;

	long long c[NDOSH], d[NDOSH], ord[10], type[10000];
	long long x, y, p, q, n, N, phi, r, s, nn, ss, fn, sf, res = 0;

	while (1) {
		p = get_prime(_MAX_R, _MIN_R, 1);
		q = get_prime(_MAX_R, _MIN_R, 0);

		N = p * q;
		phi = (p - 1) * (q - 1);

		printf("p: %lli, q: %lli, N: %lli, phi: %lli\n", p, q, N, phi);

		for (int i = 0; i < NDOSH; ++i) {
			c[i] = coprime(phi, x, y);
			d[i] = inverse(res, phi, x, c[i]);

			printf("%lli * %lli mod %lli = %lli ", c[i], d[i], phi, c[i] * d[i] % phi);
			switch (i) {
			case 0:
				printf("(for $5)\n");
				break;
			case 1:
				printf("(for $10)\n");
				break;
			case 2:
				printf("(for $50)\n");
				break;
			case 3:
				printf("(for $100)\n");
				break;
			case 4:
				printf("(for $500)\n");
				break;
			case 5:
				printf("(for $1000)\n");
				break;
			case 6:
				printf("(for $5000)\n");
				break;
			default:
				printf("BADa#^^^.\n");
				break;
			}
		}

		while (1) {
			main_menu();
			scanf("%d", &choice);

			switch (choice) {
			case 1:
				while (1) {
					char ch = 'a';

					items_menu();
					scanf("%d", &choice);

					switch (choice) {
					case 1:
						++ord[3]; ++num;
						ddosh += 100;
						break;
					case 2:
						ord[6] += 2; num += 2;
						ddosh += 10000;
						break;
					case 3:
						ord[3] += 2; ++ord[2];
						num += 3; ddosh += 250;
						break;
					case 4:
						++ord[4]; ++num;
						ddosh += 500;
						break;
					case 5:
						ch = 'b';
						break;
					default:
						printf("Try again.\n");
						break;
					}

					if (ddosh > udosh) { puts("Pff! Don`t tryna fool me!"); return -1; }

					puts("Order complete!");
					if (ch == 'b') break;
				}

				break;
			case 2:
				if (ddosh == 0) { puts("No deal there"); break; }

				fin = fopen("deals.txt", "w+");

				while (cnt != num) {
					n = rand() % (N - 1) + 2;

					for (i = 0; i < NDOSH; ++i) {
						if (ord[i]) { --ord[i]; break; }
					}

					fn = hashy(n) % N;
					r = coprime(N, x, y);
					nn = (fn * pow_pow(r, d[i], N)) % N;

					sf = pow_pow(nn, c[i], N);
//					printf("(BEFORE) n: %lli, fn: %lli, sf: %lli ", n, fn, sf);

					switch (i) {
					case 0:
						printf("(for $5)\n");
						break;
					case 1:
						printf("(for $10)\n");
						break;
					case 2:
						printf("(for $50)\n");
						break;
					case 3:
						printf("(for $100)\n");
						break;
					case 4:
						printf("(for $500)\n");
						break;
					case 5:
						printf("(for $1000)\n");
						break;
					case 6:
						printf("(for $5000)\n");
						break;
					default:
						printf("BADa#^^^.\n");
						break;
					}

					switch (i) {
					case 0:
						udosh -= 5; break;
					case 1:
						udosh -= 10; break;
					case 2:
						udosh -= 50; break;
					case 3:
						udosh -= 100; break;
					case 4:
						udosh -= 500; break;
					case 5:
						udosh -= 1000; break;
					case 6:
						udosh -= 5000; break;
					default:
						return -1;
					}

					r = inverse(res, N, x, r);
					sf = (sf * r) % N;

					fprintf(fin, "%lli %lli\n", n, sf);
//					printf("(AFTER) %lli\n", sf);

					++cnt;
				}

				printf("Out from while.\n");

				rewind(fin);
				printf("=============\n");

				while (ddosh) {
					fscanf(fin, "%lli%lli", &n, &sf);
					fn = hashy(n) % N;

//					printf("n: %lli, fn: %lli, sf: %lli\n", n, fn, sf);

					for (int i = 0; i < NDOSH; ++i) {
						if (pow_pow(sf, d[i], N) == fn) {
							switch (i) {
							case 0:
								edosh += 5;
								ddosh -= 5;
								break;
							case 1:
								edosh += 10;
								ddosh -= 10;
								break;
							case 2:
								edosh += 50;
								ddosh -= 50;
								break;
							case 3:
								edosh += 100;
								ddosh -= 100;
								break;
							case 4:
								edosh += 500;
								ddosh -= 500;
								break;
							case 5:
								edosh += 1000;
								ddosh -= 1000;
								break;
							case 6:
								edosh += 5000;
								ddosh -= 5000;
								break;
							default:
								return -1;
							}
						}
					}
				}

				break;
			case 3:
				printf("Goodbye! ^.^\n");
				return 0;
			default:
				printf("Bad choice.\n");
				break;
			}
		}
	}

	return 0;
}
