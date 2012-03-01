/*
 * Implementation of conditional sleeping queue.
 *
 */


#include "kernel/lock.h"
#include "kernel/lock_cond.h"
#include "kernel/sleepq.h"
#include "kernel/interrupt.h"
#include "kernel/thread.h"


void condition_init(cond_t *cond){
	/* For at undgå unused*/
	cond = cond;
	/* Vi kan ikke se hvad funktionen skal bruges til*/

}

void condition_wait(cond_t *cond, lock_t *lock){
	
	
    interrupt_status_t intr_status = _interrupt_disable();

	sleepq_add(cond);
	lock_release(lock);	
	thread_switch();
	
    _interrupt_set_state(intr_status);
	
	lock_acquire(lock);
	
}
void condition_signal(cond_t *cond){		
    sleepq_wake(cond);	
}
void condition_broadcast(cond_t *cond){
    sleepq_wake_all(cond);
}
