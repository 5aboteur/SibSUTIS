#include "lib.h"

/*********************************************
*					     *
*------------- LIBRARY CONTENT --------------*
*					     *
* gcd() | ext_gcd() | pow_pow() | is_prime() *
* get_prime() | dh_public() | dh_shared()    *
* bstep_gstep() | onetime_pad() | etc ...    *
*					     *
*********************************************/

void menu()
{
	puts("");
	puts("******************************************");
	puts(" 1: a^x mod m = ? [pow_pow() method]");
	puts(" 2: GCD(a, b) = ? [ext_gcd() method]");
	puts(" 3: Diffie-Hellman method simulation");
	puts(" 4: Baby-step Giant-step simulation");
	puts(" 5: Shamir method simulation");
	puts(" 6: ElGamal method simulation");
	puts(" 7: Vernam method simulation");
	puts(" 8: RSA method simulation");
	puts(" 9: RSA signature");
	puts("10: ElGamal signature");
	puts("11: DSA signature");
	puts("12: Quit");
	puts("******************************************");
	printf(">");
}

int main()
{
	char ch, rd;
	int choice;
	long long a, b, e, g, h, k, m, r, s, u, v, w, x, y, z, msg, res = 0, u1, u2,
		n, na, nb, p, pa, pb, q, qa, qb, c, ca, cb, d, da, db, f, fa, fb,
		alice_pub, alice_priv, bob_pub, bob_priv, alice_shd, bob_shd;

	FILE *fk, *fp, *fe, *fd;

	do {
		menu();
		scanf("%d", &choice);
	} while (choice <= 0 || choice > 12);

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
			q = get_prime(_MAX_R, _MIN_R, 1);
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
		srand((unsigned)time(NULL));

		p = get_prime(_MAX_R, _MIN_R, 0);

		printf("Input msg < %lli (p): ", p);
		scanf("%lli", &msg);

		if (msg > p)
		{
			msg %= p;
			printf("Input msg is too long. New msg (msg %% p) = %lli\n", msg);
		}

		ca = coprime(p - 1, x, y);
		cb = coprime(p - 1, x, y);

		da = inverse(res, p - 1, x, ca);
		db = inverse(res, p - 1, x, cb);

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
		srand((unsigned)time(NULL));

		do {
			q = get_prime(_MAX_R, _MIN_R, 0);
			p = 2 * q + 1;
		} while (!is_prime(p));

		for (g = 2; g <= 10; ++g)
		{
			if (pow_pow(g, q, p) == 1) break;
		}

		generate_keys(&ca, &da, p, g);
		generate_keys(&cb, &db, p, g);

		printf("ca: %lli, da: %lli, cb: %lli, db: %lli\n", ca, da, cb, db);

		fp = fopen("message.in", "r");
		fe = fopen("encrypt.out", "w+");
		fd = fopen("decrypt.out", "w");

		elgamal_encrypt(fp, fe, p, g, db);
		fclose(fp);

		//fseek(fe, 0L, SEEK_SET);

		elgamal_decrypt(fe, fd, p, cb);
		fclose(fe);
		fclose(fd);
		break;
	case 7:
		fk = fopen("key.z", "w+");
		fp = fopen("message.in", "r");
		fe = fopen("encrypt.out", "w+");
		fd = fopen("decrypt.out", "w");

		otp_encrypt(fp, fk, fe);

		fclose(fp);

		otp_decrypt(fe, fk, fd);

		fclose(fk);
		fclose(fe);
		fclose(fd);
		break;
	case 8:
		pa = get_prime(_MAX_R, _MIN_R, 1);
		qa = get_prime(_MAX_R, _MIN_R, 0);
		na = pa * qa;
		fa = (pa - 1) * (qa - 1);
		printf("pa: %lli, qa: %lli, na: %lli, fa: %lli\n", pa, qa, na, fa);

		pb = get_prime(_MAX_R, _MIN_R, 0);
		qb = get_prime(_MAX_R, _MIN_R, 0);
		nb = pb * qb;
		fb = (pb - 1) * (qb - 1);
		printf("pb: %lli, qb: %lli, nb: %lli, fb: %lli\n", pb, qb, nb, fb);

		da = coprime(fa, x, y);
		db = coprime(fb, x, y);

		ca = inverse(res, fa, x, da);
		cb = inverse(res, fb, x, db);

		printf("ca: %lli, da: %lli, cb: %lli, db: %lli\n", ca, da, cb, db);
		printf("cb*db mod hi = %lli\n\n", cb * db % fb);
		fp = fopen("message.in", "r");
		fe = fopen("encrypt.out", "w+");
		fd = fopen("decrypt.out", "w");

		rsa_encrypt(fp, fe, db, nb);
		fclose(fp);
		rsa_decrypt(fe, fd, cb, nb);
		fclose(fe);
		fclose(fd);
		break;
	case 9:
		srand((unsigned)time(NULL));

		p = get_prime(_MAX_R, _MIN_R, 0);
		q = get_prime(_MAX_R, _MIN_R, 0);

		n = p * q;
		f = (p - 1) * (q - 1);
		printf("p: %lli, q: %lli, n: %lli, f: %lli\n", p, q, n, f);

		d = coprime(f, x, y);
		c = inverse(res, f, x, d);
		printf("c: %lli, d: %lli\n", c, d);
		printf("c*d mod phi = %lli\n", c * d % f);

		h = hashy("message.in") % f;
		printf("h: \033[32m%lli\033[0m\n", h);

		s = pow_pow(h, c, n) % f;
		printf("\n *** s: %lli ***\n\n", s);

		fp = fopen("signature", "w");
		fprintf(fp, "%lli", s);
		fclose(fp);

		w = pow_pow(s, d, n) % f;
		printf("w: \033[32m%lli\033[0m\n", w);

		if (w == h) printf("Signature is correct.\n");
		else printf("Wrong signature.\n");
		break;
	case 10:
		srand((unsigned)time(NULL));

		do {
			q = get_prime(_MAX_R * _MAX_R, _MIN_R, 0);
			p = 2 * q + 1;
		} while (!is_prime(p));

		for (g = 2; g <= 10; ++g)
		{
			if (pow_pow(g, q, p) == 1) break;
		}
		printf("p: %lli, g: %lli\n", p, g);

		generate_keys(&z, &w, p, g);
		printf("z: %lli, w: %lli\n", z, w);

		h = hashy("message.in") % p;
		printf("h: %lli\n", h);

		k = coprime(p - 1, x, y);
		printf("k: %lli\n", k);

		r = pow_pow(g, k, p);
		u = (h - z * r) % (p - 1);
		if (u < 0) u += p - 1;
		printf("r: %lli, u: %lli\n", r, u);

		s = (inverse(res, p - 1, x, k) * u) % (p - 1);
		printf("\n *** s: %lli ***\n\n", s);

		fp = fopen("signature", "w");
		fprintf(fp, "%lli", s);
		fclose(fp);

		fa = pow_pow(w,r,p);
		fb = pow_pow(r,s,p);
		printf("(left side) fa: %lli, fb: %lli, fa*fb mod p: \033[34m%lli\033[0m\n", fa, fb, fa*fb%p);

		f = pow_pow(g, h, p);
		printf("(right side) f: \033[34m%lli\033[0m\n", f);

		if ((fa * fb % p) == f)
			printf("Signature is correct.\n");
		else printf("Wrong signature.\n");

		break;
	case 11:
		srand((unsigned)time(NULL));

		do {
			q = get_prime(_MAX_R, _MIN_R, 0);
			b = rand() % _MIN_R + 1;
			p = b * q + 1;
		} while (!is_prime(p));

		printf("p: %lli, q: %lli\n", p, q);

		do {
			g = rand() % _MAX_R + 1;
			a = pow_pow(g, (p - 1) / q, p);
		} while (a == 1);

		printf("a: %lli\n", a);

		z = rand() % (q - 1) + 1;
		w = pow_pow(a, z, p);
		printf("z: %lli, w: %lli\n", z, w);

		h = hashy("message.in") % q;
		printf("h: %lli\n", h);

		while (1) {
			k = rand() % (q - 2) + 1;
			r = pow_pow(a, k, p) % q;

			if (r == 0) continue;

			s = (k * h + z * r) % q;

			if (s == 0) continue;

			printf("k: %lli, r: \033[33m%lli\033[0m\n", k, r);
			printf("\n *** s: %lli ***\n\n", s);

			fp = fopen("signature", "w");
			fprintf(fp, "%lli", s);
			fclose(fp);

			break;
		}

		if (r <= 0 || r >= q || s <= 0 || s >= q)
			printf("Wrong signature.\n");
		else {
			b = inverse(res, q, x, h);
			u1 = s * b % q;
			u2 = (-r) * b % q;
			if (u2 < 0) u2 += q;
			printf("u1: %lli, u2: %lli\n", u1, u2);

			fa = pow_pow(a, u1, p);
			fb = pow_pow(w, u2, p);
			f = fa * fb % p;

			v = f % q;
			printf("v: \033[33m%lli\033[0m\n", v);

			if (v == r) printf("Signature is correct.\n");
			else printf("Wrong signature.\n");
		}

		break;
	case 12:
		puts("Goodbye! ^.^"); break;

	default:
		return -1;
	}

	return 0;
}
