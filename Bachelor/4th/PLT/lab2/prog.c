#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFF_SIZE 256
#define MAX_LENGTH 32

#define HASH_MIN '0' + '0' + '0'
#define HASH_MAX 'z' + 'z' + 'z'

int num = 0;
int qwe[30];

struct node {
	char *name;
	char *type;
	char *value;

	struct node *next;
};
struct node *table[HASH_MAX];


int hash(char *str)
{
	return str[0] + (str[strlen(str) / 2]) + (str[strlen(str) - 1]);
}


int rehash(char *str, int step)
{
	return (hash(str) * step) % HASH_MAX;
}


void table_init(struct node **table)
{
	for (int i = 0; i < HASH_MAX; ++i) table[i] = NULL;
}


void table_add(struct node **table, char *n, char *t, char *v)
{
	struct node *node;

	int step = 0;
	int idx = hash(n);

	if (table[idx] == NULL) {
		if ((node = malloc(sizeof(*node))) != NULL) {
			node->name = n;
			node->type = t;
			node->value = v;
			node->next = table[idx];
			table[idx] = node;

			printf("'%s [%s, %s]' added to {%d}.\n", node->name, node->type, node->value, idx);
		}
		else printf("Allocation error.\n");
	}
	else {
		step = 1;

		while (1) {
			idx = rehash(n, step);

			if (table[idx] == NULL) {
				if ((node = malloc(sizeof(*node))) != NULL) {
					node->name = n;
					node->type = t;
					node->value = v;
					node->next = table[idx];
					table[idx] = node;

					printf("'%s [%s, %s]' added to {%d}.\n", node->name, node->type, node->value, idx);

					break;
				}
				else printf("Allocation error.\n");
			}
			else {
				if (rehash(n, 0) == rehash(n, step)) {
					printf("Insertion error.\n");
					break;
				}
				else ++step;
			}
		}
	}
}


int chain_table_add(struct node **table, char *n, char *t, char *v)
{
	struct node *node;
	int idx = hash(n);
	qwe[num] = idx; ++num;

	if ((node = malloc(sizeof(*node))) == NULL) {
		printf("Allocation error.\n");
		return -1;
	}

	if (table[idx] == NULL) {
		node->name = n;
		node->type = t;
		node->value = v;
		node->next = table[idx];
		table[idx] = node;

		printf("'%s [%s, %s]' added to {%d}.\n", node->name, node->type, node->value, idx);
	}
	else {
		node = table[idx];
		while (node->next != NULL) node = node->next;
		
		node->next = malloc(sizeof(*node));
		node->next->name = n;
		node->next->type = t;
		node->next->value = v;

		printf("'%s [%s, %s]' added to {%d}.\n", node->next->name, node->next->type, node->next->value, idx);
	}
	return 0;
}


struct node *table_search(struct node **table, char *key)
{
	struct node *node;
	int step = 0;
	int idx = hash(key);

	printf(">>> key2search: %s, {%d}\n", key, idx);

	node = table[idx];

	if (table[idx] == NULL) {
		printf("Key not found.\n");
		return NULL;
	}
	else if (!(strcmp(node->name, key))) {
		printf("Key found.\n");
		return node;
	}
	else {
		step = 1;

		while (1) {
			idx = rehash(key, step);

			if (((node == NULL) || (rehash(key, 0) == rehash(key, step)))) {
				printf("Key not found.\n");
				return NULL;
			}
			else if (!(strcmp(node->name, key))) {
				printf("Key found.\n");
				return node;
			}
			else ++step;
		}
	}
}


struct node *chain_table_search(struct node **table, char *key)
{
	struct node *node;
	int idx = hash(key);

	printf(">>> key2search: %s, {%d}\n", key, idx);

	node = table[idx];

	if (node == NULL) {
		printf("Key not foundA.\n");
		return NULL;
	}
	else if (!(strcmp(node->name, key))) {
		printf("Key found.\n");
		return node;
	}
	else {
		node = node->next;
		while (1) {
			if (node == NULL) {
				printf("Key not foundB.\n");
				return NULL;
			}
			else if (!(strcmp(node->name, key))) {
				printf("Key found.\n");
				return node;
			}
			else node = node->next;
		}
	}

}


int main()
{
	int i = 0, j = 0, flen = 0, line = 1;
	char *nbuf, *tbuf, *vbuf;
	char ch;

	struct node *res;

	FILE *fin;

	if (!(fin = fopen("table.in", "r"))) {
		printf("File doesn't exist.\n");
		return -1;
	}

	table_init(table);

	fseek(fin, 0L, SEEK_END);
	flen = ftell(fin);
	printf("flen: %d\n", flen);
	rewind(fin);

	while (j < flen - 1) {
		nbuf = malloc (sizeof(char) * BUFF_SIZE);
		for (i = 0; i != BUFF_SIZE; ++i) {
			ch = fgetc(fin); ++j;
			if (ch == ' ') break;
			nbuf[i] = ch;
		}
		nbuf[i] = '\0';
		printf("name: %s\n", nbuf);

		if (i > MAX_LENGTH) {
			printf("The identifier is too long (line %d).\n", line);
			++line;
			continue;
		}

		tbuf = malloc (sizeof(char) * BUFF_SIZE);
		for (i = 0; i != BUFF_SIZE; ++i) {
			ch = fgetc(fin); ++j;
			if (ch == ' ') break;
			tbuf[i] = ch;
		}
		tbuf[i] = '\0';
		printf("type: %s\n", tbuf);

		if (i > MAX_LENGTH) {
			printf("Type of the identifier is too long (line %d).\n", line);
			++line;
			continue;
		}

		vbuf = malloc (sizeof(char) * BUFF_SIZE);
		for (i = 0; i != BUFF_SIZE; ++i) {
			ch = fgetc(fin); ++j;
			if (ch == '\n') break;
			vbuf[i] = ch;
		}
		vbuf[i] = '\0';
		printf("val: %s\n", vbuf);

		if (i > MAX_LENGTH) {
			printf("Value of the identifier is too long (line %d).\n", line);
			++line;
			continue;
		}

		chain_table_add(table, nbuf, tbuf, vbuf);

		//printf("{%d} -> ", qwe[num - 1]);
		//res = table[qwe[num - 1]];
		//while (res) { printf("(%s) ", res->name); res = res->next; }
		//puts("\n");
		++line;
	}

	printf("\n***************\n\n");

	//res = table[315];
	//while (1) { printf("(%s) ", res->name); res = res->next; }

	res = chain_table_search(table, "oleg");
	if (res != NULL) printf("Node: %s \t|\t %s \t|\t %s\n", res->name, res->type, res->value);

	res = chain_table_search(table, "qweq");
	if (res != NULL) printf("Node: %s \t|\t %s \t|\t %s\n", res->name, res->type, res->value);

	res = chain_table_search(table, "oreg");
	if (res != NULL) printf("Node: %s \t|\t %s \t|\t %s\n", res->name, res->type, res->value);

	res = chain_table_search(table, "dbl");
	if (res != NULL) printf("Node: %s \t|\t %s \t|\t %s\n", res->name, res->type, res->value);

	res = chain_table_search(table, "oleg_petuh");
	if (res != NULL) printf("Node: %s \t|\t %s \t|\t %s\n", res->name, res->type, res->value);

	fclose(fin);

//	free(nbuf);
//	free(tbuf);
//	free(vbuf);

	return 0;
}
