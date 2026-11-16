# msh: Minimal Unix Shell

**Name:** [Your name]  
**Student ID:** [Your student ID]

## Build and run

```sh
gcc -std=c11 -Wall -Wextra -Wpedantic msh.c -o msh
./msh
```

Press Ctrl-D to exit. To run the included automated checks:

```sh
sh tests/test_msh.sh
```

## Features

- [x] Task 1: prompt, arbitrary-length input, and clean EOF handling
- [x] Task 2: whitespace-separated arguments and blank-line handling
- [x] Task 3: external commands through `fork()`, `execvp()`, and `waitpid()`
- [x] Task 4: `cd`, `help`, and `exit` built-ins
- [x] Task 5: previous command status in the prompt
- [x] Task 6: background execution and child reaping
- [x] Task 7: output redirection with `>`
- [x] Task 8: Ctrl-C interrupts a foreground command without terminating msh

## Design

The shell displays the most recent command status in its prompt and reads each
input line with `getline()`, so command length is not limited by a fixed buffer.
It tokenizes the line on spaces and tabs, skips empty lines, and builds an
argument vector terminated by a null pointer. Quoting, pipes, and variable
expansion are intentionally outside this shell's small command syntax.

For an external command, msh calls `fork()`. The child restores the default
Ctrl-C behavior and, when requested, redirects standard output before calling
`execvp()` to replace itself with the requested program. If execution fails,
the child reports the error and exits with status 127 for a missing command or
126 for another execution failure. The parent waits for a foreground child's
status with `waitpid()`, retrying if interrupted. It displays the normal exit
code, or 128 plus the terminating signal number.

The `cd` and `exit` commands must be built-ins because they change or end the
shell process itself. Running `cd` in a child would change only that child’s
working directory, leaving the interactive shell where it was. Similarly, an
external `exit` process cannot terminate its parent shell. `help` is also
implemented in the shell for a concise feature summary.

For a background command, msh records the child PID while SIGCHLD is blocked,
then restores the signal mask. The SIGCHLD handler calls `waitpid()` with
`WNOHANG` for recorded jobs, so completed background children are reaped even
while msh is waiting for another input line. Completed job records are freed
in the main loop, not in the signal handler. The shell ignores SIGINT while
children restore its default action, allowing Ctrl-C to stop the foreground
program while msh remains available.

## Known limitations

- Arguments are separated only by whitespace; there is no quoting, escaping,
  globbing, variable expansion, pipelines, or command sequencing.
- Output redirection supports one `>` target for external commands only.
- Background-job completion is reaped silently; there is no `jobs` command or
  job-control support.
- `cd` and `exit` accept only the documented basic forms.

## Sources and AI-tool declaration

- The supplied practical assignment PDF was used for the required behavior and
  test cases.
- The OpenAI/Copilot AI assistant was used to help implement and review the C
  code, prepare documentation, and run tests. The submitted implementation
  should be reviewed and understood by the student.
