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
└── notes/
    └── worked-example.md
```

The `worked-example.md` is the core deliverable — a step-by-step derivation
of the problem written like a math proof, not a finished-solution dump. It
covers the problem restatement, the key insight behind the chosen approach,
a concrete trace through an example (including an edge case), pseudocode,
a brute-force vs. optimal complexity comparison, and edge cases worth
sanity-checking.

The skill deliberately does **not** write any solution code or scaffold a
language directory. Once you've read the worked example and are ready to
code, you prompt separately for the best approach and the best-suited
scaffolding (language, function signature, project files, etc.) — that's
a distinct, later step from understanding the problem.

Trigger it by pasting a neetcode.io problem URL and asking to start, set
up, or scaffold that problem.
