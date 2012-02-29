/* testexit 
 * This program tests the syscall: syscall_exit
 */

#include "tests/lib.h"

int main(void)
{
	char *string = "running testexit.c\n";
	
	syscall_write(stdout,string,19);
	
	syscall_exit(42);
	return -1;
}
