#!/usr/bin/env bash
# Build and run one solution. The single source of compiler flags for Linux and macOS:
# Sublime's build systems, FastOlympicCoding and stress.sh all go through this script.
#
# Usage: run.sh <mode> <file.cpp|file.py> [output-binary]
#   run       release build, stdin from input.txt
#   debug     sanitizer build (ASan + UBSan + checked STL), stdin from input.txt
#   timed     release build, prints wall/cpu time and peak memory
#   output    release build, stdout also saved to output.txt
#   compile   release build only
#   terminal  release build, run in a terminal window (interactive problems)
#
# Written for bash 3.2 (the bash macOS ships).
set -u

MODE="${1:-}"
SRC="${2:-}"
OUT="${3:-}"
if [ -z "$MODE" ] || [ -z "$SRC" ]; then
    sed -n '2,12p' "${BASH_SOURCE[0]}" | sed 's/^# \{0,1\}//' >&2
    exit 2
fi

SCRIPTS="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd -P)"
OS="$(uname -s)"

if [ ! -f "$SRC" ]; then
    echo "cp: no such file: $SRC" >&2
    exit 2
fi
cd "$(dirname "$SRC")" || exit 2
FILE="$(basename "$SRC")"
BASE="${FILE%.*}"
EXT="${FILE##*.}"

WARN="-Wall -Wextra -Wshadow"
RELEASE="-std=gnu++20 -O2 -pipe $WARN -DLOCAL"
DEBUG="-std=gnu++20 -O0 -g $WARN -Wconversion -DLOCAL -D_GLIBCXX_DEBUG -D_GLIBCXX_ASSERTIONS -fsanitize=address,undefined -fno-sanitize-recover=all"
# Deep recursion: macOS fixes the main thread's stack at link time (512 MB here);
# Linux takes it from ulimit, raised in raise_stack below.
LINK=""
[ "$OS" = "Darwin" ] && LINK="-Wl,-stack_size,0x20000000"

raise_stack() {
    ulimit -s unlimited 2>/dev/null || ulimit -s "$(ulimit -Hs)" 2>/dev/null || true
}

# compile <flags> <output>
compile() {
    # shellcheck disable=SC2086
    "$SCRIPTS/cxx.sh" $1 $LINK -o "$2" "$FILE"
}

# Print a line for crashes, so a segfault is never mistaken for empty output.
report() {
    local rc=$1 sig name
    [ "$rc" -eq 0 ] && return 0
    if [ "$rc" -gt 128 ]; then
        sig=$((rc - 128))
        name="$(kill -l "$sig" 2>/dev/null || echo "$sig")"
        echo "[cp: killed by SIG$name (exit $rc)]" >&2
    else
        echo "[cp: exit code $rc]" >&2
    fi
    return "$rc"
}

need_input() {
    if [ ! -f input.txt ]; then
        : > input.txt
        echo "[cp: created empty input.txt]" >&2
    fi
}

# timed <cmd...>: stdin from input.txt, then a footer with time and peak memory.
timed() {
    if command -v python3 > /dev/null 2>&1; then
        python3 -c '
import resource, subprocess, sys, time
with open("input.txt", "rb") as fin:
    start = time.time()
    rc = subprocess.call(sys.argv[1:], stdin=fin)
    wall = time.time() - start
ru = resource.getrusage(resource.RUSAGE_CHILDREN)
rss = ru.ru_maxrss / (1048576.0 if sys.platform == "darwin" else 1024.0)
sys.stdout.flush()
sys.stderr.write("\n--- %.3f s wall | %.3f s cpu | %.1f MB peak\n" % (wall, ru.ru_utime + ru.ru_stime, rss))
sys.exit(rc if rc >= 0 else 128 - rc)
' "$@"
    else
        local TIMEFORMAT=$'\n--- %R s wall | %U s cpu'
        time "$@" < input.txt
    fi
}

# in_terminal <cmd...>: run in a new terminal window, in this folder, and wait for Enter.
in_terminal() {
    local launcher q=""
    launcher="$(mktemp "${TMPDIR:-/tmp}/cp-run.XXXXXX")" || exit 1
    for a in "$@"; do q="$q $(printf '%q' "$a")"; done
    {
        echo '#!/usr/bin/env bash'
        printf 'cd %q || exit 1\n' "$PWD"
        echo 'ulimit -s unlimited 2>/dev/null || ulimit -s "$(ulimit -Hs)" 2>/dev/null'
        echo "$q"
        echo 'echo; echo "[exit $?] press Enter to close"; read -r _'
        printf 'rm -f %q\n' "$launcher"
    } > "$launcher"
    chmod +x "$launcher"

    if [ "$OS" = "Darwin" ]; then
        open -a "${CP_TERMINAL:-Terminal}" "$launcher"
        return
    fi
    local t
    for t in "${CP_TERMINAL:-}" "${TERMINAL:-}" x-terminal-emulator gnome-terminal ptyxis kgx konsole \
             xfce4-terminal kitty alacritty foot wezterm xterm; do
        [ -n "$t" ] && command -v "$t" > /dev/null 2>&1 || continue
        case "$(basename "$t")" in
            gnome-terminal | ptyxis) "$t" -- bash "$launcher" ;;
            kitty | foot)            "$t" bash "$launcher" ;;
            wezterm)                 "$t" start -- bash "$launcher" ;;
            *)                       "$t" -e bash "$launcher" ;;
        esac
        return
    done
    rm -f "$launcher"
    echo "cp: no terminal emulator found. Set CP_TERMINAL to yours (e.g. export CP_TERMINAL=kitty)." >&2
    return 1
}

case "$EXT" in
    cpp | cc | cxx)
        case "$MODE" in
            compile)
                compile "$RELEASE" "${OUT:-$BASE.bin}"
                ;;
            run)
                compile "$RELEASE" "$BASE.bin" || exit 1
                need_input; raise_stack
                "./$BASE.bin" < input.txt
                report $?
                ;;
            debug)
                compile "$DEBUG" "$BASE-dbg.bin" || exit 1
                need_input; raise_stack
                ASAN_OPTIONS="${ASAN_OPTIONS:-detect_leaks=0}" \
                    UBSAN_OPTIONS="${UBSAN_OPTIONS:-print_stacktrace=1}" "./$BASE-dbg.bin" < input.txt
                report $?
                ;;
            timed)
                compile "$RELEASE" "$BASE.bin" || exit 1
                need_input; raise_stack
                timed "./$BASE.bin"
                report $?
                ;;
            output)
                compile "$RELEASE" "$BASE.bin" || exit 1
                need_input; raise_stack
                "./$BASE.bin" < input.txt > output.txt
                rc=$?
                cat output.txt
                report $rc
                ;;
            terminal)
                compile "$RELEASE" "$BASE.bin" || exit 1
                in_terminal "./$BASE.bin"
                ;;
            *)
                echo "cp: unknown mode '$MODE'" >&2
                exit 2
                ;;
        esac
        ;;
    py)
        case "$MODE" in
            compile)  python3 -m py_compile "$FILE" ;;
            terminal) in_terminal python3 "$FILE" ;;
            timed)    need_input; timed python3 "$FILE"; report $? ;;
            output)
                need_input
                python3 "$FILE" < input.txt > output.txt
                rc=$?
                cat output.txt
                report $rc
                ;;
            *)        need_input; python3 "$FILE" < input.txt; report $? ;;
        esac
        ;;
    *)
        echo "cp: don't know how to run .$EXT files" >&2
        exit 2
        ;;
esac
