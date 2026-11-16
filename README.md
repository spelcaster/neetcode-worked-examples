# neetcode

Workspace for working through [neetcode.io](https://neetcode.io) problems:
a worked example per problem, plus solutions in whichever language fits.

## Passing a string to a program's stdin in Bash

When testing a solution that reads from stdin (common for LeetCode/neetcode-style
problems), there are three common ways to feed it input without creating a file:

### `echo` with a pipe

Pipes the output of `echo` into the program's stdin. Simple, but adds a
trailing newline and mangles multi-line input unless you use `echo -e`.

```bash
echo "()[]{}" | python3 solution.py
```

### Here-string (`<<<`)

Feeds a single string directly to stdin. Shorter than `echo | ...` and
doesn't spawn an extra process.

```bash
python3 solution.py <<< "()[]{}"
```

### Here-doc (`<<`)

Feeds multiple lines to stdin until a terminator token. Useful when a
problem's input spans several lines (e.g. an array on one line, a target
value on the next).

```bash
python3 solution.py <<EOF
[2,7,11,15]
9
EOF
```

#### Example: testing a LeetCode-style solution

For a solution that reads a bracket string and prints `true`/`false`:

```bash
# quick one-off checks
echo "()[]{}" | python3 solution.py      # -> true
python3 solution.py <<< "(]"             # -> false

# multi-line input (e.g. Two Sum: array, then target)
python3 solution.py <<EOF
[2,7,11,15]
9
EOF
```

## Per-language commands

Each problem directory holds one subdirectory per language
(`<problem-slug>/python/`, `<problem-slug>/cpp/`, ...). Run these from
inside that subdirectory.

### Python (`uv` / pytest)

```bash
uv sync                      # install deps into .venv (first time / after editing pyproject.toml)

uv run python src/solution.py   # run the solution file directly (if it has a __main__)
echo "()[]{}" | uv run python src/solution.py   # ...with stdin input

uv run pytest                # run the test suite
uv run pytest -v             # verbose, one line per test case
uv run pytest -k some_case   # run a single test by name/substring
```

### C++ (CMake / ctest)

```bash
cmake -S . -B build          # configure (first time / after editing CMakeLists.txt)
cmake --build build          # build everything (main + tests)

./build/main                 # run the interactive runner
echo "()[]{}" | ./build/main # ...with stdin input

./build/tests                # run the assert-based test suite directly
ctest --test-dir build       # ...or via ctest
ctest --test-dir build -V    # ...verbose, shows each assertion's output
```

`CMakeLists.txt` defaults `CMAKE_BUILD_TYPE` to `Debug` when none is set,
so a plain `cmake -S . -B build` already builds with debug symbols
(`-g`) — no separate debug build is needed for the Debug section below.

## Debugging

### C++ (`cgdb`)

The default `Debug` build type already includes `-g`, so no special
rebuild is required — just make sure `build/` was configured without
overriding it:

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug   # explicit, though it's the default
cmake --build build
```

Then run the binary under `cgdb`:

```bash
cgdb ./build/main
cgdb ./build/tests
```

Common `cgdb`/`gdb` commands once inside:

```
b solution.cpp:42   # set a breakpoint at a file:line
run                  # start the program (add args after `run` if needed)
n / s                # next line / step into
p someVar            # print a variable
bt                   # backtrace
c                     # continue
```

If a binary was built with optimizations (e.g. `-DCMAKE_BUILD_TYPE=Release`),
reconfigure with `-DCMAKE_BUILD_TYPE=Debug` and rebuild before debugging —
optimized builds can make `cgdb`'s line-by-line stepping misleading or
skip variables entirely.

### Python (`pdb`)

No special build step is needed — Python always runs with debug info available.

Drop a breakpoint directly in the solution or test file:

```python
breakpoint()   # built-in since Python 3.7; equivalent to import pdb; pdb.set_trace()
```

Then run normally and execution will stop there:

```bash
uv run python src/solution.py
uv run pytest -s   # -s so breakpoint() output isn't captured by pytest
```

Or run a whole script under `pdb` from the start, without editing it:

```bash
uv run python -m pdb src/solution.py
```

Common `pdb` commands once stopped:

```
n       # next line
s       # step into
c       # continue
p someVar   # print a variable
l       # list source around current line
bt      # backtrace
q       # quit
```

## The `implement-neetcode` skill

Located at [`.claude/skills/implement-neetcode/`](.claude/skills/implement-neetcode/SKILL.md).

Given a neetcode.io problem link (e.g.
`https://neetcode.io/problems/validate-parentheses`), this skill scaffolds a
new problem workspace:

```
<problem-slug>/
├── notes/
│   ├── problem.md
│   └── worked-example.md
└── <language>/   # e.g. python/, cpp/
```

`problem.md` is the statement itself — description, examples/sample
input, and constraints, saved as given. `worked-example.md` is the
derivation built on top of it: a step-by-step walkthrough written like a
math proof, not a finished-solution dump — the problem restatement, the
key insight behind the chosen approach, a concrete trace through the
sample input (including an edge case), pseudocode, a brute-force vs.
optimal complexity comparison, and edge cases worth sanity-checking.

The skill then scaffolds a project directory for the chosen language
(config/build files, src, and a test suite built from `problem.md`'s
examples plus the worked example's edge cases). It never invents the
solution file's signature or body from the problem description — it uses
an identified starter/solution file (pasted by you, or found on the
neetcode page) as-is, or stops and asks you to provide one. Writing the
actual algorithm is still a distinct, later step you ask for separately.

The skill is resumable: if a problem's notes already exist and you ask
for another language, it scaffolds only that language's directory rather
than redoing the notes.

Language rules currently exist for **Python** (`src/`/`tests/`, `uv`/
pytest) and **C++** (`include/`/`src/`/`tests/`, CMake/ctest). Asking for
any other language gets you a suggestion instead — either a minimal
one-off stub, or describing your preferred conventions so they can be
added to the skill.

Trigger it by pasting a neetcode.io problem URL and asking to start, set
up, or scaffold that problem (optionally naming a language, e.g. "...in
python").
