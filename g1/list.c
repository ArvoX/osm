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
