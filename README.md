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
