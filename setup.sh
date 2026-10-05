#!/usr/bin/env bash
# One-time (and safe to re-run) setup for Linux and macOS:
#   1. links this repo's sublime/User into Sublime Text's config directory
#   2. checks the toolchain and says exactly what to install if something is missing
# Usage:  ./setup.sh               -> auto-detect installed Sublime Text
#         ./setup.sh /path/to/Data -> portable install (folder next to sublime_text)
#         ./setup.sh --check       -> only run the toolchain check
set -uo pipefail
REPO="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd -P)"
OS="$(uname -s)"

ok()   { printf '  ok    %s\n' "$1"; }
bad()  { printf '  MISS  %s\n        -> %s\n' "$1" "$2"; MISSING=$((MISSING + 1)); }
note() { printf '  note  %s\n        -> %s\n' "$1" "$2"; }

pkg_hint() { # pkg_hint <brew> <apt> <dnf> <pacman>
    if [ "$OS" = "Darwin" ]; then
        echo "brew install $1"
    elif command -v apt-get > /dev/null 2>&1; then
        echo "sudo apt install $2"
    elif command -v dnf > /dev/null 2>&1; then
        echo "sudo dnf install $3"
    elif command -v pacman > /dev/null 2>&1; then
        echo "sudo pacman -S $4"
    else
        echo "install $2 with your package manager"
    fi
}

check() {
    MISSING=0
    echo "Toolchain:"
    local cxx tmp
    tmp="$(mktemp -d "${TMPDIR:-/tmp}/cp-setup.XXXXXX")"

    if cxx="$("$REPO/scripts/cxx.sh" --which 2> /dev/null)"; then
        if "$cxx" -std=gnu++20 -DLOCAL -o "$tmp/t" "$REPO/templates/main.cpp" > "$tmp/log" 2>&1; then
            ok "C++ compiler: $cxx ($("$cxx" -dumpfullversion 2> /dev/null || "$cxx" -dumpversion))"
        else
            bad "C++ compiler $cxx cannot build templates/main.cpp (needs GCC 10+ for C++20)" \
                "$(pkg_hint gcc g++ gcc-c++ gcc)"
        fi
        printf 'int main(){int a[2]={0,0};return a[0];}\n' > "$tmp/s.cpp"
        if "$cxx" -fsanitize=address,undefined -o "$tmp/s" "$tmp/s.cpp" > "$tmp/log" 2>&1; then
            ok "sanitizers (debug build)"
        else
            bad "sanitizer runtime (the debug build needs it)" "$(pkg_hint gcc libasan8 'libasan libubsan' gcc)"
        fi
    else
        bad "C++ compiler (GNU g++)" "$(pkg_hint gcc g++ gcc-c++ gcc)"
    fi

    if command -v python3 > /dev/null 2>&1; then
        ok "python3 ($(python3 -c 'import platform; print(platform.python_version())'))"
    else
        bad "python3 (stress-test generator, timed runs)" "$(pkg_hint python python3 python3 python)"
    fi

    if command -v clang-format > /dev/null 2>&1 || [ -x /opt/homebrew/bin/clang-format ] || [ -x /usr/local/bin/clang-format ]; then
        ok "clang-format"
    else
        note "clang-format not installed (only the format key needs it)" \
            "$(pkg_hint clang-format clang-format clang-tools-extra clang)"
    fi
    rm -rf "$tmp"

    if [ "$MISSING" -gt 0 ]; then
        echo "$MISSING required tool(s) missing. Install them, then run ./setup.sh --check"
        return 1
    fi
    echo "Everything required is installed."
}

if [ "${1:-}" = "--check" ]; then
    check
    exit $?
fi

# ---- 1. link sublime/User ----
if [ $# -ge 1 ]; then
    TARGET="$1/Packages/User"
elif [ "$OS" = "Darwin" ]; then
    TARGET="$HOME/Library/Application Support/Sublime Text/Packages/User"
else
    CONFIG="${XDG_CONFIG_HOME:-$HOME/.config}"
    BASE="$CONFIG/sublime-text"
    for d in "$CONFIG/sublime-text" "$CONFIG/sublime-text-3" \
             "$HOME/.var/app/com.sublimetext.three/config/sublime-text"; do
        if [ -d "$d" ]; then
            BASE="$d"
            break
        fi
    done
    TARGET="$BASE/Packages/User"
fi

echo "Sublime Text config:"
chmod +x "$REPO/setup.sh" "$REPO/scripts/"*.sh 2> /dev/null || true
mkdir -p "$(dirname "$TARGET")"
if [ -L "$TARGET" ] && [ "$(cd "$TARGET" 2> /dev/null && pwd -P)" = "$REPO/sublime/User" ]; then
    ok "already linked: $TARGET"
else
    if [ -e "$TARGET" ] && [ ! -L "$TARGET" ]; then
        BAK="$TARGET.backup.$(date +%Y%m%d%H%M%S)"
        echo "  Backing up existing config -> $BAK"
        mv "$TARGET" "$BAK"
    fi
    rm -f "$TARGET"
    ln -s "$REPO/sublime/User" "$TARGET"
    ok "linked: $TARGET -> $REPO/sublime/User"
fi

# A plain FastOlympicCoding.sublime-settings would apply to every machine and shadow the tracked
# per-platform files, so make sure none is lying around.
rm -f "$REPO/sublime/User/FastOlympicCoding.sublime-settings"

echo
check
STATUS=$?

echo
echo "Next:"
echo "  1. Open Sublime Text -> Project > Open Project... -> $REPO/cp.sublime-project"
echo "  2. First time on this machine: Tools > Install Package Control. It then installs the"
echo "     packages listed in sublime/User/Package Control.sublime-settings; restart Sublime after."
exit $STATUS
