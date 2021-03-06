/*
 * System calls.
 *
 * Copyright (C) 2003 Juha Aatrokoski, Timo Lilja,
 *   Leena Salmela, Teemu Takanen, Aleksi Virtanen.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *	notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above
 *	copyright notice, this list of conditions and the following
 *	disclaimer in the documentation and/or other materials provided
 *	with the distribution.
 * 3. The name of the author may not be used to endorse or promote
 *	products derived from this software without specific prior
 *	written permission.
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
 * $Id: syscall.c,v 1.3 2004/01/13 11:10:05 ttakanen Exp $
 *
 */
#include "kernel/cswitch.h"
#include "proc/syscall.h"
#include "kernel/halt.h"
#include "kernel/panic.h"
#include "lib/libc.h"
#include "kernel/assert.h"
#include "proc/process.h"
#include "lib/debug.h"
#include "drivers/device.h"
#include "drivers/gcd.h"
#include "drivers/yams.h"
#include "fs/vfs.h"

void gettty();

#include "kernel/thread.h"

/**
 * Handle system calls. Interrupts are enabled when this function is
 * called.
 *
 * @param user_context The userland context (CPU registers as they
 * where when system call instruction was called in userland)
 */
void syscall_handle(context_t *user_context)
{
	/* When a syscall is executed in userland, register a0 contains
	 * the number of the syscall. Registers a1, a2 and a3 contain the
	 * arguments of the syscall. The userland code expects that after
	 * returning from the syscall instruction the return value of the
	 * syscall is found in register v0. Before entering this function
	 * the userland context has been saved to user_context and after
	 * returning from this function the userland context will be
	 * restored from user_context.
	 */
	switch(user_context->cpu_regs[MIPS_REGISTER_A0]) {
			
		case SYSCALL_HALT:
			DEBUG("debugsyscall","t:%d. SYSCALL_HALT\n",thread_get_current_thread());
			halt_kernel();
			break;
		case SYSCALL_EXEC:
		{
			DEBUG("debugsyscall","t:%d. SYSCALL_EXE\n",thread_get_current_thread());
			const char *file = (char*)user_context->cpu_regs[MIPS_REGISTER_A1];
			int pid = process_spawn(file);
			user_context->cpu_regs[MIPS_REGISTER_V0] = pid;
			break;
		}
		case SYSCALL_EXIT:
			DEBUG("debugsyscall","t:%d. SYSCALL_EXIT\n",thread_get_current_thread());
			process_finish(user_context->cpu_regs[MIPS_REGISTER_A1]);
			break;
		case SYSCALL_JOIN:
			DEBUG("debugsyscall","t:%d. SYSCALL_JOIN\n",thread_get_current_thread());
			user_context->cpu_regs[MIPS_REGISTER_V0] = 
				process_join((process_id_t)user_context->cpu_regs[MIPS_REGISTER_A1]);            
			break;
		case SYSCALL_READ:
		{
			DEBUG("debugsyscall","SYSCALL_READ\n");
			int fhandle = (int)user_context->cpu_regs[MIPS_REGISTER_A1];
			void *buffer = (void*)user_context->cpu_regs[MIPS_REGISTER_A2];
			int length = (int)user_context->cpu_regs[MIPS_REGISTER_A3];

			switch(fhandle)
			{
				// if stdin: read from tty
				case FILEHANDLE_STDIN:
				{
					gcd_t *gcd;
					gettty(&gcd);

					length = gcd->read(gcd, buffer, length);
					// da vi retunere length behover vi saa at saette afsluttende null tegn?
					//buffer2[len] = '\0';
					break;
				}
				//if stdout or stderr: nothing to read
				case FILEHANDLE_STDOUT:
				case FILEHANDLE_STDERR:
					length = 0;
					break;
				//else read file
				default:
					length = vfs_read(fhandle - 3, buffer, length);
					break;
			}
			user_context->cpu_regs[MIPS_REGISTER_V0] = length;
			break;
		}
		case SYSCALL_WRITE:
		{
			DEBUG("debugsyscall","SYSCALL_WRITE\n");
			int fhandle = (int)user_context->cpu_regs[MIPS_REGISTER_A1];
			void *buffer = (void*)user_context->cpu_regs[MIPS_REGISTER_A2];
			int length = (int)user_context->cpu_regs[MIPS_REGISTER_A3];

			switch(fhandle)
			{
				//if stdin: nothing written
				case FILEHANDLE_STDIN:
					length = 0;
					break;
				//if stdout or stderr write to tty
				case FILEHANDLE_STDOUT:
				case FILEHANDLE_STDERR:
				{
					gcd_t *gcd;
					gettty(&gcd);

					length = gcd->write(gcd, buffer, length);
					// da vi retunere length behover vi saa at saette afsluttende null tegn?
					//buffer2[len] = '\0';
					break;
				}
				//else write to file
				default:
					length = vfs_write(fhandle - 3, buffer, length);
					break;
			}
			user_context->cpu_regs[MIPS_REGISTER_V0] = length;
			break;
		}
		case SYSCALL_OPEN:
		{
			char *path = (char*)user_context->cpu_regs[MIPS_REGISTER_A1];
			user_context->cpu_regs[MIPS_REGISTER_V0] = vfs_open(path) + 3;
			break;
		}
		case SYSCALL_CLOSE:
		{
			int filehandle = (int)user_context->cpu_regs[MIPS_REGISTER_A1];
			user_context->cpu_regs[MIPS_REGISTER_V0] = vfs_close(filehandle - 3);
			break;
		}
		case SYSCALL_CREATE:
		{
			char *path = (char*)user_context->cpu_regs[MIPS_REGISTER_A1];
			int size = (int)user_context->cpu_regs[MIPS_REGISTER_A2];
			user_context->cpu_regs[MIPS_REGISTER_V0] = vfs_create(path, size);	
			break;
		}
		case SYSCALL_DELETE:
		{
			char *path = (char*)user_context->cpu_regs[MIPS_REGISTER_A1];
			user_context->cpu_regs[MIPS_REGISTER_V0] = vfs_remove(path);
			break;
		}
		case SYSCALL_SEEK:
		{
			int filehandle = (int)user_context->cpu_regs[MIPS_REGISTER_A1];
			int offset = (int)user_context->cpu_regs[MIPS_REGISTER_A2];
			user_context->cpu_regs[MIPS_REGISTER_V0] = vfs_seek(filehandle - 3, offset);
			break;
		}
		case SYSCALL_LISTFILES:
		{
			char *volumeName = (char*)user_context->cpu_regs[MIPS_REGISTER_A1];
			char *buffer = (char*)user_context->cpu_regs[MIPS_REGISTER_A2];
			int bufferSize = (int)user_context->cpu_regs[MIPS_REGISTER_A3];
			user_context->cpu_regs[MIPS_REGISTER_V0] = vfs_listFiles(volumeName, buffer, bufferSize);
			break;
		}
		default: 
			DEBUG("debugsyscall","syscall no: %d\n",user_context->cpu_regs[MIPS_REGISTER_A0]);
			KERNEL_PANIC("Unhandled system call\n");
	}
	/* Move to next instruction after system call */
	user_context->pc += 4;
}

void gettty(gcd_t **gcd) {
	device_t *dev;

	/* Find system console (first tty) */
	dev = device_get(YAMS_TYPECODE_TTY, 0);
	KERNEL_ASSERT(dev != NULL);

	*gcd = (gcd_t *)dev->generic_device;
	KERNEL_ASSERT(*gcd != NULL);
}
