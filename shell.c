#define _GNU_SOURCE

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <memory.h>
#include <fcntl.h>

#define R0(X) do { if ((X) !=  0) { perror(#X); exit(errno); } } while (0)
#define R1(X) do { if ((X) == -1) { perror(#X); exit(errno); } } while (0)


void ex(char ** args)
{
    char buffer[1024];
    char * current = buffer;
    current += sprintf(current, "Running: ");
    for (char ** temp = args; *temp; temp++)
        current += sprintf(current, "%s ", *temp);
    fprintf(stderr, "%s\n", buffer);
    if (execvp(args[0], args) == -1)
        perror(args[0]);
}


void pipee(char ** const Begin, char ** const End)
{
    char ** head = Begin;
    char ** tail = head;
    while (tail != End)
    {
        while (*tail++);

        if (tail == End)
            ex(head);

        int pfd[2];
        R0(pipe(pfd));
        pid_t child_pid;
        R1(child_pid = fork());
        if (!child_pid)
        {
            R0(close(STDOUT_FILENO));
            R1(dup2(pfd[1], STDOUT_FILENO));
            R0(close(pfd[1]));
            R0(close(pfd[0]));
            ex(head);
        }

        R0(close(STDIN_FILENO));
        R1(dup2(pfd[0], STDIN_FILENO));
        R0(close(pfd[1]));
        R0(close(pfd[0]));
        head = tail;
    }
}

int main()
{
    char * line = NULL;
    size_t len = 0;
    R1(getline(&line, &len, stdin));
    char * token;
    char * tokens[64 * 1024] = {0};
    size_t tokens_count = 0;
    while ((token = strsep(&line, " \n\r")) != NULL)
        if (*token)
            tokens[tokens_count++] = (strcmp(token, "|") != 0) ? token : NULL;
    tokens_count++;

    int output;
    R1(output = open("/home/dima/result.out", O_RDWR | O_CREAT | O_TRUNC));
    R0(close(STDOUT_FILENO));
    R1(dup2(output, STDOUT_FILENO));
    R0(close(output));

    pipee(tokens, tokens + tokens_count);
    exit(EXIT_SUCCESS);
}
