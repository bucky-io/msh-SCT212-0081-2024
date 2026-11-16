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
                perror("msh: fork");                #define _POSIX_C_SOURCE 200809L
                
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
                
                static void print_help(void)
                {
                    puts("Built-ins:");
                    puts("  cd [dir]  Change directory (defaults to $HOME)");
                    puts("  help      Show this help");
                    puts("  exit      Exit msh");
                    puts("External commands are run with their arguments.");
                }
                
                int main(void)
                {
                    char *line = NULL;
                    size_t line_capacity = 0;
                    int last_status = 0;
                
                    for (;;) {
                        if (printf("msh[%d]> ", last_status) < 0 || fflush(stdout) == EOF) {
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
                        int exit_shell = 0;
                
                        if (argc > 0) {
                            if (strcmp(argv[0], "cd") == 0) {
                                if (argc > 2) {
                                    fprintf(stderr, "msh: usage: cd [dir]\n");
                                    last_status = 2;
                                } else {
                                    const char *directory =
                                        (argc == 1) ? getenv("HOME") : argv[1];
                
                                    if (directory == NULL) {
                                        fprintf(stderr, "msh: HOME is not set\n");
                                        last_status = 1;
                                    } else if (chdir(directory) == -1) {
                                        perror("msh: cd");
                                        last_status = 1;
                                    } else {
                                        last_status = 0;
                                    }
                                }
                            } else if (strcmp(argv[0], "help") == 0) {
                                print_help();
                                last_status = 0;
                            } else if (strcmp(argv[0], "exit") == 0) {
                                if (argc != 1) {
                                    fprintf(stderr, "msh: usage: exit\n");
                                    last_status = 2;
                                } else {                                #define _POSIX_C_SOURCE 200809L
                                
                                #include <errno.h>
                                #include <signal.h>
                                #include <stdio.h>
                                #include <stdlib.h>
                                #include <string.h>
                                #include <sys/types.h>
                                #include <sys/wait.h>
                                #include <unistd.h>
                                
                                #define MAX_BACKGROUND_JOBS 64
                                
                                static volatile sig_atomic_t background_pids[MAX_BACKGROUND_JOBS];
                                
                                static void reap_background_children(int signal_number)
                                {
                                    int saved_errno = errno;
                                    (void)signal_number;
                                
                                    for (size_t i = 0; i < MAX_BACKGROUND_JOBS; i++) {
                                        sig_atomic_t stored_pid = background_pids[i];
                                
                                        if (stored_pid <= 0) {
                                            continue;
                                        }
                                
                                        int status;
                                        pid_t pid = (pid_t)stored_pid;
                                        pid_t result;
                                
                                        do {
                                            result = waitpid(pid, &status, WNOHANG);
                                        } while (result == -1 && errno == EINTR);
                                
                                        if (result == pid || (result == -1 && errno == ECHILD)) {
                                            background_pids[i] = 0;
                                        }
                                    }
                                
                                    errno = saved_errno;
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
                                
                                static void print_help(void)
                                {
                                    puts("Built-ins:");
                                    puts("  cd [dir]  Change directory (defaults to $HOME)");
                                    puts("  help      Show this help");
                                    puts("  exit      Exit msh");
                                    puts("External commands are run with their arguments.");
                                }
                                
                                int main(void)
                                {
                                    struct sigaction action = {0};
                                    action.sa_handler = reap_background_children;
                                    action.sa_flags = SA_RESTART;
                                    sigemptyset(&action.sa_mask);
                                
                                    if (sigaction(SIGCHLD, &action, NULL) == -1) {
                                        perror("msh: sigaction");
                                        return EXIT_FAILURE;
                                    }
                                
                                    sigset_t child_signal;
                                    sigset_t previous_mask;
                                    sigemptyset(&child_signal);
                                    sigaddset(&child_signal, SIGCHLD);
                                
                                    char *line = NULL;
                                    size_t line_capacity = 0;
                                    int last_status = 0;
                                
                                    for (;;) {
                                        if (printf("msh[%d]> ", last_status) < 0 ||
                                            fflush(stdout) == EOF) {
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
                                
                                            break;
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
                                        int exit_shell = 0;
                                
                                        if (argc > 0) {
                                            int background = strcmp(argv[argc - 1], "&") == 0;
                                            if (background) {
                                                argv[--argc] = NULL;
                                            }
                                
                                            if (argc == 0) {
                                                fprintf(stderr, "msh: expected a command before &\n");
                                                last_status = 2;
                                            } else if (strcmp(argv[0], "cd") == 0) {
                                                if (argc > 2) {
                                                    fprintf(stderr, "msh: usage: cd [dir]\n");
                                                    last_status = 2;
                                                } else {
                                                    const char *directory =
                                                        (argc == 1) ? getenv("HOME") : argv[1];
                                
                                                    if (directory == NULL) {
                                                        fprintf(stderr, "msh: HOME is not set\n");
                                                        last_status = 1;
                                                    } else if (chdir(directory) == -1) {
                                                        perror("msh: cd");
                                                        last_status = 1;
                                                    } else {
                                                        last_status = 0;
                                                    }
                                                }
                                            } else if (strcmp(argv[0], "help") == 0) {
                                                print_help();
                                                last_status = 0;
                                            } else if (strcmp(argv[0], "exit") == 0) {
                                                if (argc != 1) {
                                                    fprintf(stderr, "msh: usage: exit\n");
                                                    last_status = 2;
                                                } else {
                                                    exit_shell = 1;
                                                }
                                            } else if (sigprocmask(SIG_BLOCK, &child_signal,
                                                                   &previous_mask) == -1) {
                                                perror("msh: sigprocmask");
                                                last_status = 1;
                                            } else {
                                                size_t job_slot = MAX_BACKGROUND_JOBS;
                                
                                                if (background) {
                                                    for (size_t i = 0; i < MAX_BACKGROUND_JOBS; i++) {
                                                        if (background_pids[i] == 0) {
                                                            job_slot = i;
                                                            break;
                                                        }
                                                    }
                                                }
                                
                                                if (background && job_slot == MAX_BACKGROUND_JOBS) {
                                                    fprintf(stderr, "msh: too many background jobs\n");
                                                    last_status = 1;
                                
                                                    if (sigprocmask(SIG_SETMASK, &previous_mask, NULL) == -1) {
                                                        perror("msh: sigprocmask");
                                                        free(argv);
                                                        free(line);
                                                        return EXIT_FAILURE;
                                                    }
                                                } else {
                                                    pid_t pid = fork();
                                
                                                    if (pid == -1) {
                                                        perror("msh: fork");
                                                        last_status = 1;
                                                    } else if (pid == 0) {
                                                        if (sigprocmask(SIG_SETMASK, &previous_mask, NULL) == -1) {
                                                            perror("msh: sigprocmask");
                                                            _exit(126);
                                                        }
                                
                                                        execvp(argv[0], argv);
                                                        int exec_error = errno;
                                                        perror(argv[0]);
                                                        _exit(exec_error == ENOENT ? 127 : 126);
                                                    } else if (background) {
                                                        background_pids[job_slot] = (sig_atomic_t)pid;
                                                        last_status = 0;
                                                    } else {
                                                        last_status = -1; /* Set below from the child status. */
                                                    }
                                
                                                    if (sigprocmask(SIG_SETMASK, &previous_mask, NULL) == -1) {
                                                        perror("msh: sigprocmask");
                                                        free(argv);
                                                        free(line);
                                                        return EXIT_FAILURE;
                                                    }
                                
                                                    if (pid > 0 && background) {
                                                        printf("[msh] started %ld\n", (long)pid);
                                                    } else if (pid > 0) {
                                                        int status;
                                                        pid_t waited;
                                
                                                        do {
                                                            waited = waitpid(pid, &status, 0);
                                                        } while (waited == -1 && errno == EINTR);
                                
                                                        if (waited == -1) {
                                                            perror("msh: waitpid");
                                                            last_status = 1;
                                                        } else if (WIFEXITED(status)) {
                                                            last_status = WEXITSTATUS(status);
                                                        } else if (WIFSIGNALED(status)) {
                                                            last_status = 128 + WTERMSIG(status);
                                                        } else {
                                                            last_status = 1;
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                
                                        free(argv);
                                
                                        if (exit_shell) {
                                            break;
                                        }
                                    }
                                
                                    free(line);
                                    return EXIT_SUCCESS;
                                }
                                    exit_shell = 1;
                                }
                            } else {
                                pid_t pid = fork();
                
                                if (pid == -1) {
                                    perror("msh: fork");
                                    last_status = 1;
                                } else if (pid == 0) {
                                    execvp(argv[0], argv);
                                    int exec_error = errno;
                                    perror(argv[0]);
                                    _exit(exec_error == ENOENT ? 127 : 126);
                                } else {
                                    int status;
                                    pid_t waited;
                
                                    do {
                                        waited = waitpid(pid, &status, 0);
                                    } while (waited == -1 && errno == EINTR);
                
                                    if (waited == -1) {
                                        perror("msh: waitpid");
                                        last_status = 1;
                                    } else if (WIFEXITED(status)) {
                                        last_status = WEXITSTATUS(status);
                                    } else if (WIFSIGNALED(status)) {
                                        last_status = 128 + WTERMSIG(status);
                                    } else {
                                        last_status = 1;
                                    }
                                }
                            }
                        }
                
                        free(argv);
                
                        if (exit_shell) {
                            break;
                        }
                    }
                
                    free(line);
                    return EXIT_SUCCESS;
                }
            } else if (pid == 0) {            #define _POSIX_C_SOURCE 200809L
            
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
            
            static void print_help(void)
            {
                puts("Built-ins:");
                puts("  cd [dir]  Change directory (defaults to $HOME)");
                puts("  help      Show this help");
                puts("  exit      Exit msh");
                puts("External commands are run with their arguments.");
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
                    int exit_shell = 0;
            
                    if (argc > 0) {
                        if (strcmp(argv[0], "cd") == 0) {
                            if (argc > 2) {
                                fprintf(stderr, "msh: usage: cd [dir]\n");
                            } else {
                                const char *directory =
                                    (argc == 1) ? getenv("HOME") : argv[1];
            
                                if (directory == NULL) {
                                    fprintf(stderr, "msh: HOME is not set\n");
                                } else if (chdir(directory) == -1) {
                                    perror("msh: cd");
                                }
                            }
                        } else if (strcmp(argv[0], "help") == 0) {
                            print_help();
                        } else if (strcmp(argv[0], "exit") == 0) {
                            if (argc != 1) {
                                fprintf(stderr, "msh: usage: exit\n");
                            } else {
                                exit_shell = 1;
                            }
                        } else {
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
                    }
            
                    free(argv);
            
                    if (exit_shell) {
                        break;
                    }
                }
            
                free(line);
                return EXIT_SUCCESS;
            }
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
}#define _POSIX_C_SOURCE 200809L

#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define MAX_BACKGROUND_JOBS 64

static volatile sig_atomic_t background_pids[MAX_BACKGROUND_JOBS];

static void reap_background_children(int signal_number)
{
    int saved_errno = errno;
    (void)signal_number;

    for (size_t i = 0; i < MAX_BACKGROUND_JOBS; i++) {
        sig_atomic_t stored_pid = background_pids[i];

        if (stored_pid <= 0) {
            continue;
        }

        pid_t pid = (pid_t)stored_pid;
        int status;
        pid_t result;

        do {
            result = waitpid(pid, &status, WNOHANG);
        } while (result == -1 && errno == EINTR);

        if (result == pid || (result == -1 && errno == ECHILD)) {
            background_pids[i] = 0;
        }
    }

    errno = saved_errno;
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

static int is_builtin(const char *command)
{
    return strcmp(command, "cd") == 0 ||
           strcmp(command, "help") == 0 ||
           strcmp(command, "exit") == 0;
}

static void print_help(void)
{
    puts("Built-ins:");
    puts("  cd [dir]  Change directory (defaults to $HOME)");
    puts("  help      Show this help");
    puts("  exit      Exit msh");
    puts("External commands support output redirection with >.");
}

int main(void)
{
    struct sigaction action = {0};
    action.sa_handler = reap_background_children;
    action.sa_flags = SA_RESTART;
    sigemptyset(&action.sa_mask);

    if (sigaction(SIGCHLD, &action, NULL) == -1) {
        perror("msh: sigaction");
        return EXIT_FAILURE;
    }

    sigset_t child_signal;
    sigset_t previous_mask;
    sigemptyset(&child_signal);
    sigaddset(&child_signal, SIGCHLD);

    char *line = NULL;
    size_t line_capacity = 0;
    int last_status = 0;

    for (;;) {
        if (printf("msh[%d]> ", last_status) < 0 ||
            fflush(stdout) == EOF) {
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
            break;
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
        int exit_shell = 0;

        if (argc > 0) {
            int background = strcmp(argv[argc - 1], "&") == 0;
            if (background) {
                argv[--argc] = NULL;
            }

            const char *output_path = NULL;
            int redirection_error = 0;

            for (size_t i = 0; i < argc; i++) {
                if (strcmp(argv[i], ">") == 0) {
                    if (output_path != NULL || i + 2 != argc) {
                        redirection_error = 1;
                        break;
                    }

                    output_path = argv[i + 1];
                    argv[i] = NULL;
                    argc = i;
                    break;
                }
            }

            if (redirection_error || argc == 0) {
                fprintf(stderr, "msh: usage: command [args] > file\n");
                last_status = 2;
            } else if (output_path != NULL && is_builtin(argv[0])) {
                fprintf(stderr, "msh: redirection is supported for external commands only\n");
                last_status = 2;
            } else if (background && is_builtin(argv[0])) {
                fprintf(stderr, "msh: built-ins cannot run in the background\n");
                last_status = 2;
            } else if (strcmp(argv[0], "cd") == 0) {
                if (argc > 2) {
                    fprintf(stderr, "msh: usage: cd [dir]\n");
                    last_status = 2;
                } else {
                    const char *directory =
                        (argc == 1) ? getenv("HOME") : argv[1];

                    if (directory == NULL) {
                        fprintf(stderr, "msh: HOME is not set\n");
                        last_status = 1;
                    } else if (chdir(directory) == -1) {
                        perror("msh: cd");
                        last_status = 1;
                    } else {
                        last_status = 0;
                    }
                }
            } else if (strcmp(argv[0], "help") == 0) {
                print_help();
                last_status = 0;
            } else if (strcmp(argv[0], "exit") == 0) {
                if (argc != 1) {
                    fprintf(stderr, "msh: usage: exit\n");
                    last_status = 2;
                } else {
                    exit_shell = 1;
                }
            } else if (sigprocmask(SIG_BLOCK, &child_signal,
                                   &previous_mask) == -1) {
                perror("msh: sigprocmask");
                last_status = 1;
            } else {
                size_t job_slot = MAX_BACKGROUND_JOBS;

                if (background) {
                    for (size_t i = 0; i < MAX_BACKGROUND_JOBS; i++) {
                        if (background_pids[i] == 0) {
                            job_slot = i;
                            break;
                        }
                    }
                }

                if (background && job_slot == MAX_BACKGROUND_JOBS) {
                    fprintf(stderr, "msh: too many background jobs\n");
                    last_status = 1;
                } else {
                    pid_t pid = fork();

                    if (pid == -1) {
                        perror("msh: fork");
                        last_status = 1;
                    } else if (pid == 0) {
                        if (sigprocmask(SIG_SETMASK, &previous_mask, NULL) == -1) {
                            perror("msh: sigprocmask");
                            _exit(126);
                        }

                        if (output_path != NULL) {
                            int output_fd = open(
                                output_path,
                                O_WRONLY | O_CREAT | O_TRUNC,
                                0644
                            );

                            if (output_fd == -1) {
                                perror(output_path);
                                _exit(1);
                            }

                            if (dup2(output_fd, STDOUT_FILENO) == -1) {
                                perror("msh: dup2");
                                close(output_fd);
                                _exit(1);
                            }

                            if (close(output_fd) == -1) {
                                perror("msh: close");
                                _exit(1);
                            }
                        }

                        execvp(argv[0], argv);
                        int exec_error = errno;
                        perror(argv[0]);
                        _exit(exec_error == ENOENT ? 127 : 126);
                    } else if (background) {
                        background_pids[job_slot] = (sig_atomic_t)pid;
                        last_status = 0;
                    } else {
                        last_status = -1;
                    }

                    if (sigprocmask(SIG_SETMASK, &previous_mask, NULL) == -1) {
                        perror("msh: sigprocmask");
                        free(argv);
                        free(line);
                        return EXIT_FAILURE;
                    }

                    if (pid > 0 && background) {
                        printf("[msh] started %ld\n", (long)pid);
                    } else if (pid > 0) {
                        int status;
                        pid_t waited;

                        do {
                            waited = waitpid(pid, &status, 0);
                        } while (waited == -1 && errno == EINTR);

                        if (waited == -1) {
                            perror("msh: waitpid");
                            last_status = 1;
                        } else if (WIFEXITED(status)) {
                            last_status = WEXITSTATUS(status);
                        } else if (WIFSIGNALED(status)) {
                            last_status = 128 + WTERMSIG(status);
                        } else {
                            last_status = 1;
                        }
                    }
                }

                if (background && job_slot == MAX_BACKGROUND_JOBS) {
                    if (sigprocmask(SIG_SETMASK, &previous_mask, NULL) == -1) {
                        perror("msh: sigprocmask");
                        free(argv);
                        free(line);
                        return EXIT_FAILURE;
                    }
                }
            }
        }

        free(argv);

        if (exit_shell) {
            break;
        }
    }

    free(line);
    return EXIT_SUCCESS;
}