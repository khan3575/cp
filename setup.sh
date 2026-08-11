#!/usr/bin/env bash
# Links this repo's sublime/User into Sublime Text's config directory.
# Usage:  ./setup.sh              -> auto-detect installed Sublime Text
#         ./setup.sh /path/to/Data -> portable install (folder next to sublime_text)
set -euo pipefail
REPO="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

if [ $# -ge 1 ]; then
    TARGET="$1/Packages/User"
else
    case "$(uname -s)" in
        Darwin) TARGET="$HOME/Library/Application Support/Sublime Text/Packages/User" ;;
        *)      TARGET="$HOME/.config/sublime-text/Packages/User" ;;
    esac
fi

mkdir -p "$(dirname "$TARGET")"

if [ -e "$TARGET" ] && [ ! -L "$TARGET" ]; then
    BAK="$TARGET.backup.$(date +%Y%m%d%H%M%S)"
    echo "Backing up existing config -> $BAK"
    mv "$TARGET" "$BAK"
fi
rm -f "$TARGET"

ln -s "$REPO/sublime/User" "$TARGET"
chmod +x "$REPO/scripts/"*.sh 2>/dev/null || true
echo "Linked: $TARGET  ->  $REPO/sublime/User"
echo "Now open Sublime Text and use Project > Open Project... -> $REPO/cp.sublime-project"
