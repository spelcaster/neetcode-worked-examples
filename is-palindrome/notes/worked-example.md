# Valid Palindrome — Worked Example

Problem: [`notes/problem.md`](./problem.md)

## 1. Restatement

Given a string, determine whether it reads the same forward and backward
once we strip out everything that isn't a letter or digit and ignore
case. Return `true`/`false`.

## 2. Pick the approach

The check is inherently symmetric: character `i` from the front must
match character `i` from the back. That access pattern — "compare the
outermost remaining elements, then shrink inward" — is exactly what a
**two-pointer** scan does: one pointer starts at index `0`, one at
`length - 1`, and they walk toward each other.

The only extra wrinkle is that not every character counts. Each pointer,
before comparing, must skip forward/backward past any character that
isn't alphanumeric. Because both pointers skip independently, there's no
need to build a separate cleaned-up copy of the string first — the
skipping can happen in place as the pointers move.

## 3. Trace

### Successful case: `s = "Was it a car or a cat I saw?"`

Indices (0-based) of the raw string, including spaces/punctuation:

```
0:W 1:a 2:s 3:' ' 4:i 5:t 6:' ' 7:a 8:' ' 9:c 10:a 11:r 12:' ' 13:o 14:r
15:' ' 16:a 17:' ' 18:c 19:a 20:t 21:' ' 22:I 23:' ' 24:s 25:a 26:w 27:?
```

| left | right | left char (skip?) | right char (skip?) | compared (lowercased) | result |
|------|-------|--------------------|---------------------|------------------------|--------|
| 0  | 27 | `W` (keep) | `?` (skip, right→26) | — | — |
| 0  | 26 | `W` (keep) | `w` (keep) | `w` == `w` | match → left=1, right=25 |
| 1  | 25 | `a` (keep) | `a` (keep) | `a` == `a` | match → left=2, right=24 |
| 2  | 24 | `s` (keep) | `s` (keep) | `s` == `s` | match → left=3, right=23 |
| 3  | 23 | `' '` (skip, left→4) | `' '` (skip, right→22) | — | — |
| 4  | 22 | `i` (keep) | `I` (keep) | `i` == `i` | match → left=5, right=21 |
| 5  | 21 | `t` (keep) | `' '` (skip, right→20) | — | — |
| 5  | 20 | `t` (keep) | `t` (keep) | `t` == `t` | match → left=6, right=19 |
| 6  | 19 | `' '` (skip, left→7) | `a` (keep) | — | — |
| 7  | 19 | `a` (keep) | `a` (keep) | `a` == `a` | match → left=8, right=18 |
| 8  | 18 | `' '` (skip, left→9) | `c` (keep) | — | — |
| 9  | 18 | `c` (keep) | `c` (keep) | `c` == `c` | match → left=10, right=17 |
| 10 | 17 | `a` (keep) | `' '` (skip, right→16) | — | — |
| 10 | 16 | `a` (keep) | `a` (keep) | `a` == `a` | match → left=11, right=15 |
| 11 | 15 | `r` (keep) | `' '` (skip, right→14) | — | — |
| 11 | 14 | `r` (keep) | `r` (keep) | `r` == `r` | match → left=12, right=13 |
| 12 | 13 | `' '` (skip, left→13) | `o` (keep) | — | — |
| 13 | 13 | pointers meet | | | loop ends → **true** |

Every comparison matched and the pointers met in the middle without ever
crossing a mismatch, so the function returns `true`.

### Failing case: `s = "tab a cat"`

```
0:t 1:a 2:b 3:' ' 4:a 5:' ' 6:c 7:a 8:t
```

| left | right | left char | right char | compared | result |
|------|-------|-----------|------------|----------|--------|
| 0 | 8 | `t` | `t` | `t` == `t` | match → left=1, right=7 |
| 1 | 7 | `a` | `a` | `a` == `a` | match → left=2, right=6 |
| 2 | 6 | `b` | `c` | `b` != `c` | **mismatch → return false immediately** |

The pointers never even reach the middle — the function exits as soon as
the first mismatch is found, without inspecting the rest of the string.

## 4. Pseudocode

```
function isPalindrome(s):
    left = 0
    right = length(s) - 1

    while left < right:
        while left < right and not isAlnum(s[left]):
            left += 1
        while left < right and not isAlnum(s[right]):
            right -= 1

        if toLower(s[left]) != toLower(s[right]):
            return false

        left += 1
        right -= 1

    return true
```

## 5. Complexity: brute force vs. optimal

**Brute force:** build a new string containing only the lowercased
alphanumeric characters (one O(n) pass), then compare it against its own
reverse (another O(n) pass to reverse, plus an O(n) comparison). This is
O(n) time — same order as the optimal approach — but it's O(n) **extra
space**, because it allocates a second (and sometimes a third, for the
reversed copy) full string just to throw it away after one comparison.
The repeated work isn't asymptotic here, it's the unnecessary allocation
and copying of data that the two-pointer approach never needs to
materialize.

**Optimal (two-pointer):** walk both ends of the original string inward,
skipping non-alphanumeric characters and lowercasing on the fly during
the single pass. O(n) time, **O(1) extra space** — nothing is copied,
reversed, or stored beyond the two index variables.

Gap: same O(n) time class, but O(n) space → O(1) space, by comparing
in place instead of materializing a cleaned/reversed copy.

## 6. Edge cases

- **All non-alphanumeric, e.g. `s = "..,?"`**: both pointers skip every
  character while scanning toward each other; `left` and `right` cross
  (or meet) without a single real comparison happening. The loop
  condition `left < right` stops them from reading past each other, and
  the function returns `true` — an empty "effective" string is
  vacuously a palindrome, matching how the trace above would behave if
  every row were a skip row.
- **Single character, e.g. `s = "a"`**: `left = 0`, `right = 0`, so
  `left < right` is false immediately — the loop body never runs and the
  function returns `true` without comparing anything, same mechanism as
  the trace's termination row (`left == right` → stop).
- **Already all lowercase letters, no punctuation, e.g. `s = "racecar"`**:
  behaves exactly like the successful trace above but with no skip rows
  at all — every step is a compare-and-match step.
- **Mismatch at the very first characters, e.g. `s = "ab"`**: same exit
  mechanism as the failing trace — the very first comparison (`a` vs
  `b`) fails and the function returns `false` before the pointers move
  at all.
