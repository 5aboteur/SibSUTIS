/*
 * llist.c: Singly linked list implementation.
 *
 */
#include <stdio.h>
#include <stdlib.h>

#include "llist.h"

struct listnode *list_createnode(int value)
{
    struct listnode *p;
    
    p = malloc(sizeof(*p));
    if (p != NULL) {
        p->value = value;
        p->next = NULL;
    }
    return p;
}

struct listnode *list_addfront(struct listnode *list, int value)
{
    struct listnode *newnode = list_createnode(value);
    
    if (newnode != NULL) {
        newnode->next = list;
        return newnode;
    } 
    return list;
}

struct listnode *list_lookup(struct listnode *list, int value)
{
    for (; list != NULL; list = list->next) {
        if (list->value == value) {
            return list;
        }
    }
    return NULL;
}

struct listnode *list_delete(struct listnode *list, int value)
{
    struct listnode *p, *prev = NULL;
    
    for (p = list; p != NULL; p = p->next) {
        if (p->value == value) {
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

void list_deleteall(struct listnode *list)
{
    struct listnode *p;
    
    while (list != NULL) {
        p = list;
        list = list->next;
        free(p);
    }
}
