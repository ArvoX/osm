#include <stdlib.h>
#include "list.h"

int length(Listnode *start){
    int len = 0;
    while (start != NULL) {
        len++;
        start = start->next;
    }
    return len;
}

Data head(Listnode *start){
    if (start != NULL){
        return start->content;
    } else {
        return NULL;
    }
}


void append(Listnode **start, Data elem){
    Listnode *new, *current;
    new = (Listnode*)malloc(sizeof(Listnode));
    
    new->content = elem;
    new->next = NULL;

    current = *start;
    
    if (current != NULL){
        
        while (current->next != NULL) {
            current = current->next;
        }        
        current->next = new;
    }
    else
        *start = new;
}

void prepend(Listnode **start, Data elem){
    Listnode *new = (Listnode*)malloc(sizeof(Listnode));
    
    new->content = elem;
    new->next = *start;

    *start = new;
}

void remv(Listnode **start, int (*match)(Data)){
    Listnode *curr, **last;
    if (*start == NULL)
        return;
    last = start;
    curr = *start;
    while (!(match(curr->content)) && curr!=NULL ) {
        last = &(curr->next);
        curr = curr->next;
    }
    if (curr == NULL)
        return; /* not found, do nothing */
    *last = curr->next; /* found, remove and free */
    free(curr);
}
