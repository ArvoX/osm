#include "tests/lib.h"
#include "tests/str.h"

void show(const char *filename);
void touch(const char *filename);
void rm(const char *filename);
void cp(const char *from, const char *to);
void ls(const char *volumename);
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
            nextArg += getFirstArg(&buf[nextArg], firstArg);
            nextArg += getFirstArg(&buf[nextArg], secondArg);
            if(strcmp(buf,"exit") == 0)
                break;
            else if(strcmp(command, "show") == 0)
                show(firstArg);
            else if(strcmp(command, "touch") == 0)
                touch(firstArg);
            else if(strcmp(command, "rm") == 0)
                rm(firstArg);
            else if(strcmp(command, "cp") == 0)
                cp(firstArg, secondArg);
            else if(strcmp(command, "ls") == 0)
                ls(firstArg);
            else
            {
                int pid = syscall_exec(buf);
                write("Starting ");
                write(buf);
                write(", ");
                writeInt(pid);
                write("\n");
                writeInt(syscall_join(pid));
            }
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

void touch(const char *filename)
{
    syscall_create(filename, 2048);
}

void rm(const char *filename)
{
    syscall_delete(filename);
}

void cp(const char *from, const char *to)
{
    char *buf[80];
    int in = syscall_open(from);
    int out = syscall_open(to);
    int readed;
    while((readed = syscall_read(in, buf, 80)) > 0)
        syscall_write(out, buf, readed);
    syscall_close(in);
    syscall_close(out);
}

void ls(const char *volumename)
{
    char buffer[512];
    syscall_listfiles(volumename, buffer, 512);
    int l;
    for(l = strlen(buffer); l > 0; buffer)
        writeLine(buffer);
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
    while(str[++i] == ' ');

    return i;
}
