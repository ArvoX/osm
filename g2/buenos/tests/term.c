
#include "tests/lib.h"

char* numbers = "0123456789";

void write(char *buf);
void writeInt(int i);
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

void writeInt(int i)
{
    char c;
    if(i < 0)
    {
        c = '-';
        syscall_write(stdout, &c, 1);
        i = -i;
    }
    else if(i == 0)
    {
        c = '0';
        syscall_write(stdout, &c, 1);
        i = -i;
    }
    while(i > 0)
    {
        int j = i % 10;
        int i = i / 10;
        syscall_write(stdout, &numbers[j], 1);
    }
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
    int i, outfilled = 0;
    while(1)
    {
        int readed = syscall_read(stdin, buf, len);
        for(i = 0; i < readed; i++)
        {
            writeInt((int)buf[i]);
            if(buf[i] == '\n')
            {
                //char c = '\n';
                //syscall_write(stdout, &c, 1);
                return 1;
            }
            else if(++outfilled < len)
            {
                syscall_write(stdout, &buf[i], 1);
                out[outfilled] = buf[i];
            }
            else
                return 0;
        }
    }
}
