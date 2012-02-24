
#include "tests/lib.h"

void write(char *buf);
int strlen(char *buf);
int readline(char *buf, int len);

int main(void)
{
    write("Stating term.\n");
    while(1)
    {
        write("$ ");
        char buf[80];
        if(readline(buf, 80))
            syscall_exec(buf);
        else
            write("input to long\n");
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
    while(str[i] != 0)
        i++;
    return i;
}

int readline(char *out, int len)
{
    char buf[len];
    int outfilled = 0;
    while(1)
    {
        int readed = syscall_read(stdin, buf, len);
        for(i = 0; i < readed; i++)
            if(buf[i] == '\n')
                return 1;
            else if(++outfilled < len)
                out[outfilled] = buf[i];
            else
                return 0;
    }
}
