#!/usr/bin/env bash
# Stress test: compare sol vs brute on random tests until they disagree.
# Usage: ../../scripts/stress.sh A brute [iterations]
set -uo pipefail
SOL="${1:-A}"; BRUTE="${2:-brute}"; ITER="${3:-1000}"
GEN="${GEN:-gen.py}"
FLAGS="-std=gnu++20 -O2 -DLOCAL"

g++ $FLAGS -o "$SOL.bin"   "$SOL.cpp"   || exit 1
g++ $FLAGS -o "$BRUTE.bin" "$BRUTE.cpp" || exit 1

for i in $(seq 1 "$ITER"); do
    python3 "$GEN" "$i" > _in.txt
    ./"$SOL.bin"   < _in.txt > _out1.txt
    ./"$BRUTE.bin" < _in.txt > _out2.txt
    if ! diff -qwB _out1.txt _out2.txt > /dev/null; then
        echo "MISMATCH on test $i"
        echo "--- input ---";  cat _in.txt
        echo "--- $SOL ---";   cat _out1.txt
        echo "--- $BRUTE ---"; cat _out2.txt
        cp _in.txt input.txt
        exit 1
    fi
    printf "\rpassed %d/%d" "$i" "$ITER"
done
echo -e "\nAll $ITER tests passed."
