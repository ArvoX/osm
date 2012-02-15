#include <stdlib.h>
#include "stack.h"

int matchAll(unused Data data){
    return 1;
}

void stack_init(stack_t* stack) {
}

int stack_empty(stack_t* stack) {
    return length(stack->list) == 0;
}

void* stack_top(stack_t* stack) {    
    return head(stack->list);
}

void* stack_pop(stack_t* stack) {
    Data data = head(stack->list);
    rmv(stack->list,&matchAll)
    return data;
}

int stack_push(stack_t* stack, void* data) {
    prepend(stack->list,data);
}
