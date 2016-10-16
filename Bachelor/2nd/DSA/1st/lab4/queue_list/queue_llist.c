/*
 * queue_llist.c: Linked-list based queue implementation.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include "llist.h"

struct queue {
	struct listnode *head;
	struct listnode *tail;
	int size;
};

/* queue_create: Creates an empty queue. */
struct queue *queue_create()
{
	struct queue *q = malloc(sizeof(*q));
	if (q != NULL) {
		q->size = 0;
		q->head = NULL;
		q->tail = NULL;
	}
	return q;
}

/* queue_size: Returns size of a queue. */
int queue_size(struct queue *q)
{
	return q->size;
}

/* queue_enqueue: Add item to the queue. */
void queue_enqueue(struct queue *q, int value)
{
	struct listnode *oldtail = q->tail;
	/* Create new node */
	q->tail = list_createnode(value);
	if (q->head == NULL) {
	/* List is empty */
		q->head = q->tail;
	} else {
	/* Add new node to the end of list */
		oldtail->next = q->tail;
	}
	q->size++;
}

/* queue_dequeue: Gets item from the queue. */
int queue_dequeue(struct queue *q)
{
	int value;
	struct listnode *p;
	if (q->size == 0)
		return -1;
	/* Delete first node */
	value = q->head->value;
	p = q->head->next;
	free(q->head);
	q->head = p;
	q->size--;
	return value;
}

/* queue_free: Removes queue. */
void queue_free(struct queue *q)
{
	while (q->size > 0)
	queue_dequeue(q);
	free(q);
}

int main()
{
	int i, j;
	struct queue *q;

	q = queue_create();
	for (i = 1; i <= 10; i++) {
		queue_enqueue(q, i);
	}
	for (i = 1; i <= 5; i++) {
		j = queue_dequeue(q);
		printf("%d\n", j);
	}
	getchar();
    return 0;
}