#!/usr/bin/env bash
# Stress test: compare sol vs brute on random tests until they disagree.
# Run from the problem's folder, which must hold <sol>.cpp, <brute>.cpp and gen.py:
#   ../../scripts/stress.sh A brute [iterations]       (from practice/<topic>/)
#   ../../../scripts/stress.sh A brute [iterations]    (from contests/<judge>/<contest>/)
set -uo pipefail
SOL="${1:-A}"; BRUTE="${2:-brute}"; ITER="${3:-1000}"
SOL="${SOL%.cpp}"; BRUTE="${BRUTE%.cpp}"
GEN="${GEN:-gen.py}"
SCRIPTS="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd -P)"

for f in "$SOL.cpp" "$BRUTE.cpp" "$GEN"; do
    if [ ! -f "$f" ]; then
        echo "stress: $f not found in $PWD (copy brute.cpp and gen.py from templates/)" >&2
        exit 2
    fi
done

"$SCRIPTS/run.sh" compile "$SOL.cpp"   || exit 1
"$SCRIPTS/run.sh" compile "$BRUTE.cpp" || exit 1
ulimit -s unlimited 2>/dev/null || ulimit -s "$(ulimit -Hs)" 2>/dev/null || true

fail() {
    echo
    echo "$1 on test $i"
    echo "--- input ---";  cat _in.txt
    echo "--- $SOL ---";   cat _out1.txt
    echo "--- $BRUTE ---"; cat _out2.txt
    cp _in.txt input.txt
    echo "(saved to input.txt)"
    exit 1
}

for i in $(seq 1 "$ITER"); do
    python3 "$GEN" "$i" > _in.txt || { echo "stress: $GEN failed on seed $i" >&2; exit 2; }
    : > _out1.txt; : > _out2.txt
    ./"$BRUTE.bin" < _in.txt > _out2.txt || { echo; echo "stress: $BRUTE crashed on test $i" >&2; cp _in.txt input.txt; exit 2; }
    ./"$SOL.bin"   < _in.txt > _out1.txt || fail "CRASH (exit $?)"
    diff -qwB _out1.txt _out2.txt > /dev/null || fail "MISMATCH"
    printf "\rpassed %d/%d" "$i" "$ITER"
done
echo
echo "All $ITER tests passed."
