#include <stdlib.h>
#include "stack.h"

int matchAll(Data data){
    return 1;
}

void stack_init(stack_type* stack) {
}

int stack_empty(stack_type* stack) {
    return length(stack->list) == 0;
}

void* stack_top(stack_type* stack) {    
    return head(stack->list);
}

void* stack_pop(stack_type* stack) {
    Data data = head(stack->list);
    rmv(stack->list,&matchAll)
    return data;
}

int stack_push(stack_type* stack, void* data) {
    prepend(stack->list,data);
}
