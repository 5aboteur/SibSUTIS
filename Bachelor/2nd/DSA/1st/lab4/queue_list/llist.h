/*
 * llist.h: Singly linked list implementation.
 *
 */

#ifndef LIST_H
#define LIST_H 

struct listnode {
    int value;
    struct listnode *next;
};
        
struct listnode *list_createnode(int value);
struct listnode *list_addfront(struct listnode *list, int value);
struct listnode *list_lookup(struct listnode *list, int value);
struct listnode *list_delete(struct listnode *list, int value);
void list_deleteall(struct listnode *list);

#endif /* LIST_H */
