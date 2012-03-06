
#include "tests/lib.h"

char* numbers = "0123456789";

void write(char *buf);
void writeInt(int i);
int strlen(char *buf);
int strcmp(char *s1, char *s2);
int readline(char *buf, int len);

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

void write(char *buf)
{
    int len = strlen(buf);

    syscall_write(stdout, buf, len);
}

void writeInt(int i)
{
    char c[80];
    if(i < 0)
    {
        c[0] = '-';
        syscall_write(stdout, &c, 1);
        i = -i;
    }
    else if(i == 0)
    {
        syscall_write(stdout, &numbers[0], 1);
    }
    int k = 80;
    while(i > 0)
    {
        int j = i % 10;
        i = i / 10;
        if(k > 0)
            c[--k] = numbers[j];
        else
        {
            write("number to big\n");
            return;
        }

    }
    syscall_write(stdout, &c[k], 80-k);
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
    int bufsize = len - 1;
    char buf[bufsize];
    int i, outfilled = 0;
    while(1)
    {
        int readed = syscall_read(stdin, buf, bufsize);
        for(i = 0; i < readed; i++)
        {
            if(buf[i] == 13)
            {
                char c = '\n';
                syscall_write(stdout, &c, 1);
                out[outfilled] = '\0';
                return 1;
            }
            else if(outfilled < bufsize)
            {
                syscall_write(stdout, &buf[i], 1);
                out[outfilled++] = buf[i];
            }
            else
                return 0;
        }
    }
}

int strcmp(char *s1, char *s2)
{
    int i;
    for(i=0; s1[i] != '\0' && s2[i] != '\0'; i++)
        if(s1[i] < s2i[i])
            return -1;
        else if(s1[i] > s2i[i])
            return 1;
    return 0;
}
