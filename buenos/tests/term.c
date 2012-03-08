#include "tests/lib.h"
#include "tests/str.h"

int main(void)
{
    write("Welcome to the terminal.\n");
    while(1)
    {
        write("$ ");
        char buf[80];
        if(readline(buf, 80))
        {
            if(strcmp(buf,"exit") == 0)
                break;
            int pid = syscall_exec(buf);
            write("Starting ");
            write(buf);
            write(", ");
            writeInt(pid);
            write("\n");
            writeInt(syscall_join(pid));
        }
        else
            write("input to long\n");
    }
    return 0;
}
