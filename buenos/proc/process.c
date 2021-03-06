
/*
 * Process startup.
 *
 * Copyright (C) 2003-2005 Juha Aatrokoski, Timo Lilja,
 *       Leena Salmela, Teemu Takanen, Aleksi Virtanen.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above
 *    copyright notice, this list of conditions and the following
 *    disclaimer in the documentation and/or other materials provided
 *    with the distribution.
 * 3. The name of the author may not be used to endorse or promote
 *    products derived from this software without specific prior
 *    written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * $Id: process.c,v 1.11 2007/03/07 18:12:00 ttakanen Exp $
 *
 */

#include "proc/process.h"
#include "proc/elf.h"
#include "kernel/thread.h"
#include "kernel/assert.h"
#include "kernel/interrupt.h"
#include "kernel/config.h"
#include "fs/vfs.h"
#include "drivers/yams.h"
#include "vm/vm.h"
#include "vm/pagepool.h"

#include "kernel/sleepq.h"
#include "lib/debug.h"

/** @name Process startup
 *
 * This module contains a function to start a userland process.
 */

spinlock_t proc_table_slock;

process_t proc_table[USER_PROC_LIMIT];

void clear_proc_table_entry(process_id_t pid);

/**
 * Starts one userland process. The thread calling this function will
 * be used to run the process and will therefore never return from
 * this function. This function asserts that no errors occur in
 * process startup (the executable file exists and is a valid ecoff
 * file, enough memory is available, file operations succeed...).
 * Therefore this function is not suitable to allow startup of
 * arbitrary processes.
 *
 * @executable The name of the executable to be run in the userland
 * process
 */
void process_start(uint32_t pid)
{
    const char *executable;
    thread_table_t *my_entry;
    pagetable_t *pagetable;
    uint32_t phys_page;
    context_t user_context;
    uint32_t stack_bottom;
    elf_info_t elf;
    openfile_t file;

    int i;
    
    DEBUG("debugsyscall","process_start - initial \n");
   
    interrupt_status_t intr_status;

    intr_status = _interrupt_disable();
    spinlock_acquire(&proc_table_slock);
    executable = proc_table[pid].executable;
    spinlock_release(&proc_table_slock);
    _interrupt_set_state(intr_status);

    my_entry = thread_get_current_thread_entry();
    
    /* If the pagetable of this thread is not NULL, we are trying to
       run a userland process for a second time in the same thread.
       This is not possible. */
    KERNEL_ASSERT(my_entry->pagetable == NULL);
    
    
    my_entry->process_id = pid;
        
    
    pagetable = vm_create_pagetable(thread_get_current_thread());
    KERNEL_ASSERT(pagetable != NULL);

    intr_status = _interrupt_disable();
    my_entry->pagetable = pagetable;
    _interrupt_set_state(intr_status);

    file = vfs_open((char *)executable);
 
	/* Make sure the file existed and was a valid ELF file */
    KERNEL_ASSERT(file >= 0);
    KERNEL_ASSERT(elf_parse_header(&elf, file));	
	
	/* Saving the program file in the proces file tabel.
	   We use spinlock because it is possible the parent is dead 
	   and this process will be an orphan. */
	intr_status = _interrupt_disable();
    spinlock_acquire(&proc_table_slock);
	proc_table[pid].files[0] = file;	
    spinlock_release(&proc_table_slock);
    _interrupt_set_state(intr_status);	
	
    /* Trivial and naive sanity check for entry point: */
    KERNEL_ASSERT(elf.entry_point >= PAGE_SIZE);

    /* Calculate the number of pages needed by the whole process
       (including userland stack). Since we don't have proper tlb
       handling code, all these pages must fit into TLB. */
    KERNEL_ASSERT(elf.ro_pages + elf.rw_pages + CONFIG_USERLAND_STACK_SIZE
          <= _tlb_get_maxindex() + 1);

    /* Allocate and map stack */
    for(i = 0; i < CONFIG_USERLAND_STACK_SIZE; i++) {
        phys_page = pagepool_get_phys_page();
        KERNEL_ASSERT(phys_page != 0);
        vm_map(my_entry->pagetable, phys_page, 
               (USERLAND_STACK_TOP & PAGE_SIZE_MASK) - i*PAGE_SIZE, 1);
    }

    /* Allocate and map pages for the segments. We assume that
       segments begin at page boundary. (The linker script in tests
       directory creates this kind of segments) */
    for(i = 0; i < (int)elf.ro_pages; i++) {
        phys_page = pagepool_get_phys_page();
        KERNEL_ASSERT(phys_page != 0);
        vm_map(my_entry->pagetable, phys_page, 
               elf.ro_vaddr + i*PAGE_SIZE, 1);
    }

    for(i = 0; i < (int)elf.rw_pages; i++) {
        phys_page = pagepool_get_phys_page();
        KERNEL_ASSERT(phys_page != 0);
        vm_map(my_entry->pagetable, phys_page, 
               elf.rw_vaddr + i*PAGE_SIZE, 1);
    }

    /* Put the mapped pages into TLB. Here we again assume that the
       pages fit into the TLB. After writing proper TLB exception
       handling this call should be skipped. */
    intr_status = _interrupt_disable();
    tlb_fill(my_entry->pagetable);
    _interrupt_set_state(intr_status);
    
    /* Now we may use the virtual addresses of the segments. */

    /* Zero the pages. */
    memoryset((void *)elf.ro_vaddr, 0, elf.ro_pages*PAGE_SIZE);
    memoryset((void *)elf.rw_vaddr, 0, elf.rw_pages*PAGE_SIZE);

    stack_bottom = (USERLAND_STACK_TOP & PAGE_SIZE_MASK) - 
        (CONFIG_USERLAND_STACK_SIZE-1)*PAGE_SIZE;
    memoryset((void *)stack_bottom, 0, CONFIG_USERLAND_STACK_SIZE*PAGE_SIZE);

    /* Copy segments */

    if (elf.ro_size > 0) {
    /* Make sure that the segment is in proper place. */
        KERNEL_ASSERT(elf.ro_vaddr >= PAGE_SIZE);
        KERNEL_ASSERT(vfs_seek(file, elf.ro_location) == VFS_OK);
        KERNEL_ASSERT(vfs_read(file, (void *)elf.ro_vaddr, elf.ro_size)
              == (int)elf.ro_size);
    }

    if (elf.rw_size > 0) {
    /* Make sure that the segment is in proper place. */
        KERNEL_ASSERT(elf.rw_vaddr >= PAGE_SIZE);
        KERNEL_ASSERT(vfs_seek(file, elf.rw_location) == VFS_OK);
        KERNEL_ASSERT(vfs_read(file, (void *)elf.rw_vaddr, elf.rw_size)
              == (int)elf.rw_size);
    }


    /* Set the dirty bit to zero (read-only) on read-only pages. */
    for(i = 0; i < (int)elf.ro_pages; i++) {
        vm_set_dirty(my_entry->pagetable, elf.ro_vaddr + i*PAGE_SIZE, 0);
    }

    /* Insert page mappings again to TLB to take read-only bits into use */
    intr_status = _interrupt_disable();
    tlb_fill(my_entry->pagetable);
    _interrupt_set_state(intr_status);

    /* Initialize the user context. (Status register is handled by
       thread_goto_userland) */
    memoryset(&user_context, 0, sizeof(user_context));
    user_context.cpu_regs[MIPS_REGISTER_SP] = USERLAND_STACK_TOP;
    user_context.pc = elf.entry_point;

    thread_goto_userland(&user_context);

    KERNEL_PANIC("thread_goto_userland failed.");
}

/* Run process in new thread, returns PID of new process */
process_id_t process_spawn(const char *executable) {    
    interrupt_status_t intr_status;
    process_id_t pid = -1;
    
    // Acquiring the spinlock of the process table.
    intr_status = _interrupt_disable();
    spinlock_acquire(&proc_table_slock);    
    
    // Find an empty entrance in the process tabel.
    int i;    
    for(i = 0; i < USER_PROC_LIMIT; i++) {
        if (proc_table[i].state == PROC_FREE) {
            pid = i;
            break;
        }
    }
    KERNEL_ASSERT(pid > -1);

    // Initialize the process
    proc_table[pid].state = PROC_RUNNING;
    
    for(i = 0; i < PROC_EXEC_NAME_MAX - 1 && executable[i] != '\0'; i++)
        proc_table[pid].executable[i] = executable[i];
    proc_table[pid].executable[++i] = '\0';

    // Creating new thread. The new thread is going to run process_start()
    TID_t current_thread = thread_create(&process_start, pid);
    
    // Vi skal evt. tjekke om current_thread == -1.
    thread_run(current_thread);
    
    spinlock_release(&proc_table_slock);
    _interrupt_set_state(intr_status);
    return pid;
}

/* Run process in this thread , only returns if there is an error */
int process_run( const char *executable ){
    interrupt_status_t intr_status;
    process_id_t pid = -1;
    
    DEBUG("debugsyscall","process_run - initial \n");
    // Acquiring the spinlock of the process table.
    intr_status = _interrupt_disable();
    spinlock_acquire(&proc_table_slock);
    
    // Find an empty entrance in the process tabel.
    int i;    
    for(i = 0; i < USER_PROC_LIMIT; i++) {
        if (proc_table[i].state == PROC_FREE) {
            pid = i;
            break; 
         }
    }
    KERNEL_ASSERT(pid > -1);
    // Initialize the process

    proc_table[pid].state = PROC_RUNNING;
    for(i = 0; i < PROC_EXEC_NAME_MAX - 1 && executable[i] != '\0'; i++)
        proc_table[pid].executable[i] = executable[i];
    proc_table[pid].executable[++i] = '\0';
    
    spinlock_release(&proc_table_slock);
    _interrupt_set_state(intr_status);
    
    DEBUG("debugsyscall","process_run - pid %d\n",pid);

    // Starting the process. This function should never return.
    process_start(pid);
    
    return -1;
}

process_id_t process_get_current_process(void) {        
    return thread_get_current_thread_entry()->process_id;        
} 

/* Stop the current process and the kernel thread in which it runs */
void process_finish(int retval) {  
    interrupt_status_t intr_status;
    
    DEBUG("debugsyscall","process_finish - initial \n");
    
    thread_table_t *my_entry;
    process_id_t pid;
    
    my_entry = thread_get_current_thread_entry();
    pid = my_entry->process_id;
    
    intr_status = _interrupt_disable();
    spinlock_acquire(&proc_table_slock);

	/* Closing all open files in this process */
	int i;
	for (i=0; i<PROC_MAX_OPEN_FILES; i++){
		if  (proc_table[pid].files[i] > -1){
			vfs_close(proc_table[pid].files[i]);			
		}
	}
	
    process_id_t child = proc_table[pid].first_child;
    
    if(proc_table[pid].orphan)
        clear_proc_table_entry(pid);
    else
    {
        proc_table[pid].state = PROC_ZOMBIE;    
        proc_table[pid].retval = retval;
        proc_table[pid].first_child = -1;
     }

    while(child != -1)
    {
        process_id_t sibling = proc_table[child].sibling;
        if(proc_table[child].state == PROC_ZOMBIE)
            clear_proc_table_entry(child);
        else
           proc_table[child].orphan = 1;
        child = sibling;
     }
    
    sleepq_wake(&proc_table[pid]);

    spinlock_release(&proc_table_slock);
    _interrupt_set_state(intr_status);
    
    vm_destroy_pagetable(my_entry->pagetable);
    my_entry->pagetable = NULL;
    
    thread_finish();
}
/* Wait for the given process to terminate , returning its return value,
 * a nd marking the process table entry as free */
uint32_t process_join(process_id_t pid)
{    
    uint32_t retval;
    
    DEBUG("debugsyscall","process_join - initial \n");

    interrupt_status_t intr_status;
    DEBUG("debugsyscall","t:%d. disable interrupt...",thread_get_current_thread());


/* We have to disable interrupts for this thread to avoid a situation where the
thread is added to the sleep queue and interupted before the spinlock is
released. */
    intr_status = _interrupt_disable();
    DEBUG("debugsyscall","done. status: %d\n",(int)intr_status);
    DEBUG("debugsyscall","acquiring spinlock...");
    spinlock_acquire(&proc_table_slock);
    DEBUG("debugsyscall","done\n");


    while (proc_table[pid].state != PROC_ZOMBIE) {
        sleepq_add(&proc_table[pid]);
        
        DEBUG("debugsyscall","process_join - while loop. ");

        spinlock_release(&proc_table_slock);

        DEBUG("debugsyscall","process_join - spinlock released. switch\n");

        thread_switch();
        
        DEBUG("debugsyscall","process_join - waked\n");

        spinlock_acquire(&proc_table_slock);
    }
    
    DEBUG("debugsyscall","proc state == PROC_ZOMBIE\n");

    retval = (uint32_t)proc_table[pid].retval;
    // Resetting the entrance in the process tabel.
    clear_proc_table_entry(pid);
    
    spinlock_release(&proc_table_slock);
    _interrupt_set_state(intr_status);
    
    return retval;
}

/* Initialize process table. Should be called before any other process-related calls */
void process_init ( void ) {
    
    spinlock_reset(&proc_table_slock);
    int i;
    for(i = 0; i<USER_PROC_LIMIT; i++){
        clear_proc_table_entry(i);
    } 

}

// clears an proc_table entry, lock must be acquired before called
void clear_proc_table_entry(process_id_t pid) { 
    proc_table[pid].state       = PROC_FREE;
    proc_table[pid].retval      = 0;
    proc_table[pid].first_child = -1;
    proc_table[pid].sibling     = -1;
    proc_table[pid].orphan      = 0;
	
    int i;
    for(i = 0; i < PROC_EXEC_NAME_MAX; i++)
        proc_table[pid].executable[i] = '\0';
    for(i = 0; i < PROC_MAX_OPEN_FILES; i++)
        proc_table[pid].files[i] = -1;
}
