#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define _MAX_R 9999
#define _MIN_R 666

#define MAGIC 3733

#define NPLAYERS 9
#define NCARDS 52


// LAB 1
unsigned long long gcd(unsigned long long a, unsigned long long b);
long long ext_gcd(long long a, long long b, long long *x, long long *y);

long long pow_pow(long long a, long long x, long long m);

int is_prime(unsigned long long v);
unsigned long long get_prime(unsigned long long max, unsigned long long min, int reset);

unsigned long long dh_public(unsigned long long g, unsigned long long priv_key, unsigned long long p);
unsigned long long dh_shared(unsigned long long pub_key, unsigned long long priv_key, unsigned long long p);

int compare(const void *a, const void *b);
unsigned long long bstep_gstep(unsigned long long a, unsigned long long p, unsigned long long y);

// LAB 2
long long coprime(long long p, long long x, long long y);
long long inverse(long long res, long long p, long long x, long long cx);

void generate_keys(unsigned long long *cx, unsigned long long *dx, unsigned long long p, unsigned long long g);
void elgamal_encrypt(FILE *fp, FILE *fe, unsigned long long p, unsigned long long g, unsigned long long d);
void elgamal_decrypt(FILE *fe, FILE *fd, unsigned long long p, unsigned long long c);

void otp_encrypt(FILE *fp, FILE *fk, FILE *fe);
void otp_decrypt(FILE *fe, FILE *fk, FILE *fd);

void rsa_encrypt(FILE *fp, FILE *fe, unsigned long long d, unsigned long long n);
void rsa_decrypt(FILE *fe, FILE *fd, unsigned long long c, unsigned long long n);

// LAB 3
//unsigned long long hashy(char *fname);

//LAB 4
void shuffle_it(int arr[]);
void print_field(long long deck[], long long card[]);
void go_five(long long deck[], long long card[]);
