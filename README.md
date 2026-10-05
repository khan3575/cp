# cp

A single git repository holding a Sublime Text 4 setup for competitive programming (C++20, plus
Python) together with every template, library snippet, and solved problem. The Sublime config
lives inside the repo at `sublime/User/` and is symlinked into Sublime's real config directory, so
cloning the repo and running one setup script reproduces the whole editor on a new machine.

**Linux and macOS are the supported platforms and behave identically**: same keys, same flags,
same scripts. Windows files are kept but are not maintained to the same level (see Known
limitations).

## Quick start (Linux or macOS)

```bash
git clone git@github.com:khan3575/cp.git
cd cp
./setup.sh
```

`setup.sh` is safe to re-run at any time. It:

1. symlinks `sublime/User/` onto Sublime's config directory
   (`~/Library/Application Support/Sublime Text/Packages/User` on macOS,
   `~/.config/sublime-text/Packages/User` on Linux; an existing real folder is moved to
   `User.backup.<timestamp>` first). Pass a path for a portable install: `./setup.sh /path/to/Data`.
2. checks the toolchain and prints the exact install command for anything missing.
   `./setup.sh --check` runs only this step.

Then in Sublime: **Project → Open Project…** → `cp.sublime-project`. On a brand-new machine also
run **Tools → Install Package Control** once; it installs the packages listed in
`sublime/User/Package Control.sublime-settings`.

What you need installed:

| | macOS | Debian / Ubuntu | Fedora | Arch |
|---|---|---|---|---|
| GNU g++ (10 or newer) | `brew install gcc` | `sudo apt install g++` | `sudo dnf install gcc-c++ libasan libubsan` | `sudo pacman -S gcc` |
| python3 | preinstalled / `brew install python` | `sudo apt install python3` | `sudo dnf install python3` | `sudo pacman -S python` |
| clang-format (optional) | `brew install clang-format` | `sudo apt install clang-format` | `sudo dnf install clang-tools-extra` | `sudo pacman -S clang` |

No compiler path is hard-coded anywhere. On macOS plain `g++` is Apple Clang (no
`<bits/stdc++.h>`), so `scripts/cxx.sh` picks the newest Homebrew `g++-N` it can find and keeps
working after `brew upgrade gcc`. On Linux it uses `g++`. Set `CP_CXX=/path/to/g++` to override.

## Keybindings

| Action | Linux | macOS |
|---|---|---|
| Run (release build, stdin from `input.txt`) | `F5` | `F5` or `Cmd+R` |
| Debug build (ASan + UBSan + checked STL) | `F6` | `F6` or `Cmd+Shift+R` |
| Run in a terminal window (interactive problems) | `F7` | `F7` or `Cmd+Alt+R` |
| Run timed (wall time, CPU time, peak memory) | `F8` | `F8` |
| Compile only | `Ctrl+F5` | `Ctrl+F5` |
| Toggle `input.txt` in a side pane | `F9` | `F9` |
| New problem file | `Ctrl+Alt+N` | `Cmd+Alt+N` |
| New contest scaffold | `Ctrl+Alt+C` | `Cmd+Alt+C` |
| Format with clang-format | `Ctrl+Alt+F` | `Ctrl+Alt+F` |

On a Mac laptop the F-keys need `fn` unless "Use F1, F2, etc. keys as standard function keys" is
on in System Settings → Keyboard. "Run (output.txt)" has no key: use **Tools → Build With…**.

## How a build works

Every way of building goes through one script, so there is one place to change a flag:

```
Sublime build system ─┐
                      ├─> scripts/run.sh <mode> <file> ─┐
scripts/stress.sh ────┘                                 ├─> scripts/cxx.sh (finds the compiler)
FastOlympicCoding (macOS) ──────────────────────────────┘
```

`scripts/run.sh <mode> <file>` also works from a shell. Modes: `run`, `debug`, `timed`, `output`,
`compile`, `terminal`. It handles `.cpp` and `.py`.

- **Release** flags: `-std=gnu++20 -O2 -pipe -Wall -Wextra -Wshadow -DLOCAL`.
- **Debug** flags add `-O0 -g -Wconversion -D_GLIBCXX_DEBUG -D_GLIBCXX_ASSERTIONS
  -fsanitize=address,undefined -fno-sanitize-recover=all`. Use it on any WA or RE before reading
  your code: an out-of-bounds index or signed overflow that the release build silently gets wrong
  aborts here with the file and line.
- **Deep recursion works on both systems.** macOS binaries are linked with a 512 MB main-thread
  stack; on Linux the script raises `ulimit -s` before running.
- A crash is never silent: `[cp: killed by SIGSEGV (exit 139)]` or `[cp: exit code 3]` is printed
  after the program's output.
- A missing `input.txt` is created empty instead of failing the build.
- Binaries are `<name>.bin` and `<name>-dbg.bin` beside the source, gitignored.
- The terminal used by `F7` is Terminal.app on macOS and the first of `x-terminal-emulator`,
  `gnome-terminal`, `ptyxis`, `kgx`, `konsole`, `xfce4-terminal`, `kitty`, `alacritty`, `foot`,
  `wezterm`, `xterm` on Linux. Set `CP_TERMINAL` to choose (e.g. `iTerm` or `kitty`).

## File and folder conventions

```
cp/
├── templates/           main.cpp, interactive.cpp, brute.cpp, gen.py
├── library/              ds/, graph/, math/, string/ — one algorithm per file
├── scripts/              run.sh, cxx.sh, stress.sh
├── contests/<judge>/<contest-id>-<name>/   e.g. contests/codeforces/2001-div2/
│   ├── A.cpp, B.cpp, …   one file per problem, from templates/main.cpp
│   ├── input.txt         gitignored scratch stdin
│   └── notes.md          verdict table, generated by the new-contest command
└── practice/<topic>/<problem-id>-<slug>.cpp   e.g. practice/dp/1974-e-money-buying.cpp
```

- **New problem** creates one file from `templates/main.cpp` in the folder of the file you are
  looking at, plus an `input.txt` beside it.
- **New contest** prompts for a path under `contests/` and a space-separated list of problem
  letters, then creates one `.cpp` per letter plus `input.txt` and `notes.md`.
- Both commands find the repo from the plugin's own location, so they work whichever folder or
  project the window has open.
- Contest folder naming: `<judge>/<contest-id>-<short-name>` (`codeforces/2264-div2`).
- **`<contest-id>` is the id in the judge's own URL, not the round number.** Codeforces Round 1121
  (Div. 2) lives at `codeforces.com/contest/2264`, so the folder is `2264-div2`, not `1121-div2`.
  On CodeChef the id is the contest code in the URL (`codechef.com/START251C` → `START251C-div3`).
- Every solution file starts with two comment lines: line 1 the problem URL, line 2 a one-sentence
  idea. `templates/*.cpp` ship with blank `// URL:` / `// idea:` lines so new files inherit them.
- Practice file naming: `<problem-id>-<slug>.cpp`, lowercase, hyphens. Practice folders are topics
  (`practice/dp/`), never dates. CSES and other problem-set practice goes here, not in `contests/`.
- A snippet only enters `library/` after it has passed on a real judge — see `library/README.md`.

## Stress testing

Put `brute.cpp` and `gen.py` (copies of the ones in `templates/`) beside the solution, then from
that folder:

```bash
../../scripts/stress.sh A brute 1000        # from practice/<topic>/
../../../scripts/stress.sh A brute 1000     # from contests/<judge>/<contest>/
```

It stops on the first mismatch **or crash**, prints the input and both outputs, and saves the
failing input to `input.txt`, ready for `F6`.

## Debug printing

`dbg(a, b, c)` (from `templates/main.cpp`) prints `[a, b, c] = <va> | <vb> | <vc>` to **stderr** and
compiles to nothing when `LOCAL` is undefined — no judge defines it, so it is safe to leave in
submitted code. Handles scalars, strings, pairs, and any container (including `vector<bool>`),
nested arbitrarily.

The alias and debug block of the template sits between `// clang-format off` and
`// clang-format on`, so formatting a solution does not unfold it.

## FastOlympicCoding: two platform files, and no `-DLOCAL`

Sublime merges a platform-suffixed settings file into the plain name, so this repo tracks two
files and each machine reads only its own:

```
sublime/User/FastOlympicCoding (Linux).sublime-settings    -> /usr/bin/g++
sublime/User/FastOlympicCoding (OSX).sublime-settings      -> scripts/cxx.sh (newest Homebrew g++-N)
```

There is deliberately **no** plain `FastOlympicCoding.sublime-settings`: it would apply to both
machines at once. `setup.sh` deletes one if it finds it, and `.gitignore` keeps it out of git.
Both files hold absolute paths into the repo (`~/Desktop/cp` on each machine), so edit them if the
repo moves.

**FOC compiles without `-DLOCAL`, on purpose.** FOC runs the program with stderr merged into stdout
and compares that combined text against the expected answer, so a `dbg()` line would turn a correct
answer into a wrong one. FOC runs behave like the judge; `-DLOCAL`, `dbg()` and `input.txt` belong
to the Sublime build (`F5`). The lint command uses `-fsyntax-only` because FOC runs it with no
working directory set and would otherwise drop a stray `a.out`.

Test data is saved beside the source as `<file>.cpp:tests`, which `.gitignore` excludes.

## Known limitations

- **Windows is not maintained.** `setup.bat`, `scripts/stress.bat` and the `windows` entries of
  the build files are the original inline `g++` commands. They were never run and do not go
  through `run.sh`.
- **Files created from the template before this change** still carry the old `dbg` helper, which
  fails to compile on `dbg(some_vector_of_bool)`. New files are fine.
- **Sanitizer output on current macOS** includes a harmless
  `WARN: Invalid dyld module map detected` line from GCC's runtime. The error report above it is
  correct.
- **Flatpak Sublime on Linux** is linked by `setup.sh`, but its sandbox may not see the host
  compiler. Use the `.deb`/`.rpm`/tarball build.
- **The Competitive Companion browser extension** is not part of this repo.
- `sublime/User/cpt.sublime-snippet` (`cpt` trigger) is a legacy template with a weaker
  single-argument `dbg`. Prefer the new-problem command.
- Sublime Text 4 is assumed. `cp_tools.py` is written to run on both of its plugin hosts.

## Tuning

- Compiler flags: `scripts/run.sh` only.
- FastOlympicCoding: the two `FastOlympicCoding (Linux|OSX).sublime-settings` files.
- Formatting style: `.clang-format` (4 spaces, 100 columns), matching `tab_size` in
  `sublime/User/Preferences.sublime-settings`.
- Font: no `font_face` is set, so each OS uses its own default monospace font. Set one in
  `Preferences.sublime-settings` only if it is installed on every machine you use.
