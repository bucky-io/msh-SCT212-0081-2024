#!/bin/sh
set -eu

shell=${1:-./msh}
case "$shell" in
    /*) ;;
    *) shell="$PWD/$shell" ;;
esac

tmp=$(mktemp -d)
trap 'rm -rf "$tmp"' EXIT HUP INT TERM

output=$(printf '\n echo\t hello   world  \nexit\n' | "$shell")
printf '%s\n' "$output" | grep -q 'hello world'

output=$(printf 'nosuchcommand_msh_test\nexit\n' | "$shell" 2>&1)
printf '%s\n' "$output" | grep -q 'msh\[127\]>'

output=$(printf 'false\nexit\n' | "$shell")
printf '%s\n' "$output" | grep -q 'msh\[1\]>'

output=$(printf 'cd /tmp\npwd\nexit\n' | "$shell")
printf '%s\n' "$output" | grep -q '/tmp'

(cd "$tmp" && umask 000 && printf 'echo hi > out.txt\nexit\n' | "$shell" >/dev/null)
test "$(cat "$tmp/out.txt")" = hi
test "$(stat -c '%a' "$tmp/out.txt")" = 644

output=$(printf 'sleep 0.1 &\nsleep 0.3\nexit\n' | "$shell")
pid=$(printf '%s\n' "$output" | sed -n 's/.*\[msh\] started \([0-9][0-9]*\).*/\1/p')
test -n "$pid"
if kill -0 "$pid" 2>/dev/null; then
    printf 'background process %s was not reaped\n' "$pid" >&2
    exit 1
fi

printf 'All msh tests passed.\n'
