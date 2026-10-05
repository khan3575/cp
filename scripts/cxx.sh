#!/usr/bin/env bash
# Runs the real GNU g++ with the given arguments, wherever it lives on this machine.
#   macOS: plain `g++` is Apple Clang (no <bits/stdc++.h>), so pick the newest Homebrew g++-N.
#   Linux: plain `g++`.
# Override with CP_CXX=/path/to/g++ (or an already-set $CXX).
# `cxx.sh --which` prints the compiler it would use.
set -u

find_cxx() {
    if [ -n "${CP_CXX:-${CXX:-}}" ]; then
        echo "${CP_CXX:-$CXX}"
        return 0
    fi
    if [ "$(uname -s)" != "Darwin" ]; then
        command -v g++ && return 0
        echo "cp: g++ not found. Install it: sudo apt install g++  (or: sudo dnf install gcc-c++ / sudo pacman -S gcc)" >&2
        return 1
    fi

    local best="" bestv=0 dir c v
    local IFS=:
    for dir in /opt/homebrew/bin /usr/local/bin $PATH; do
        for c in "$dir"/g++-[0-9]*; do
            [ -x "$c" ] || continue
            v="${c##*/g++-}"
            case "$v" in *[!0-9]*) continue ;; esac
            if [ "$v" -gt "$bestv" ]; then
                best="$c"
                bestv="$v"
            fi
        done
    done
    if [ -z "$best" ]; then
        echo "cp: no Homebrew GCC found (plain g++ on macOS is Apple Clang). Install it: brew install gcc" >&2
        return 1
    fi
    echo "$best"
}

CXX="$(find_cxx)" || exit 127
if [ "${1:-}" = "--which" ]; then
    echo "$CXX"
    exit 0
fi
exec "$CXX" "$@"
