# cp — competitive programming repo

One git repo holding the Sublime Text setup, templates, an algorithm library, and every solved
problem. Cloning it and running one script reproduces the whole editor on either machine.

## Machines: this repo is used on Linux AND macOS

| | Linux (primary) | macOS |
|---|---|---|
| OS | Ubuntu 26.04 | — |
| Compiler | `/usr/bin/g++` (GCC 15.2.0, real GCC) | `/opt/homebrew/bin/g++-16` (Homebrew GCC) |
| Why the absolute path | not needed, plain `g++` is correct | plain `g++` is an Apple Clang shim with no `<bits/stdc++.h>`, and a Dock-launched app does not inherit shell `PATH` |
| Sublime install | snap `sublime-text` (classic confinement) | normal app |
| `Packages/User` | `~/.config/sublime-text/Packages/User` | `~/Library/Application Support/Sublime Text/Packages/User` |
| Symlinked to | `cp/sublime/User` via `./setup.sh` | same |

The macOS compiler path is pinned in **three** places. Change all three together:
`sublime/User/CP.sublime-build` (`osx` branches), `sublime/User/FastOlympicCoding (OSX).sublime-settings`,
and the `Darwin` branch at the top of `scripts/stress.sh`.

## Layout

```
cp/
├── templates/        main.cpp, interactive.cpp, brute.cpp, gen.py
├── library/          ds/ graph/ math/ string/ — one algorithm per file
├── contests/<judge>/<contest-id>-<short-name>/
│                     A.cpp B.cpp … + input.txt + notes.md
│                     judges: codeforces/ codechef/ atcoder/ cses/
├── practice/<topic>/<problem-id>-<slug>.cpp
├── scripts/          stress.sh, stress.bat
└── sublime/User/     the tracked Sublime config, symlinked into place
```

## Rules

- **`<contest-id>` is the id in the judge's own URL, not the round number.** Codeforces Round 1121
  (Div. 2) is at `codeforces.com/contest/2264`, so the folder is `2264-div2`. On CodeChef it is the
  contest code (`START251C-div3`).
- **Every solution file starts with two comment lines:** line 1 the problem URL, line 2 a
  one-sentence idea. `templates/*.cpp` ship with blank `// URL:` / `// idea:` lines.
- Practice folders are **topics**, never dates. Practice files are `<problem-id>-<slug>.cpp`,
  lowercase and hyphenated. Two solutions to one problem get a suffix describing the approach
  (`dp_c-vacation.cpp` and `dp_c-vacation-recursive.cpp`).
- One file per problem. Never a shared `a.cpp` scratch file.
- Nothing enters `library/` until it has passed on a real judge, with the verification link in the
  header. Library files paste in with zero edits: no `#include`, no `using namespace`.
- Build flags are `-std=gnu++20 -O2 -DLOCAL`. Unix binaries are named `.bin` so `.gitignore` catches
  them; Windows uses `.exe`.
- `dbg(...)` prints to **stderr** and compiles to nothing without `-DLOCAL`, so it is safe to leave
  in submitted code.

## FastOlympicCoding

Two tracked files, one per platform — `FastOlympicCoding (Linux).sublime-settings` and `(OSX)`.
There is deliberately no plain-name file, because the compiler path differs per machine and Sublime
merges a platform-suffixed file into the plain name automatically.

**FOC compiles without `-DLOCAL`, on purpose. Do not "fix" this by adding it.** FOC runs the
program with stderr merged into stdout and compares that combined text against the expected answer,
so a single `dbg()` line turns a correct answer into a wrong one. The trap is that it looks like a
wrong-answer bug in your solution, not a build-flag problem. In the installed package:

```
Modules/ProcessManager.py:104-106   run_file(): stderr=subprocess.STDOUT, cwd = source dir
test_manager.py:97-103              is_correct_answer() compares the combined output string
```

Demonstrated on this machine with one source file and the same FOC command:

```
without -DLOCAL:   42                 <- matches expected output
with    -DLOCAL:   [n] = 5
                   42                 <- compares as WRONG
```

FOC behaves like the judge. `-DLOCAL` / `dbg()` / `input.txt` live in `CP.sublime-build` (`F5`)
instead. The lint command also needs `-fsyntax-only`, because FOC runs it with **no** working
directory set (`Cpp_Intellij_Sense.py:155,162`) and would otherwise drop a stray `a.out` wherever
Sublime happened to start.

## Judge quirks

**CodeChef: file letter does not equal contest position.** A Starters contest's API listing includes
problems that are not in your division's scored set, marked `category_name: "unscored"`. In
START251C, `CHKEV` (Check Even) is unscored, so the Division 3 set actually starts at the second
entry. Mapping `A.cpp` to the first problem in the listing shifts every file by one and gives every
solution the wrong URL.

The scored Division 3 set for START251C, which is what `A.cpp`–`F.cpp` correspond to:

```
1  BUSROW      Bus Rows              A.cpp
2  MUL123      Make Multiple         B.cpp
3  CHOCGM      Chocolate Game        C.cpp
4  BINSPLT     Binary Split          D.cpp
5  MAKEDISTK   Make Distinct         E.cpp
6  BINSPLTHD   Binary Split (Hard)   F.cpp (never attempted, deleted)
```

Before trusting any letter-to-problem mapping, check the code against the problem name, and filter
the API listing on `category_name == "main"`:

```bash
curl -s "https://www.codechef.com/api/contests/<CODE>" | python3 -c "
import json,sys
for v in json.load(sys.stdin)['problems'].values():
    print(v['category_name'], v['code'], v['name'])"
```

**Codeforces: contest id is not the round number.** Round 1121 (Div. 2) is contest `2264`. Get the
id from the URL, never from the round title. Folder names use the id.

## Handles

- Codeforces `khan3575` — 1079, peak 1297 (Feb 2025), 88 rated contests.
- CodeChef `khan3575` — 1445 (2★), peak 1521, 22 contests.

## How to explain things to me

Literal terms, no analogies. ASCII trees for any nested structure. For a practice problem, give a
hint first and the full solution only when I ask.

## Known issues

Fixed:

- [x] FastOlympicCoding had no settings file at all, so it ran on package defaults
      (`-std=gnu++11`, no `-O2`, no output extension). Replaced with the two platform files.
- [x] `scripts/stress.sh` called bare `g++`, which breaks on macOS. Now picks by `uname`, and
      honours an existing `$CXX`.
- [x] `.gitignore` missed FOC test data (`*:tests`) and Package Control per-machine state.
- [x] Templates had no `// URL:` / `// idea:` header lines, so new files inherited none.
- [x] `library/math/modpow.cpp` had no verification reference.

Open:

- [ ] `library/math/modpow.cpp` is unverified — submit to CSES 1095 and replace the `UNVERIFIED`
      line with the accepted link.
- [ ] `library/ds/dsu.cpp` claims "CF 1213G" but there is **no submission to 1213G on this
      account**, of any verdict. The other two library files cite CSES tasks, which have no public
      submission URLs, so none of the three can carry a real verification link yet.
- [ ] `practice/dp/dp_e.cpp` (Knapsack 2) is abandoned mid-write: it reads input, sizes the dp
      array, and never computes or prints an answer.
- [ ] `contests/codeforces/2264-div2/B.cpp` is a complete solution that was **never submitted** —
      the account has exactly one submission to contest 2264 (problem A, `OK`).
- [ ] The 17 `// idea: TODO` lines still need real one-sentence ideas.

When switching to the Mac:

- [ ] `git pull`
- [ ] `ls -l /opt/homebrew/bin/g++-16` — confirm the pinned path still exists
- [ ] run `./setup.sh` if `~/Library/Application Support/Sublime Text/Packages/User` is not a
      symlink to `cp/sublime/User`
- [ ] re-run the structure audit
