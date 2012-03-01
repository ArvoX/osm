#include "kernel/lock_cond.h"
#include "kernel/interrupt.h"
#include "kernel/sleepq.h"
#include "kernel/spinlock.h"
#include "kernel/thread.h"

/*
 * Lock
 *
 */

/** Initialize an already allocated lock_t structure.*/
int lock_reset(lock_t *lock){
	lock->locked = 0;
	return 0;
}

/** Aquire the lock.*/
void lock_acquire(lock_t *lock){
	
	
	/* We have to disable interrupts for this thread to avoid a situation where the
	 thread is added to the sleep queue and interupted before the spinlock is
	 released. */	
	interrupt_status_t intr_status;	
	intr_status = _interrupt_disable();	
	spinlock_acquire((spinlock_t*) lock);
	
	while (lock->locked) {				
		sleepq_add(lock);		
		spinlock_release((spinlock_t*)lock);
		thread_switch();
		spinlock_acquire((spinlock_t*)lock);
	}

	lock->locked = 1;	
	spinlock_release((spinlock_t*)lock);
	_interrupt_set_state(intr_status);		
}

/** Release the lock*/
void lock_release(lock_t *lock){
	
	/* Disable interrups while aquring spinlock*/	
	interrupt_status_t intr_status;
	intr_status = _interrupt_disable();
	spinlock_acquire((spinlock_t*)lock);
	/* Unlocking*/
	lock->locked = 0;	
	spinlock_release((spinlock_t*)lock);
	_interrupt_set_state(intr_status);
	
}



/*
 * Implementation of conditional sleeping queue.
 *
 */
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
