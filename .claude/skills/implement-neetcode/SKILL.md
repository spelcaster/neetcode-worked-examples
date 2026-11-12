---
name: implement-neetcode
description: Scaffold a new problem workspace from a neetcode.io problem link (e.g. https://neetcode.io/problems/validate-parentheses or .../question). Creates a problem directory with notes/ (problem statement + a worked, step-by-step example with complexity analysis), then scaffolds a language project directory (config, src, tests) for a supported language — currently Python and C++. Resumable: if the problem's notes already exist and only a new language is requested, scaffolds just that language directory. Never invents the solution file's signature or algorithm — uses an identified starter/solution file or asks the user for one. Use this whenever the user pastes a neetcode.io URL and wants to start, set up, or scaffold that problem (in a given language), or explicitly asks to use this skill.
---

# Implement Neetcode

Turn a neetcode.io problem link into a problem statement, a worked example
written out like a math proof, and a scaffolded language project
directory so the user can start coding immediately. Scaffolding never
includes the actual algorithm or an invented function signature — the src
file comes from an identified starter/solution file, or from the user.
Writing the algorithm itself is still a distinct, later step the user asks
for separately.

## Step 1 — Identify the problem

Extract the slug from the URL. It's the path segment after `/problems/`,
with any trailing `/question` or similar stripped:

```
https://neetcode.io/problems/validate-parentheses/question  ->  validate-parentheses
https://neetcode.io/problems/two-sum                          ->  two-sum
```

The slug becomes the problem's directory name, used exactly as-is (kebab-case).

Also note which language, if any, the user's request already named (e.g.
"...in python", "...in cpp") — Step 2 needs it.

## Step 2 — Check what already exists

Before fetching or writing anything, look at `<problem-slug>/` on disk:

- **Doesn't exist at all** → proceed with the full flow from Step 3.
- **Exists, with `notes/problem.md` and `notes/worked-example.md` both
  present, and a language was requested that doesn't have a directory
  yet** → the notes are done; skip straight to Step 7 and scaffold only
  that language directory. Don't re-fetch the statement or rewrite the
  worked example — read the existing notes for context (sample input,
  trace, edge cases) instead.
- **The requested language directory already exists too** → don't
  overwrite silently. Show the user what's there and ask how to proceed.
- **Exists, but notes are incomplete** (e.g. an older `worked-example.md`
  from before `problem.md` existed) → flag this to the user, backfill the
  missing note from Step 5/6 below, then continue.
- **No language named yet, notes already complete** → ask which language
  before doing anything else (same as Step 7's language check).

## Step 3 — Get the problem statement

Try fetching the URL. neetcode.io renders problem content client-side with
JavaScript, so a plain fetch typically returns only the page shell/title,
not the description, examples, or constraints — don't spend more than one
attempt on this before falling back. While you have the fetch result,
also look for any embedded starter/solution code for the target
language (e.g. a JSON data blob in the page source) — Step 7 needs it,
and it's cheaper to look now than to re-fetch later.

When the fetch doesn't yield real statement text, fall back to your own
knowledge: neetcode's problems are drawn from well-known interview
questions (mostly matching LeetCode problems of the same or similar name),
so you likely already know the statement, function signature, examples,
and constraints. Reconstruct them from that knowledge.

Either way, before writing any files, state in one or two sentences what
problem you understood (e.g. "This is LeetCode's Valid Parentheses:
check whether a string of brackets is balanced") so the user can correct
you if you've got the wrong problem or a wrong assumption about signature
or constraints. Don't block on this as a question — just say it and
proceed, since the user is right there to interrupt if something's off.

## Step 4 — Create the directory structure

Under the repo root, create:

```
<problem-slug>/
└── notes/
    ├── problem.md
    └── worked-example.md
```

The language directory (`<problem-slug>/<language>/`) comes later, in
Step 7, once the notes exist — the sample input and edge cases written
there are what the scaffolded tests are built from.

## Step 5 — Write notes/problem.md

Save the problem statement itself here — this is the source material, not
the derivation. Include:

1. **Title and link** back to the neetcode URL.
2. **Description** — the problem as given (or reconstructed per Step 3),
   in full enough detail to code against: what's given, what to return,
   any function signature hints.
3. **Examples** — every sample input/output pair from the problem (with
   explanations if the source gives them). These are copied as-is, not
   paraphrased — Step 7's tests are built directly from them.
4. **Constraints** — input bounds, edge conditions the problem statement
   itself calls out (e.g. string length, value ranges).

If Step 3 fell back to reconstructing from knowledge rather than a real
fetch, say so in this file (a one-line note) so the user knows the
examples/constraints are reconstructed, not scraped verbatim.

## Step 6 — Write the worked example

This is the derivation, not the statement — write `notes/worked-example.md`
as a step-by-step walkthrough, the way you'd work through a math problem
by hand. Use this structure:

1. **Problem restatement** — given/determine, in your own words, with a
   link back to `notes/problem.md`.
2. **Pick the approach** — name the key insight or data structure and
   *why* it fits this problem's shape (e.g. "most-recent-first access
   pattern -> stack").
3. **Walk through a concrete example** — trace the algorithm over the
   sample input from `notes/problem.md`, step by step. A table (index /
   element / action / state of the main data structure) works well for
   anything stack/queue/pointer-based. Include at least one case that
   fails or behaves differently, traced the same way, so the edge
   behavior is visible rather than asserted.
4. **Generalize into pseudocode** — the algorithm distilled from the
   trace above.
5. **Complexity, worst vs. desired solution** — this is required, not
   optional. Cover both:
   - The **brute-force / naive** approach someone would reach for first
     (e.g. checking every pair, nested loops, recomputing from scratch) —
     its time and space complexity, and *why* it's slow (what repeated
     work it does).
   - The **optimal / desired** approach from steps 2-4 — its time and
     space complexity, and *why* it avoids that repeated work.
   State Big-O for both and name the gap plainly (e.g. "O(n^2) -> O(n) by
   trading nothing for an O(n) hash set").
6. **Edge cases** — the inputs worth sanity-checking (empty input, all
   one type, single element, etc.) and what the algorithm does with each,
   tied back to the trace in step 3 rather than asserted from nowhere.

Use `notes/validate-parentheses-worked-example.md` at the repo root (if
present) as a reference for depth and tone — it's a worked example for
Valid Parentheses done in this style.

## Step 7 — Scaffold the language directory

Scaffold `<problem-slug>/<language>/` so the user can start coding
immediately instead of asking for scaffolding in a separate prompt.

**Determine the language.** If the user's request already named one, use
that. Otherwise ask which language before creating anything — don't guess.

**Only scaffold a language this skill has rules for below.** If the user
asks for one with no rules yet, don't invent a structure for it. Tell them
plainly that this skill doesn't have conventions for `<language>` yet, and
suggest either:
- scaffolding a minimal single-file layout as a one-off now (once the src
  file itself is sourced per the rule below), or
- they describe the conventions they want (build tool, test framework,
  layout), so rules for that language can be added to this skill for next
  time.

**Identify the solution file — never invent it.** The src file's function/
method signature and body must come from an actual source, not from your
own reading of the problem:
- Check whether the user already pasted or attached a starter/solution
  file for this language in the conversation.
- Check whether Step 3's fetch turned up an embedded starter-code template
  for this language on the neetcode page.
- If neither is available, **stop and ask the user to provide the
  solution/starter file for `<language>`** before creating anything in
  the language directory. Do not fabricate a signature or a stub body
  from the question description or from the algorithm identified in the
  worked example — that's exactly what this step must not do. Config/
  build files with no bearing on the signature (see below) can still be
  pointed out as ready to go, but wait on the src file and tests.

Once a real solution file is in hand, save its content as given — don't
rewrite its signature or body — into the language's standard src location
(e.g. splitting a single provided snippet into a declaration + definition
for C++, if that's this language's convention). Preserve whatever body it
already has (even if that body is just a `pass` or empty return from a
starter template) rather than replacing it with your own placeholder.

**Tests use the real signature and the real sample input.** Build the test
file against the solution file's actual method/function signature, with
cases taken from `notes/problem.md`'s examples (not invented ones) plus
the edge cases from `notes/worked-example.md` step 6. The suite may fail
(or not build meaningfully) until the solution is implemented — that's
expected and fine.

### Python

Based on `single-neuron/python/`. Create:

```
<problem-slug>/python/
├── pyproject.toml
├── src/
│   └── solution.py
└── tests/
    └── test_solution.py
```

`pyproject.toml`:
```toml
[project]
name = "<problem-slug>"
version = "0.1.0"
requires-python = ">=3.10"
dependencies = []

[dependency-groups]
dev = [
    "pytest",
]

[tool.pytest.ini_options]
pythonpath = ["src"]
```
Only add to `dependencies` if the problem genuinely needs a library (e.g.
`numpy` for array/ML-flavored problems) — leave it empty otherwise.

`src/solution.py`: a leading comment `# Worked example: ../notes/worked-example.md`,
then the identified solution file's `Solution` class as given (signature
and body untouched).

`tests/test_solution.py`: `pytest` tests (use `@pytest.mark.parametrize`
when there's more than one case) built from `notes/problem.md`'s examples
and the worked example's edge cases, importing `Solution` from `solution`.

After creating the files, run `uv sync` inside `<problem-slug>/python/` if
`uv` is on `PATH`, so `uv.lock` and `.venv` are ready to go. If `uv` isn't
available, skip this and just note it to the user — don't fail the
scaffold over it.

### C++

Based on `validate-parentheses/cpp/`. Create:

```
<problem-slug>/cpp/
├── CMakeLists.txt
├── .gitignore
├── README.md
├── include/
│   └── solution.hpp
├── src/
│   ├── solution.cpp
│   └── main.cpp
└── tests/
    └── test_solution.cpp
```

`CMakeLists.txt`: mirror `validate-parentheses/cpp/CMakeLists.txt` — C++17
(`CMAKE_CXX_STANDARD 17`, required), `-Wall -Wextra`, `include/` as the
include dir, a `solution` library built from `src/solution.cpp`, a `main`
executable linking it, a `tests` executable linking it, and `ctest` wired
up (`enable_testing()` + `add_test`). Use `<problem-slug with underscores>`
as the `project(...)` name.

`.gitignore`: just `build/` and `compile_commands.json`.

`include/solution.hpp`: `#pragma once`, whatever headers the identified
signature needs, a `Solution` class declaring the method(s) exactly as
given in the identified solution file.

`src/solution.cpp`: includes the header, method body exactly as given in
the identified solution file (untouched — don't replace a provided body
with a placeholder, and don't write one if none was given beyond the
declaration).

`src/main.cpp`: a small interactive runner reading input from stdin and
printing the result, following the pattern in
`validate-parentheses/cpp/src/main.cpp`.

`tests/test_solution.cpp`: an assert-based `main()` with cases from
`notes/problem.md`'s examples and the worked example's edge cases,
following `validate-parentheses/cpp/tests/test_solution.cpp`.

`README.md`: short — point at `../notes/problem.md` and
`../notes/worked-example.md`, list the file layout, and give build/run
commands (`cmake -S . -B build`, `cmake --build build`, `./build/main`,
`./build/tests`, `ctest --test-dir build`). Follow
`validate-parentheses/cpp/README.md`.

## Step 8 — Report back

Summarize what was created (paths only, not file contents) in a couple of
sentences — the notes (or that they were reused, per Step 2), and the
scaffolded language directory, or what you're waiting on (a language
choice, or a solution file) if scaffolding couldn't finish. Don't re-paste
the problem statement or worked example back into the chat — the user can
open them. Don't offer to write the solution itself — that's a separate
request the user will make when ready.
