#include "lib.c"

 /*********************************************
 *					      *
 *------------- LIBRARY CONTENT --------------*
 *					      *
 * gcd() | ext_gcd() | pow_pow() | is_prime() *
 * get_prime() | dh_public() | dh_shared()    *
 * bstep_gstep() | onetime_pad() ..
 *					      *
 *********************************************/

void menu()
{
	puts("");
	puts("\033[1;37m******************************************\033[0m");
	puts("\033[32m 1: a^x mod m = ? [pow_pow() method]");
	puts("\033[33m 2: GCD(a, b) = ? [ext_gcd() method]");
	puts("\033[34m 3: Diffie-Hellman method simulation");
	puts("\033[35m 4: Baby-step Giant-step simulation");
	puts("\033[36m 5: Shamir method simulation");
	puts("\033[2;32m 6: ElGamal method simulation");
	puts("\033[2;33m 7: Vernam method simulation");
	puts("\033[2;35m 8: RSA method simulation");
	puts("\033[1;31m 9: Quit");
	puts("\033[37m******************************************\033[0m");
	printf(">");
}

int main()
{
	char ch, rd;
	int choice;
	long long a, b, e, g, k, m, n, na, nb, p, pa, pb, q, qa, qb, r, x, y, ca, cb, da, db, msg, res,
		  fa, fb, alice_pub, alice_priv, bob_pub, bob_priv, alice_shd, bob_shd;

	FILE *fk, *fp, *fe, *fd;

	do {
		menu();
		scanf("%d", &choice);
	} while (choice <= 0 || choice > 9);

	switch (choice)
	{
		case 1:
			printf("Input base, power, modulus: ");
			scanf("%lli %lli %lli", &b, &p, &m);
			res = pow_pow(b, p, m);
			printf("%lli^%lli mod %lli = %lli\n",
				b, p, m, res);
			break;
		case 2:
			printf("Input a, b: ");
			scanf("%lli %lli", &a, &b);
			res = ext_gcd(a, b, &x, &y);
			printf("GCD(%lli, %lli) = %lli; X = %lli, Y = %lli\n",
				a, b, res, x, y);
			break;
		case 3:
			puts("Computing..");

			do {
				q = get_prime(_MAX_R, _MIN_R);
				p = 2 * q + 1;
			} while (!is_prime(p));

			for (g = 2; g <= 10; ++g)
			{
				if (pow_pow(g, q, p) == 1) break;
			}

			alice_priv = rand() % (_MIN_R + _MIN_R / 2);
			bob_priv = rand() % (2 * _MIN_R + _MIN_R);

			alice_pub = dh_public(g, alice_priv, p);
			bob_pub = dh_public(g, bob_priv, p);

			alice_shd = dh_shared(bob_pub, alice_priv, p);
			bob_shd = dh_shared(alice_pub, bob_priv, p);

			puts("Done!");

			printf("p = %lli, g = %lli;\n", p, g);
			printf("Alice`s keys: [PUB: %lli, PRV: %lli, SHD: %lli]\n",
				alice_pub, alice_priv, alice_shd);
			printf("Bob`s keys: [PUB: %lli, PRV: %lli, SHD: %lli]\n",
				bob_pub, bob_priv, bob_shd);
			break;
		case 4:
			printf("Input a, p, y: ");
			scanf("%lli %lli %lli", &a, &p, &y);

			res = bstep_gstep(a, p, y);

			printf("x = %lli\n", res);
			if (res == -1) break;

			printf("Let's test: %lli^%lli mod %lli = %lli\n",
				a, res, p, pow_pow(a, res, p));
			break;
		case 5:
			srand(time(NULL));

			p = get_prime(_MAX_R, _MIN_R);

			printf("Input msg < %lli (p): ", p);
			scanf("%lli", &msg);

			do {
				ca = rand() % (p - 1) + 3;
				if (!(ca % 2) || ca == cb) continue;
			} while (ext_gcd(p - 1, ca, &x, &y) != 1);

			do {
				cb = rand() % (p - 1) + 3;
				if (!(cb % 2) || cb == ca) continue;
			} while (ext_gcd(p - 1, cb, &x, &y) != 1);

			res = ext_gcd(p - 1, ca, &x, &da);

			if (da < 0) da += p - 1;

			res = ext_gcd(p - 1, cb, &x, &db);

			if (db < 0) db += p - 1;

			puts("ci * di mod (p - 1) = 1 ?");
			printf("%lli * %lli mod %lli = %lli\n", ca, da, p - 1, ca * da % (p - 1));
			printf("%lli * %lli mod %lli = %lli\n", cb, db, p - 1, cb * db % (p - 1));

			res = pow_pow(msg, ca, p);
			res = pow_pow(res, cb, p);
			res = pow_pow(res, da, p);
			res = pow_pow(res, db, p);

			printf("Result: %lli that equals your 'msg', ain`t it?\n", res);
			break;
		case 6:
			srand(time(NULL));

			do {
				q = get_prime(_MAX_R, _MIN_R);
				p = 2 * q + 1;
			} while (!is_prime(p));

			for (g = 2; g <= 10; ++g)
			{
				if (pow_pow(g, q, p) == 1) break;
			}

			ca = rand() % (p - 1) + 2;
			da = pow_pow(g, ca, p);

			cb = rand() % (p - 1) + 2;
			db = pow_pow(g, cb, p);

			printf("%lli %lli %lli %lli\n", ca, da, cb, db);

			fp = fopen("message.in", "r");
			fe = fopen("encrypt.out", "w+");
			fd = fopen("decrypt.out", "w");

			while ((ch = fgetc(fp)) != EOF)
			{
				k = rand() % (p - 2) + 1;

				r = pow_pow(g, k, p);
				e = (int) ch * pow_pow(db, k, p);

				fprintf(fe, "%lli %lli\n", r, e);
			}

			fclose(fp);

			fseek(fe, 0L, SEEK_SET);

			while (!feof(fe))
			{
				fscanf(fe, "%lli", &r);
				fscanf(fe, "%lli", &e);

				ch = (char)(e * pow_pow(r, p - 1 - cb, p) % p);

				fputc(ch, fd);
				fgetc(fe);
			}

			fclose(fe);
			fclose(fd);
			break;
		case 7:
			srand(time(NULL));

			fk = fopen("z.key", "w+");
			fp = fopen("message.in", "r");
			fe = fopen("encrypt.out", "w+");
			fd = fopen("decrypt.out", "w");


			long long j = 0;
			while ((ch = fgetc(fp)) != EOF)
			{
				do {
					rd = (char) rand() % 255;
				} while (rd != EOF);
				fputc(rd, fk);

				ch = onetime_pad(ch, rd);
				fputc(ch, fe);
				++j;
			}

			fclose(fp);

			fseek(fe, 0L, SEEK_SET);
			fseek(fk, 0L, SEEK_SET);

			j = 0;
			while ((ch = fgetc(fe)) != EOF)
			{
				rd = fgetc(fk);

				ch = onetime_pad(ch, rd);
				fputc(ch, fd);
				++j;
			}

			fclose(fk);
			fclose(fe);
			fclose(fd);
			break;
		case 8:
			srand(time(NULL));

			do {
				q = get_prime(_MAX_R, _MIN_R);
				pa = 2 * q + 1;
			} while (!is_prime(pa));

			do {
				q = get_prime(_MAX_R, _MIN_R);
				qa = 2 * q + 1;
			} while (!is_prime(pb) && pa != qa);

			na = pa * qa;
			fa = (pa - 1) * (qa - 1);
			printf("%lli %lli %lli %lli\n", pa, qa, na, fa);

			do {
				q = get_prime(_MAX_R, _MIN_R);
				pb = 2 * q + 1;
			} while (!is_prime(pb) && pa != pb);

			do {
				q = get_prime(_MAX_R, _MIN_R);
				qb = 2 * q + 1;
			} while (!is_prime(qb) && pb != qb && qb != qa);

			nb = pb * qb;
			fb = (pb - 1) * (qb - 1);
			printf("%lli %lli %lli %lli\n", pb, qb, nb, fb);

			do {
				da = rand() % fa;
			} while (ext_gcd(fa, da, &x, &y));

			do {
				db = rand() % fb;
			} while (ext_gcd(fb, db, &x, &y));

			res = ext_gcd(fa, da, &x, &ca);
			if (ca < 0) ca += fa;

			res = ext_gcd(fb, db, &x, &cb);
			if (cb < 0) cb += fb;


			printf("%lli %lli %lli %lli\n", ca, da, cb, db);

			fp = fopen("msg.in", "r");
			fe = fopen("encrypt.out", "w+");
			fd = fopen("decrypt.out", "w");

			while ((ch = fgetc(fp)) != EOF)
			{
				e = pow_pow((int)ch, db, nb);

				fprintf(fe, "%lli\n", e);
			}

			fclose(fp);

			fseek(fe, 0L, SEEK_SET);

			while (!feof(fe))
			{
				fscanf(fe, "%lli", &e);

				ch = (char) pow_pow(e, cb, nb);

				fputc(ch, fd);
				fgetc(fe);
			}

			fclose(fe);
			fclose(fd);
			break;
		case 9:
			puts("Goodbye! ^.^"); break;

		default:
			return -1;
	}

	return 0;
}
