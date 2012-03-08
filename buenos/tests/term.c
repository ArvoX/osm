#include "tests/lib.h"
#include "tests/str.h"

void show(const char *filename);
int getFirstArg(const char *str, char *arg);

int main(void)
{
    write("Welcome to the terminal.\n");
    while(1)
    {
        write("$ ");
        char buf[80];
        if(readline(buf, 80))
        {
            int nextArg;
            char command[80], firstArg[80], secondArg[80];
            nextArg = getFirstArg(buf, command);
            nextArg = getFirstArg(&buf[nextArg], firstArg);
            nextArg = getFirstArg(&buf[nextArg], secondArg);
            if(strcmp(buf,"exit") == 0)
                break;
            else if(strcmp(command, "show") == 0);
                show(firstArg);
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

void show(const char *filename)
{
    char *buf[80];
    int fid = syscall_open(filename);
    int readed;
    while((readed = syscall_read(fid, buf, 80)) > 0)
        syscall_write(stdout, buf, readed);
    syscall_close(fid);
}

int getFirstArg(const char *str, char *arg)
{
    int i;
    int q = 0;
    for(i = 0; str[i] != '\0'; i++)
        if(str[i] == '"')
            q = !q;
        else if(!q && str[i] == ' ')
            break;
        else
            arg[i] = str[i];
    arg[i] = '\0';
    while(str[i++] == ' ');

    return i;
}
