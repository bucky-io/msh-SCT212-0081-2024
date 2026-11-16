#define _POSIX_C_SOURCE 200809L

#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void)
{
    char *line = NULL;
    size_t capacity = 0;

    for (;;) {
        if (fputs("msh> ", stdout) == EOF || fflush(stdout) == EOF) {
            perror("msh: prompt");
            free(line);
            return EXIT_FAILURE;
        }

        if (getline(&line, &capacity, stdin) == -1) {
            if (ferror(stdin)) {
                perror("msh: getline");
                free(line);
                return EXIT_FAILURE;
            }

            /* EOF (Ctrl-D): exit cleanly after releasing getline's buffer. */
            break;
        }

        /* Task 1 only: the line is discarded; Task 2 will parse it. */
    }

    free(line);
    return EXIT_SUCCESS;
}