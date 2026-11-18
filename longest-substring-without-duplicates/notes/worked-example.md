# Longest Substring Without Repeating Characters — Worked Example

Problem: [`notes/problem.md`](./problem.md)

## 1. Restatement

Given: a string `s`.
Determine: the length of the longest contiguous substring of `s` that
contains no repeated character.

## 2. Pick the approach

The brute force checks every substring for duplicates independently,
which repeats work: if `s[i..j]` has no duplicates, extending to
`s[i..j+1]` only needs to check whether `s[j+1]` is new — it doesn't
need to recheck `s[i..j]` from scratch. That "extend a window on the
right, only shrink it from the left when a conflict appears" access
pattern is a **sliding window**, backed by a **hash set** (or a
hash map of last-seen index) so "is this character already in the
window" is an `O(1)` check instead of a rescan.

Concretely: keep a window `[left, right]` that is always duplicate-free,
and a set of the characters currently inside it. Advance `right` one
character at a time. If `s[right]` is already in the set, shrink the
window from the left — removing characters from the set — until the
duplicate is gone. After each step, the window is duplicate-free, so its
length is a candidate for the answer.

## 3. Trace

### Example 1: `s = "zxyzxyz"` (expected output: `3`)

| right | s[right] | in window? | action | window (left..right) | set | best so far |
|-------|----------|------------|--------|------------------------|-----|-------------|
| 0 | z | no | add | [0,0] = "z" | {z} | 1 |
| 1 | x | no | add | [0,1] = "zx" | {z,x} | 2 |
| 2 | y | no | add | [0,2] = "zxy" | {z,x,y} | 3 |
| 3 | z | yes (at index 0) | shrink left past the old `z`, then add | [1,3] = "xyz" | {x,y,z} | 3 |
| 4 | x | yes (at index 1) | shrink left past the old `x`, then add | [2,4] = "yzx" | {y,z,x} | 3 |
| 5 | y | yes (at index 2) | shrink left past the old `y`, then add | [3,5] = "zxy" | {z,x,y} | 3 |
| 6 | z | yes (at index 3) | shrink left past the old `z`, then add | [4,6] = "xyz" | {x,y,z} | 3 |

Final answer: `3`, matching `notes/problem.md`. The window never grows
past length 3 because every 4th character repeats one already 3 slots
back, forcing a shrink that keeps pace with the growth.

### Example 2: `s = "xxxx"` (expected output: `1`) — a case where the window can't grow at all

| right | s[right] | in window? | action | window (left..right) | set | best so far |
|-------|----------|------------|--------|------------------------|-----|-------------|
| 0 | x | no | add | [0,0] = "x" | {x} | 1 |
| 1 | x | yes (at index 0) | shrink left past index 0, then add | [1,1] = "x" | {x} | 1 |
| 2 | x | yes (at index 1) | shrink left past index 1, then add | [2,2] = "x" | {x} | 1 |
| 3 | x | yes (at index 2) | shrink left past index 2, then add | [3,3] = "x" | {x} | 1 |

Final answer: `1`. Every new character immediately collides with the one
right behind it, so the window is forced back down to size 1 on every
step instead of ever growing — the edge behavior that distinguishes this
from Example 1, where collisions were 3 steps apart instead of 1.

## 4. Pseudocode

```
function lengthOfLongestSubstring(s):
    window = empty set
    left = 0
    best = 0

    for right in 0 .. length(s) - 1:
        while s[right] in window:
            remove s[left] from window
            left = left + 1
        add s[right] to window
        best = max(best, right - left + 1)

    return best
```

## 5. Complexity: brute force vs. optimal

**Brute force:** for every pair of start/end indices `(i, j)`, scan
`s[i..j]` to check for duplicates (e.g. with a fresh set each time).
That's `O(n^2)` substrings in the worst case, each costing up to `O(n)`
to validate, giving `O(n^3)` time (or `O(n^2)` with a smarter incremental
check per start index), `O(n)` space for the validation set. It's slow
because it re-examines overlapping ranges of the string from scratch
instead of reusing what the previous window already proved duplicate-free.

**Optimal (sliding window + hash set):** `left` and `right` each move
forward at most `n` times total across the whole run (never backward),
so the total work is `O(n)` time. Space is `O(min(n, alphabet size))`
for the set holding the current window's characters — bounded by the
character set (e.g. 128 for ASCII) rather than growing unboundedly with
`n`.

Gap: `O(n^3)` (or `O(n^2)`) → `O(n)` time, by trading nothing but an
`O(min(n, alphabet))`-size hash set for never re-scanning a prefix of
the window that's already known to be duplicate-free.

## 6. Edge cases

- **Empty string, `s = ""`**: the loop over `right` never executes (no
  indices `0 .. -1`), so `best` stays at its initial value `0` — correct,
  since there's no substring at all. Not exercised by either trace above
  since both sample inputs are non-empty, but it falls out of the
  pseudocode's loop bounds directly.
- **Single character, `s = "a"`**: one iteration, `s[0]` isn't in the
  (empty) window, window becomes `"a"`, `best = 1` — same first-row
  mechanism as `right = 0` in both traces above.
- **All identical characters, `s = "xxxx"`**: traced in Example 2 above —
  the window is repeatedly forced back to size 1, so the answer is the
  minimum non-zero length rather than growing with `n`.
- **No repeats anywhere, e.g. `s = "xyz"`**: following the same mechanism
  as the first three rows of Example 1's trace (each new character is
  novel, window only grows, never shrinks) — the window would end up
  spanning the whole string, so the answer equals `length(s)`.
- **Repeat separated by other characters, e.g. the `z` at index 3 in
  Example 1**: the shrink step must move `left` past the *specific*
  earlier occurrence of the repeated character, not just one step — this
  is why `left` advances via a `while` loop (removing one character at a
  time until the duplicate is gone) rather than a single conditional
  move, as seen in every "yes" row of both traces.
