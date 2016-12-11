#define _GNU_SOURCE

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/socket.h>
#include <stdbool.h>

#define R0(X) do { if ((X) !=  0) { perror(#X); exit(errno); } } while (0)
#define R1(X) do { if ((X) == -1) { perror(#X); exit(errno); } } while (0)

int main()
{
    int ss[2];
    R0(socketpair(AF_LOCAL, SOCK_STREAM, 0, ss));
    pid_t child_pid;
    R1(child_pid = fork());
    R0(close(ss[!child_pid]));
    while (true);
}
