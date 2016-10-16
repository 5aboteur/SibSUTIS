/*
 * llist.c: Singly linked list implementation.
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h> 

struct listnode {
    char *data;
    int value;
    
    struct listnode *next;
};

struct listnode *list_createnode(char *data, int value)
{
    struct listnode *p;
    
    p = malloc(sizeof(*p));
    if (p != NULL) {
        p->data = data;
        p->value = value;
        p->next = NULL;
    }
    return p;
}

struct listnode *list_addfront(struct listnode *list, char *data, int value)
{
    struct listnode *newnode = list_createnode(data, value);
    
    if (newnode != NULL) {
        newnode->next = list;
        return newnode;
    } 
    return list;
}

struct listnode *list_lookup(struct listnode *list, char *data, int value)
{
    for (; list != NULL; list = list->next) {
        if (strcmp(list->data, data) == 0 && list->value == value) {
            return list;
        }
    }
    return NULL;
}

struct listnode *list_delete(struct listnode *list, char *data, int value)
{
    struct listnode *p, *prev = NULL;
    
    for (p = list; p != NULL; p = p->next) {
        if (strcmp(p->data, data) == 0 && p->value == value) {
            if (prev == NULL) {
                list = p->next;
            } else {
                prev->next = p->next;
            }
            free(p);
            return list;
        }
        prev = p;
    }
    return NULL;
}

void list_print(struct listnode *list)
{
    for (; list != NULL; list = list->next) {
        printf("[%s, %d]\n", list->data, list->value);
    }
}

struct listnode *list_addend(struct listnode *list, char *data, int value)
{
	struct listnode *p, *newnode = list_createnode(data, value);
    
    if (newnode != NULL) {
        newnode->next = NULL;
        if (list == NULL) {
            list = newnode;
            return list;
        }
    	for (p = list; p->next != NULL; p = p->next);
    	p->next = newnode;
    	return list;
	}
    return NULL;
}

int main()
{
    struct listnode *list = NULL;
    struct listnode *p;
    int i;
    for (i = 0; i < 20; i++) {
        list = list_addend(list, "A", i+1);
    }
    list_print(list);
    p = list_delete(list, "A", 11);
    if (p != NULL) {
        list = p; 
        printf("Deleted!\n");
    } else {
        printf("Element is not found!\n");
    }
    list_print(list);
    //system("pause");
	return 0;
}
