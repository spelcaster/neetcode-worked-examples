---
name: implement-neetcode
description: Scaffold a new problem workspace from a neetcode.io problem link (e.g. https://neetcode.io/problems/validate-parentheses or .../question). Creates a problem directory with a notes/ subdirectory containing a worked, step-by-step example with complexity analysis. Does NOT write any solution code or starter files — the user will separately prompt for the best approach and scaffolding when they're ready to code. Use this whenever the user pastes a neetcode.io URL and wants to start, set up, or scaffold that problem, or explicitly asks to use this skill.
---

# Implement Neetcode

Turn a neetcode.io problem link into a worked example written out like a
math proof — no solution code, no starter files. The user will prompt
separately for the best approach and the best-suited scaffolding when
they're ready to write code.

## Step 1 — Identify the problem

Extract the slug from the URL. It's the path segment after `/problems/`,
with any trailing `/question` or similar stripped:

```
https://neetcode.io/problems/validate-parentheses/question  ->  validate-parentheses
https://neetcode.io/problems/two-sum                          ->  two-sum
```

The slug becomes the problem's directory name, used exactly as-is (kebab-case).

## Step 2 — Get the problem statement

Try fetching the URL. neetcode.io renders problem content client-side with
JavaScript, so a plain fetch typically returns only the page shell/title,
not the description, examples, or constraints — don't spend more than one
attempt on this before falling back.

When the fetch doesn't yield real statement text, fall back to your own
knowledge: neetcode's problems are drawn from well-known interview
questions (mostly matching LeetCode problems of the same or similar name),
so you likely already know the statement, function signature, and
constraints. Reconstruct them from that knowledge.

Either way, before writing any files, state in one or two sentences what
problem you understood (e.g. "This is LeetCode's Valid Parentheses:
check whether a string of brackets is balanced") so the user can correct
you if you've got the wrong problem or a wrong assumption about signature
or constraints. Don't block on this as a question — just say it and
proceed, since the user is right there to interrupt if something's off.

## Step 3 — Create the directory structure

Under the repo root, create:

```
<problem-slug>/
└── notes/
    └── worked-example.md
```

Do not create a language directory or any starter/solution file — no code
for the problem gets written by this skill at all. When the user is ready
to code, they'll prompt separately for the best approach and the
best-suited scaffolding (language, signature, project files, etc.); that's
a deliberate later step, not part of this skill.

If `<problem-slug>/` already exists, don't overwrite silently — check what's
there and ask the user how to proceed.

## Step 4 — Write the worked example

This is the core deliverable. Write `notes/worked-example.md` as a
step-by-step derivation, the way you'd work through a math problem by
hand — not a finished-solution dump. Use this structure:

1. **Problem restatement** — given/determine, in your own words, with a
   link back to the neetcode URL.
2. **Pick the approach** — name the key insight or data structure and
   *why* it fits this problem's shape (e.g. "most-recent-first access
   pattern -> stack").
3. **Walk through a concrete example** — trace the algorithm over an
   actual input, step by step. A table (index / element / action / state
   of the main data structure) works well for anything stack/queue/
   pointer-based. Include at least one case that fails or behaves
   differently, traced the same way, so the edge behavior is visible
   rather than asserted.
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

## Step 5 — Report back

Summarize what was created (paths only, not file contents) in a couple of
sentences. Don't re-paste the worked example back into the chat — the user
can open it. Don't offer to write the solution or scaffold a language
directory — that's a separate request the user will make when ready.
