/* testexec
 * This program tests the syscall: syscall_exec and syscall_join
 * Require the program testexit.c works proporly
 */

#include "tests/lib.h"

int main(void)
{
	int child_p, retval;
	child_p = syscall_exec("[test]testexit");
	retval = syscall_join(child_p);
	
	syscall_halt();
	
	return -1;
}
