/* testexec
 * This program tests the syscall: syscall_exec and syscall_join
 * Require the program testexit.c works proporly
 */

#include "tests/lib.h"
#include "tests/str.h"

int main(void)
{
	int child_p, retval;

	write("dette er en test\n");

	child_p = syscall_exec("[test]testexit");
	retval = syscall_join(child_p);
	
	syscall_halt();
	
	return -1;
}
