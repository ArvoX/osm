#ifndef LIST_H
#define LIST_H

typedef void* Data;
typedef struct listnode {
    Data content;
    struct listnode* next;
} Listnode;

/* add an element at the end of the list */
void append(Listnode **start, Data elem);

/* add an element at the front */
void prepend(Listnode **start, Data elem);

int length(Listnode *start); // length

/* return first element (if not empty) */
Data head(Listnode *start);

#endif

