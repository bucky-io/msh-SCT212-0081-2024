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
static size_t split_arguments(char *line, char **argv)
{
    size_t argc = 0;
    char *token = strtok(line, " \t");

    while (token != NULL) {
        argv[argc++] = token;
        token = strtok(NULL, " \t");
    }

    argv[argc] = NULL;
    return argc;
}

int main(void)
{
    char *line = NULL;
    size_t line_capacity = 0;

    for (;;) {
        if (fputs("msh> ", stdout) == EOF || fflush(stdout) == EOF) {
            perror("msh: prompt");
            free(line);
            return EXIT_FAILURE;
        }

        ssize_t nread = getline(&line, &line_capacity, stdin);
        if (nread == -1) {
            if (ferror(stdin)) {
                perror("msh: getline");
                free(line);
                return EXIT_FAILURE;
            }

            break; /* EOF */
        }

        size_t length = (size_t)nread;
        if (length > 0 && line[length - 1] == '\n') {
            line[--length] = '\0';
        }

        /* At most one argument per two input characters, plus the NULL slot. */
        size_t argv_capacity = length / 2 + length % 2 + 1;
        char **argv = malloc(argv_capacity * sizeof(*argv));
        if (argv == NULL) {
            perror("msh: malloc");
            free(line);
            return EXIT_FAILURE;
        }

        size_t argc = split_arguments(line, argv);

        if (argc > 0) {
            /*
             * argv[0..argc-1] are the arguments and argv[argc] is NULL.
             * Task 3 will execute this command.
             */
        }

        free(argv);
    }

    free(line);
    return EXIT_SUCCESS;
}#define _POSIX_C_SOURCE 200809L

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

static size_t split_arguments(char *line, char **argv)
{
    size_t argc = 0;
    char *token = strtok(line, " \t");

    while (token != NULL) {
        argv[argc++] = token;
        token = strtok(NULL, " \t");
    }

    argv[argc] = NULL;
    return argc;
}

int main(void)
{
    char *line = NULL;
    size_t line_capacity = 0;

    for (;;) {
        if (fputs("msh> ", stdout) == EOF || fflush(stdout) == EOF) {
            perror("msh: prompt");
            free(line);
            return EXIT_FAILURE;
        }

        ssize_t nread = getline(&line, &line_capacity, stdin);
        if (nread == -1) {
            if (ferror(stdin)) {
                perror("msh: getline");
                free(line);
                return EXIT_FAILURE;
            }

            break; /* EOF */
        }

        size_t length = (size_t)nread;
        if (length > 0 && line[length - 1] == '\n') {
            line[--length] = '\0';
        }

        size_t argv_capacity = length / 2 + length % 2 + 1;
        char **argv = malloc(argv_capacity * sizeof(*argv));
        if (argv == NULL) {
            perror("msh: malloc");
            free(line);
            return EXIT_FAILURE;
        }

        size_t argc = split_arguments(line, argv);

        if (argc > 0) {
            pid_t pid = fork();

            if (pid == -1) {
                perror("msh: fork");
            } else if (pid == 0) {
                execvp(argv[0], argv);
                perror(argv[0]);
                _exit(EXIT_FAILURE);
            } else {
                int status;
                pid_t waited;

                do {
                    waited = waitpid(pid, &status, 0);
                } while (waited == -1 && errno == EINTR);

                if (waited == -1) {
                    perror("msh: waitpid");
                }
            }
        }

        free(argv);
    }

    free(line);
    return EXIT_SUCCESS;
}