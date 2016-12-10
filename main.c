#define _GNU_SOURCE

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <pwd.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <sched.h>

#define R0(X) do { if ((X) !=  0) { perror(#X); exit(errno); } } while (0)
#define R1(X) do { if ((X) == -1) { perror(#X); exit(errno); } } while (0)

int open_fifo(const char * name, int flag)
{
    const char * home_dir = getpwuid(getuid())->pw_dir;
    char path[1024] = {0};
    sprintf(path, "%s/%s", home_dir, name);
    mkfifo(path, S_IRWXU | S_IRWXG | S_IRWXO);
    int fd;
    R1(fd = open(path, flag));
    return fd;
}

int main()
{
    const int in = open_fifo("in.fifo", O_RDONLY);
    const int out = open_fifo("out.fifo", O_WRONLY);
    while (true)
    {
        R1(splice(in, NULL, out, NULL, 0xFFFF, 0));
        sched_yield();
    }
}
