/* testexit 
 * This program tests the syscall: syscall_exit
 */

#include "tests/lib.h"

int main(void)
{
	syscall_exit(42);
	return -1;
}
