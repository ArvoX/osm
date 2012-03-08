#include "tests/lib.h"
#include "tests/str.h"

void write(char *buf)
{
    int len = strlen(buf);

    syscall_write(stdout, buf, len);
}

void writeChar(char c)
{
    syscall_write(stdout, &c, 1);
}

void writeInt(int i)
{
    char c[80];
    if(i < 0)
    {
        writeChar('-');
        i = -i;
    }
    else if(i == 0)
    {
        writeChar(numbers[0]);
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
            switch(buf[i])
            {
                case '\r':
                case '\n':
                    writeChar('\n');
                    out[outfilled] = '\0';
                    return 1;
                case 127:
                    if(outfilled > 0)
                    {
                        writeChar('\b');
                        writeChar(' ');
                        writeChar('\b');
                        outfilled--;
                    }
                    break;
                default:
                    if(outfilled < bufsize)
                    {
                        syscall_write(stdout, &buf[i], 1);
                        out[outfilled++] = buf[i];
                    }
                    else
                        return 0;
            }
        }
    }
}

int strcmp(char *s1, char *s2)
{
    int i;
    for(i=0; s1[i] != '\0' && s2[i] != '\0'; i++)
        if(s1[i] < s2[i])
            return -1;
        else if(s1[i] > s2[i])
            return 1;
    return 0;
}
