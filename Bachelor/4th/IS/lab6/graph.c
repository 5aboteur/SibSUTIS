/*
 * File:    graph.c
 * Author:  SillyHat
 *
 * Cool stuff!
 */

#include <stdio.h>
#include <string.h>
#include "lib.c"

#define CLR_NUM 3
#define A       2
#define V_MIN	10
#define V_MAX	500

FILE *bob_says;
FILE *putout;

/*
 * Prototypes, yay!
 */

void get_rsa_parameters(long long *n, long long *c, long long *d);
void get_r(int color, long long n, long long *r);
void get_z(long long r, long long d, long long n, long long *z);
int set_graph_parameters();
int get_graph_parameters();
void set_vertex_parameters();
void shuffle_colors(int *array);
void swap(int *a, int *b);
void print_vertex_set(int mode);
void print_edge_set();
int bob_routine();
void terrible_error_137();

const int V, E;
const char* colors[CLR_NUM] = {"Red", "Blue", "Yellow"};

/*
 * Vertex structure, edge structure.
 */

struct vertex {
    int index;
    int color;
    long long r;
    long long n;
    long long c;
    long long d;
    long long z;
};

struct edge {
    struct vertex *a;
    struct vertex *b;
};

struct vertex *vertex_set;
struct edge *edge_set;

/*
 * Getting parameters: RSA protocol, R and Z.
 */

void get_rsa_parameters(long long *n, long long *c, long long *d) {
    long long dump, p, phi, q;

    p = prime_number_generator(1000, 40000, 1);
    q = prime_number_generator(1000, 40000, 1);
    *n = p * q;
    phi = (p - 1) * (q - 1);

    do {
        *c = ((rand() % (phi - 2)) + 2) | 1;
    } while (euclid(phi, *c, &dump, &(*d)) != 1);

    if (*d < 0) {
        *d += phi;
    }
}

void get_r(int color, long long n, long long *r) {
    *r = (((rand() % (n - 10000)) + 10000) & ~3) | color;
}

void get_z(long long r, long long d, long long n, long long *z) {
    *z = pow_mod(r, d, n);
}

/*
 * Generate random simple graph, write it to a file.
 */

int set_graph_parameters() {
	int i, temp_v;
	FILE *putin;

	putin = fopen("graph.in", "w");
	if (putin == NULL) {
        fprintf(putout, "\033[31mError opening file!\033[0m\n");
        return -1;
    }

	temp_v = (rand() % (V_MAX - V_MIN)) + V_MIN;
	fprintf(putin, "%d %d\n", temp_v, (temp_v - 2) * 2);

	for (i = 0; i < (temp_v - 2); ++i) {
		fprintf(putin, "%d %d\n", i, i + 1);
		fprintf(putin, "%d %d\n", i, i + 2);
	}

	for (i = 0; i < temp_v; ++i) {
		fprintf(putin, "%c ", colors[i % CLR_NUM][0]);
	}
	fclose(putin);
	return 0;
}

/*
 * Fetch input data from a file.
 */

int get_graph_parameters() {
    char c, dump;
    int i, a, b, choice;
    FILE *putin;

	do {
		printf("1. Use predefined graph\n");
		printf("2. Use randomly generated graph\n");
		printf("==========\n");
		printf("\033[36mYour choice:\033[0m ");
		scanf("%d%*c", &choice);
		printf("==========\n");
		if (choice < 1 || choice > 2) {
			terrible_error_137();
		}
	} while (choice < 1 || choice > 2);

	if (--choice) {
		set_graph_parameters();
		putin = fopen("graph.in", "r");
	} else {
		putin = fopen("graph.in.pre", "r");
	}

    if (putin == NULL) {
        fprintf(putout, "\033[31mError opening file!\033[0m\n");
        return -1;
    }

    if (choice) {
		fprintf(putout, "Fetching data from 'graph.in'...\n==========\n");
	} else {
		fprintf(putout, "Fetching data from 'graph.in.pre'...\n==========\n");
	}

    fscanf(putin, "%d %d", &V, &E);
    fprintf(putout, "Vertices (V): %lli\nEdges (E): %lli\n==========\n", V, E);

    vertex_set = (struct vertex *)malloc(V * sizeof(struct vertex));
    edge_set = (struct edge *)malloc(E * sizeof(struct edge));

    for (i = 0; i < E; ++i) {
        fscanf(putin, "%d %d", &a, &b);
        edge_set[i].a = &vertex_set[a];
        edge_set[i].b = &vertex_set[b];
    }
    fscanf(putin, "%c", &dump);

    for (i = 0; i < V; ++i) {
        vertex_set[i].index = i;
        fscanf(putin, "%c ", &c);
        if (c == 'R') {
            vertex_set[i].color = 0;
        } else if (c == 'B') {
            vertex_set[i].color = 1;
        } else if (c == 'Y') {
            vertex_set[i].color = 2;
        }
    }
    fclose(putin);
    print_edge_set();
    print_vertex_set(0);

    if (choice) {
		fprintf(putout, "Fetching data from 'graph.in': OK\n==========\n");
	} else {
		fprintf(putout, "Fetching data from 'graph.in.pre': OK\n==========\n");
    }
    return 0;
}

/*
 * Set parameters to the vertices of a graph. Using simple functions
 * for shuffling the colors.
 */

void set_vertex_parameters() {
    int i, basket[CLR_NUM] = {0, 1, 2};
    long long c, d, n, r, z;

    shuffle_colors(basket);

    for (i = 0; i < V; ++i) {
        vertex_set[i].color = basket[vertex_set[i].color];
        get_rsa_parameters(&n, &c, &d);
        get_r(vertex_set[i].color, n, &r);
        get_z(r, d, n, &z);
        vertex_set[i].r = r;
        vertex_set[i].n = n;
        vertex_set[i].c = c;
        vertex_set[i].d = d;
        vertex_set[i].z = z;
    }
    //print_vertex_set(1);
}

void shuffle_colors(int *array) {
    int i, j;

    for (i = CLR_NUM - 1; i > 0; --i) {
        j = rand() % (i + 1);
        swap(&array[i], &array[j]);
    }
}

void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

/*
 * These guys print the current vertices set state. And edges. Yep.
 */

void print_vertex_set(int mode) {
    int i;

    fprintf(putout, "VERTICES SET:\n");

    for (i = 0; i < V; ++i) {
        fprintf(putout, "Index:\t%d\n", vertex_set[i].index);
        fprintf(putout, "Color:\t%s\n", colors[vertex_set[i].color]);

        if (mode) {
            fprintf(putout, "R:\t%lli\n", vertex_set[i].r);
            fprintf(putout, "N:\t%lli\n", vertex_set[i].n);
            fprintf(putout, "C:\t%lli\n", vertex_set[i].c);
            fprintf(putout, "D:\t%lli\n", vertex_set[i].d);
            fprintf(putout, "Z:\t%lli\n", vertex_set[i].z);
        }
        fprintf(putout, "==========\n");
    }
}

void print_edge_set() {
    int i;

    fprintf(putout, "EDGES SET:\n");

    for (i = 0; i < E; ++i) {
        fprintf(putout, "Edge %d:\t%d <-> %d\n", i, edge_set[i].a->index,
                edge_set[i].b->index);
    }
    fprintf(putout, "==========\n");
}

/*
 * So, Bob does this stuff.
 */

int bob_routine() {
    int x = rand() % E;
    long long col1, col2;

    fprintf(bob_says, "[BOB SAYS] I choose edge %d (%d <-> %d).\n", x,
            edge_set[x].a->index, edge_set[x].b->index);
    col1 = pow_mod(edge_set[x].a->z, edge_set[x].a->c, edge_set[x].a->n) & 3;
    col2 = pow_mod(edge_set[x].b->z, edge_set[x].b->c, edge_set[x].b->n) & 3;
    fprintf(bob_says, "[BOB SAYS] Vertex A is '%s'\n", colors[col1]);
    fprintf(bob_says, "[BOB SAYS] Vertex B is '%s'\n", colors[col2]);

    if (col1 != col2) {
        fprintf(bob_says, "[BOB SAYS] Yep.\n==========\n");
        return 1;
    } else {
        fprintf(bob_says, "[BOB SAYS] Nope.\n==========\n");
        print_vertex_set(1);
        return 0;
    }
}

/*
 * Yep, this again.
 */

void terrible_error_137() {
    printf("\033[31mError code 137: Are you retarded? Try again!\033[0m\n");
}

int main() {
    int i;

    srand((unsigned) time(NULL));
    printf("==========\n\033[35m'ZERO-KNOWLEDGE PROOF' SIMULATION\033[0m\n==========\n");
    putout = fopen("graph.log", "w");
    fprintf(putout, "'ZERO-KNOWLEDGE PROOF' LOG FILE\n==========\n");
    get_graph_parameters();
    fprintf(putout, "LAUNCHING THE ALGORITHM\n==========\n");
    bob_says = fopen("graph_bob.log", "w");

    for (i = 0; i < A * E; ++i) {
        set_vertex_parameters();
        if (!bob_routine()) {
            fprintf(putout, "Failed at test %d.\n==========\n", i + 1);
            break;
        }
    }
    fclose(bob_says);

    if (i == A * E) {
        fprintf(putout, "All %d tests are passed!\n==========\n", A * E);
    }
    free(vertex_set);
    free(edge_set);
    fprintf(putout, "Cleanup complete! Ending my routine.\n");
    printf("\033[32mDone!\nResults:\t'\033[35mgraph.log\033[32m'\nBob's log:\t'\033[36mgraph_bob.log\033[32m'\033[0m\n");
    fclose(putout);
    return 0;
}
