#define _POSIX_C_SOURCE 200809L

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>

int main(void)
{
    char *line = NULL;
    size_t capacity = 0;

    for (;;) {
        fputs("msh> ", stdout);
        fflush(stdout);

        if (getline(&line, &capacity, stdin) == -1) {
            if (ferror(stdin)) {
                perror("getline");
                free(line);
                return EXIT_FAILURE;
            }
            break;
        }
    }

    free(line);
    return EXIT_SUCCESS;
}
