/*
 * Implementation of conditional sleeping queue.
 *
 */


#include "kernel/lock.h"
#include "kernel/condition.h"
#include "kernel/sleepq.h"


void condition_init(cond_t *cond){
	
}
void condition_wait(cond_t *cond, lock_t *lock){
		
	sleepq_add(cond);
	lock_release(lock);
	thread_switch();
	lock_acquire(lock);
	
}
void condition_signal(cond_t *cond){		
    sleepq_wake(cond);	
}
void condition_broadcast(cond_t *cond){
	
}
