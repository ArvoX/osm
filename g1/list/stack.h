#ifndef STACK_H
#define STACK_H
#include "list.h"

typedef struct stack_type {
  Listnode *list;
} stack_type;

/* Initialise a freshly allocated stack.  Must be called before using
   any of the other stack functions. */
void stack_init(stack_type*);

/* Returns true if the stack is empty. */
int stack_empty(stack_type*);

/* Return the top element of the stack.  Undefined behaviour if the
   stack is empty. */
void* stack_top(stack_type*);

/* Remove the top element of the stack and return it.  Undefined
   behaviour if the stack is empty. */
void* stack_pop(stack_type*);

/* Push an element to the top of the stack.  Returns 0 if possible,
   any other value if there was an error (for example, if the stack is
   full or no memory could be allocated). */
int stack_push(stack_type*, void*);


#endif
