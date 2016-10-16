/*
 * stack_llist.c: Linked-list based stack implementation.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include "llist.h"

struct stack {
	struct listnode *top;
	int size;
};

/* stack_create: Creates an empty stack. */
struct stack *stack_create() 
{
	struct stack *s = malloc(sizeof(*s));
	if (s != NULL) {
		s->size = 0;
		s->top = NULL;
	}
	return s;
}

/* stack_free: Removes stack. */
void stack_free(struct stack *s) 
{
	while (s->size > 0)
		stack_pop(s);
	free(s);
}

/* stack_size: Returns size of a stack. */
int stack_size(struct stack *s) 
{
	return s->size;
}

/* stack_push: Pushes item to the stack. */
int stack_push(struct stack *s, int value)
{
	s->top = list_addfront(s->top, value);
	if (s->top == NULL) {
		fprintf(stderr, "stack: Stack overflow\n");
		return -1;
	}
	s->size++;
	return 0;
}

/* stack_pop: Pops item from the stack. */
int stack_pop(struct stack *s)
{
	struct listnode *next;
	int value;
	if (s->top == NULL) {
		fprintf(stderr, "stack: Stack underflow\n");
		return -1;
	}
	next = s->top->next;
	value = s->top->value;
	free(s->top);
	s->top = next;
	s->size--;
	return value;
}

int main()
{
	struct stack *s;
	int i, val;
	s = stack_create();
	for (i = 1; i <= 8; i++) {
		stack_push(s, i);
	}
	for (i = 1; i <= 4; i++) {
		val = stack_pop(s);
		printf("pop: %d\n", val);
	}
	stack_free(s);
	getchar();
	return 0;
}
