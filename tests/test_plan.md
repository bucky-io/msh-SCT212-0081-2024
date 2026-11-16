# msh Test Plan

Run the automated checks with:

```sh
sh tests/test_msh.sh
```

The Ctrl-C test was run in a pseudo-terminal so the terminal's interrupt
character generated SIGINT as it would interactively.

| Input / test | Expected behaviour | Actual result |
|---|---|---|
| Blank line | A new prompt appears without an error | Passed; prompt returned |
| `echo hello world` | Prints the text and shows status 0 | Passed; printed `hello world`, next prompt was `msh[0]>` |
| `ls /nonexistent` | `ls` reports an error and prompt shows a non-zero status | Passed; `ls` reported the missing path and prompt showed `msh[2]>` |
| `nosuchcommand` | Error message, status 127, and shell remains usable | Passed; error printed, next prompt was `msh[127]>` |
| `cd /tmp` then `pwd` | Prints `/tmp` | Passed |
| `sleep 3 &` | Prompt returns immediately and child PID is printed | Passed; PID printed and the prompt returned before the following foreground `sleep 3` |
| `echo hi > f.txt` then `cat f.txt` | File contains `hi` | Passed; file contained `hi` and had mode 0644 |
| `sleep 100` then Ctrl-C | Sleep stops; shell survives and shows status 130 | Passed in a pseudo-terminal; prompt showed `msh[130]>` and accepted `exit` |
| Ctrl-D at the prompt | Shell exits cleanly | Passed; exit status 0 |
| Extra test: tabs and repeated spaces in `echo` arguments | Whitespace is ignored between arguments | Passed; printed `hello world` |
| Extra test: `false` followed by another command | Prompt shows status 1 and shell continues | Passed; prompt showed `msh[1]>` |
| Extra test: `cd` with no argument followed by `pwd` | Changes to `$HOME` | Passed; printed the home directory |
| Extra test: complete a background job, then inspect its PID | Completed background child is reaped | Passed; completed PID no longer existed |
| Extra test: 100,000 spaces followed by a newline | Long input is read without a fixed-buffer limit and treated as blank | Passed; next prompt appeared normally |
