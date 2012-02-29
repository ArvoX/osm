/*
 * Lock
 *
 */


#include "kernel/lock.h"


/** Initialize an already allocated lock_t structure.*/
int lock_reset(lock_t *lock){
	lock.locked = 0;
}

/** Aquire the lock.*/
void lock_acquire(lock_t *lock){
	
	
	/* We have to disable interrupts for this thread to avoid a situation where the
	 thread is added to the sleep queue and interupted before the spinlock is
	 released. */	
    interrupt_status_t intr_status;	
    intr_status = _interrupt_disable();	
    spinlock_acquire(lock);
	
	while (lock.locked) {				
		sleepq_add(lock);		
		spinlock_release(lock);
        thread_switch();
		spinlock_acquire(lock);
	}

	lock->locked = 1;	
	spinlock_release(lock);
    _interrupt_set_state(intr_status);		
}

/** Release the lock*/
void lock_release(lock_t *lock){
	
	/* Disable interrups while aquring spinlock*/	
    interrupt_status_t intr_status;
	intr_status = _interrupt_disable();
    spinlock_acquire(lock);
	/* Unlocking*/
	lock->locked = 0;	
	spinlock_release(lock);
    _interrupt_set_state(intr_status);
	
}