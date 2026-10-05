"""
cp_tools.py — small helper commands for competitive programming.

Commands:
    cp_new_problem   Create <Name>.cpp from templates/main.cpp in the current folder.
    cp_new_contest   Create contests/<path>/{A..}.cpp in one shot.
    cp_toggle_input  Open/close input.txt next to the current file, in a side column.
    cp_format        Format the current file with clang-format, using the repo's .clang-format.

Drop this in Packages/User/ (the setup script symlinks it there).
"""

import os
import shutil
import subprocess
import sublime
import sublime_plugin

FALLBACK = (
    "#include <bits/stdc++.h>\n"
    "using namespace std;\n\n"
    "int main() {\n"
    "    ios::sync_with_stdio(false);\n"
    "    cin.tie(nullptr);\n\n"
    "    return 0;\n"
    "}\n"
)


def _repo_root(window):
    # This file is <repo>/sublime/User/cp_tools.py, reached through the Packages/User symlink,
    # so the repo is found no matter which folder or project the window has open.
    root = os.path.dirname(os.path.dirname(os.path.dirname(os.path.realpath(__file__))))
    if os.path.isdir(os.path.join(root, "templates")):
        return root
    folders = window.folders()
    return folders[0] if folders else None


def _find_tool(name):
    # GUI-launched Sublime may not have Homebrew's bin on PATH.
    found = shutil.which(name)
    if found:
        return found
    for folder in ("/opt/homebrew/bin", "/usr/local/bin", "/usr/bin"):
        path = os.path.join(folder, name)
        if os.path.isfile(path) and os.access(path, os.X_OK):
            return path
    return None


def _template(window, name="main.cpp"):
    root = _repo_root(window)
    candidates = []
    if root:
        candidates.append(os.path.join(root, "templates", name))
    candidates.append(os.path.join(sublime.packages_path(), "User", name))
    for path in candidates:
        if os.path.isfile(path):
            try:
                with open(path, "r", encoding="utf-8") as fh:
                    return fh.read()
            except OSError:
                pass
    return FALLBACK


def _write_if_absent(path, content=""):
    if os.path.exists(path):
        return False
    os.makedirs(os.path.dirname(path), exist_ok=True)
    with open(path, "w", encoding="utf-8", newline="\n") as fh:
        fh.write(content)
    return True


def _current_dir(window):
    view = window.active_view()
    if view and view.file_name():
        return os.path.dirname(view.file_name())
    return _repo_root(window)


class CpNewProblemCommand(sublime_plugin.WindowCommand):
    """Create a single problem file. The prompt shows the folder, so it can be changed."""

    def run(self):
        # Start from the active file's folder when that is inside practice/ or contests/;
        # from anywhere else (library/, templates/, no file open) start at practice/.
        root = _repo_root(self.window)
        folder = _current_dir(self.window)
        initial = ""
        if root:
            initial = "practice/"
            if folder:
                rel = os.path.relpath(os.path.realpath(folder), os.path.realpath(root))
                parts = rel.split(os.sep)
                if parts[0] in ("practice", "contests"):
                    initial = "/".join(parts) + "/"
        self.window.show_input_panel(
            "New problem, path inside cp/ (e.g. practice/dp/1633-dice-combinations):",
            initial, self.on_done, None, None
        )

    def on_done(self, name):
        name = name.strip()
        if not name or name.endswith("/"):
            return
        if not name.endswith((".cpp", ".py")):
            name += ".cpp"

        root = _repo_root(self.window)
        if "/" in name and root:
            path = os.path.join(root, *[p for p in name.split("/") if p])
        else:
            folder = _current_dir(self.window)
            if not folder:
                sublime.error_message("cp_tools: open the CP project folder first.")
                return
            path = os.path.join(folder, name)

        folder = os.path.dirname(path)
        body = _template(self.window) if path.endswith(".cpp") else ""
        created = _write_if_absent(path, body)
        _write_if_absent(os.path.join(folder, "input.txt"), "")

        self.window.open_file(path)
        shown = os.path.relpath(path, root) if root else path
        sublime.status_message(
            "cp: {} {}".format("created" if created else "opened", shown)
        )


class CpNewContestCommand(sublime_plugin.WindowCommand):
    """Scaffold contests/<judge>/<contest>/ with one file per problem."""

    def run(self):
        self.window.show_input_panel(
            "Contest path under contests/ (e.g. codeforces/2001-div2):",
            "codeforces/",
            self.ask_problems,
            None,
            None,
        )

    def ask_problems(self, rel):
        rel = rel.strip().strip("/")
        if not rel:
            return
        self.rel = rel
        self.window.show_input_panel(
            "Problems (space separated):", "A B C D E F", self.on_done, None, None
        )

    def on_done(self, problems):
        root = _repo_root(self.window)
        if not root:
            sublime.error_message("cp_tools: open cp.sublime-project first.")
            return

        folder = os.path.join(root, "contests", *self.rel.split("/"))
        os.makedirs(folder, exist_ok=True)

        body = _template(self.window)
        names = [p for p in problems.split() if p]
        first = None
        for p in names:
            path = os.path.join(folder, p + ".cpp")
            _write_if_absent(path, body)
            if first is None:
                first = path

        _write_if_absent(os.path.join(folder, "input.txt"), "")
        _write_if_absent(
            os.path.join(folder, "notes.md"),
            "# {}\n\n| Problem | Verdict | Idea |\n|---|---|---|\n".format(self.rel)
            + "".join("| {} |  |  |\n".format(p) for p in names),
        )

        if first:
            self.window.open_file(first)
        sublime.status_message("cp: scaffolded {} ({} problems)".format(self.rel, len(names)))


class CpToggleInputCommand(sublime_plugin.WindowCommand):
    """Show input.txt beside the solution; press again to close it."""

    def run(self):
        folder = _current_dir(self.window)
        if not folder:
            return
        path = os.path.join(folder, "input.txt")
        _write_if_absent(path)

        for view in self.window.views():
            if view.file_name() == path:
                view.close()
                if self.window.num_groups() > 1 and not self.window.views_in_group(1):
                    self.window.run_command("set_layout", {
                        "cols": [0.0, 1.0], "rows": [0.0, 1.0], "cells": [[0, 0, 1, 1]],
                    })
                return

        if self.window.num_groups() < 2:
            self.window.run_command("set_layout", {
                "cols": [0.0, 0.68, 1.0],
                "rows": [0.0, 1.0],
                "cells": [[0, 0, 1, 1], [1, 0, 2, 1]],
            })
        self.window.focus_group(1)
        self.window.open_file(path)


class CpFormatCommand(sublime_plugin.TextCommand):
    """Format the whole buffer with clang-format (style from the nearest .clang-format)."""

    def run(self, edit):
        exe = _find_tool("clang-format")
        if not exe:
            sublime.error_message(
                "cp_tools: clang-format not found.\n\n"
                "macOS:  brew install clang-format\n"
                "Linux:  sudo apt install clang-format"
            )
            return

        view = self.view
        name = view.file_name()
        if not name:
            name = os.path.join(_repo_root(view.window()) or os.getcwd(), "untitled.cpp")
        region = sublime.Region(0, view.size())
        source = view.substr(region)
        try:
            proc = subprocess.Popen(
                [exe, "--style=file", "--assume-filename=" + name],
                stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.PIPE,
                cwd=os.path.dirname(name),
            )
            out, err = proc.communicate(source.encode("utf-8"))
        except OSError as exc:
            sublime.error_message("cp_tools: could not run clang-format: {}".format(exc))
            return
        if proc.returncode != 0:
            sublime.error_message("cp_tools: clang-format failed:\n\n" + err.decode("utf-8", "replace"))
            return

        formatted = out.decode("utf-8")
        if formatted == source:
            sublime.status_message("cp: already formatted")
            return
        carets = [(r.a, r.b) for r in view.sel()]
        viewport = view.viewport_position()
        view.replace(edit, region, formatted)
        view.sel().clear()
        size = view.size()
        for a, b in carets:
            view.sel().add(sublime.Region(min(a, size), min(b, size)))
        view.set_viewport_position(viewport, False)
        sublime.status_message("cp: formatted")

    def is_enabled(self):
        return self.view.match_selector(0, "source.c, source.c++")
