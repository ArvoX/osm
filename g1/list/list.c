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

void reverse(Listnode **start){
    Listnode *new, *cur, *next;
    new = NULL;
    cur = *start;
    while(cur != NULL){
        next = cur->next;
        cur->next = new;
        new = cur;
        cur = next;
    }
    *start = new;
}

Data pop(Listnode **start){
    Listnode *node = *start;
    Data data = head(node);
    *start = node->next;
    free(node);
    return data;
}
