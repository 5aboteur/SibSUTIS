#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define T 100000

struct node {
	struct node *next;
	struct node *prev;

	int counter;
};

struct queue {
	struct node *head;
	struct node *tail;
};

struct queue *_initq()
{
	struct queue *q =
		(struct queue *)malloc(sizeof(struct queue));
	q->head = q->tail = NULL;
	return (q);
}

struct queue *_push(struct queue *q, int c)
{
	struct node *nd =
		(struct node *)malloc(sizeof(struct node));

	if (q->head == NULL) {
		nd->next = nd->prev = NULL;
		q->head = nd;
		q->tail = nd;
		nd->counter = c;
		return (q);
	}

	nd->next = NULL;
	nd->prev = q->tail->next;
	nd->counter = c;
	q->tail->next = nd;
	q->tail = nd;
	return (q);
}

struct queue *_pop(struct queue *q)
{
	q->tail->next = NULL;
	return (q);
}

void _free(struct queue *q)
{
	free(q);
}

int main()
{
	srand48((unsigned)time(NULL));

	int i = 0, cnt_rd = 0, cnt_wr = 0, cnt_proc = 0;
	double p_rd = 0.05, p_wr = 0.005;

	struct queue *q;

	q = _initq();

	for (i = 0; i < T; i++) {
		double r_rd = drand48();
		double r_wr = drand48();

		if (r_rd < 0.05) {
			_push(q, 10);
			cnt_rd++;
		}

		if (r_wr < 0.005) {
			_push(q, 10);
			cnt_wr++;
		}

		if (q->tail) {
			q->tail->counter--;
			if (!(q->tail->counter)) {
				q =	_pop(q);
				cnt_proc++;
			}
		}
	}

	printf("[RD]: %d\n[WR]: %d\n[RD+WR]: %d\n\n[PROC]: %d\n",
		cnt_rd, cnt_wr, cnt_rd + cnt_wr, cnt_proc);

	_free(q);
	return 0;
}
