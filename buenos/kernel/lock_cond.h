#include "kernel/spinlock.h"

#ifndef LOCKCOND
#define LOCKCOND

typedef struct {
	int locked;
	spinlock_t spinlock;
} lock_t;

int lock_reset(lock_t *lock);
void lock_acquire(lock_t *lock);
void lock_release(lock_t *lock);

typedef struct {
} cond_t;

void condition_init(cond_t *cond);
void condition_wait(cond_t *cond, lock_t *lock);
void condition_signal(cond_t *cond);
void condition_broadcast(cond_t *cond);

#endif
