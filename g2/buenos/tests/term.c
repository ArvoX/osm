
#include "tests/lib.h"

void write(char *buf);
int strlen(char *buf);
void readline(char *buf);

int main(void)
{
    write("Stating term.\n");
    while(1)
    {
        write("$ ");
        char buf[80];
        syscall_read(stdin, buf, 80);
        syscall_exec(buf);
    }
    return 0;
}

void write(char *buf)
{
    int len = strlen(buf);

    syscall_write(stdout, buf, len);
}

int strlen(char *str)
{
    int i = 0;
    while(str[i] != NULL)
        i++;
    return i;
}
