#ifndef STACK_H
#define STACK_H

#define STACK_MAX_SIZE 100

typedef struct stack_t {
  void* data[STACK_MAX_SIZE];
  int top;
} stack_t;

/* Initialise a freshly allocated stack.  Must be called before using
   any of the other stack functions. */
void stack_init(stack_t*);

/* Returns true if the stack is empty. */
int stack_empty(stack_t*);

/* Return the top element of the stack.  Undefined behaviour if the
   stack is empty. */
void* stack_top(stack_t*);

/* Remove the top element of the stack and return it.  Undefined
   behaviour if the stack is empty. */
void* stack_pop(stack_t*);

/* Push an element to the top of the stack.  Returns 0 if possible,
   any other value if there was an error (for example, if the stack is
   full or no memory could be allocated). */
int stack_push(stack_t*, void*);


#endif
