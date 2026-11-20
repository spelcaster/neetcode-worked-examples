# Container With Most Water — Worked Example

Problem: [`notes/problem.md`](./problem.md)

## 1. Restatement

Given: an array `height` of bar heights.
Determine: the maximum value of `(j - i) * min(height[i], height[j])` over
all pairs of indices `i < j` — the widest-and-tallest-possible rectangle
you can form using two bars as its walls (water can't rise above the
shorter wall, and spills over it).

## 2. Pick the approach

Brute force checks every pair `(i, j)`, but there's a key observation that
avoids that: for a *fixed* pair, the wall that is **shorter** is the only
thing limiting the area — the taller wall's extra height is wasted. If you
start with the widest possible container (`i = 0`, `j = n - 1`) and need to
narrow it, moving the pointer at the **taller** wall can only shrink the
width while keeping the same (or a smaller) limiting height — strictly
worse. Moving the pointer at the **shorter** wall is the only move that
has any chance of increasing the limiting height enough to offset the
narrower width.

That "start wide, discard the pair that can't possibly improve, narrow
inward" pattern is the **two-pointer** technique: one pointer at each end,
always advancing the one pointing at the shorter bar.

## 3. Trace

### Main example: `height = [1,7,2,5,4,7,3,6]` (from `notes/problem.md`)

Convention: when `height[left] == height[right]`, move `left` (either
choice is safe — both bars are equally limiting).

| left | right | h[left] | h[right] | width | limiting height | area | max so far | move |
|------|-------|---------|----------|-------|------------------|------|------------|------|
| 0 | 7 | 1 | 6 | 7 | 1 | 7  | 7  | `h[left] < h[right]` → move `left` |
| 1 | 7 | 7 | 6 | 6 | 6 | 36 | 36 | `h[right] < h[left]` → move `right` |
| 1 | 6 | 7 | 3 | 5 | 3 | 15 | 36 | `h[right] < h[left]` → move `right` |
| 1 | 5 | 7 | 7 | 4 | 7 | 28 | 36 | tie → move `left` |
| 2 | 5 | 2 | 7 | 3 | 2 | 6  | 36 | `h[left] < h[right]` → move `left` |
| 3 | 5 | 5 | 7 | 2 | 5 | 10 | 36 | `h[left] < h[right]` → move `left` |
| 4 | 5 | 4 | 7 | 1 | 4 | 4  | 36 | `h[left] < h[right]` → move `left` |
| 5 | 5 | — | — | — | — | — | 36 | `left == right` → stop |

Result: **36**, matching `notes/problem.md`. Note the best pair
(indices 1 and 7, area 36) is found on the very first move away from the
initial widest pair — the bar at index 0 (height 1) is so short that no
pair involving it can beat what's already found, so discarding it loses
nothing.

### Edge case: all-equal bars, `height = [2,2,2]`

| left | right | h[left] | h[right] | width | limiting height | area | max so far | move |
|------|-------|---------|----------|-------|------------------|------|------------|------|
| 0 | 2 | 2 | 2 | 2 | 2 | 4 | 4 | tie → move `left` |
| 1 | 2 | 2 | 2 | 1 | 2 | 2 | 4 | tie → move `left` |
| 2 | 2 | — | — | — | — | — | 4 | `left == right` → stop |

Result: **4**, matching `notes/problem.md`. Every pair here is equally
limited (all bars the same height), so the widest pair — chosen first —
is automatically the best one; narrowing only ever loses width without
any possible compensating gain in height.

## 4. Pseudocode

```
function maxArea(height):
    left = 0
    right = length(height) - 1
    best = 0

    while left < right:
        width = right - left
        limitingHeight = min(height[left], height[right])
        best = max(best, width * limitingHeight)

        if height[left] <= height[right]:
            left += 1
        else:
            right -= 1

    return best
```

## 5. Complexity: brute force vs. optimal

**Brute force (check every pair):** two nested loops over all `i < j`,
computing `(j - i) * min(height[i], height[j])` for each — `O(n^2)` time,
`O(1)` space. It's slow because it re-examines pairs whose outcome is
already decided: once a pair `(i, j)` has been checked, a brute force
still goes on to check every pair involving the *shorter* of the two
walls paired with something narrower, even though step 2's argument shows
none of those can ever beat keeping the current wall and probing inward
from the other side.

**Optimal (two pointers):** `left` and `right` move strictly toward each
other and never reverse, so the loop runs at most `n - 1` times —
`O(n)` time, `O(1)` space. Each iteration discards exactly the one wall
that step 2 proves can't be part of a better answer than what's already
been checked, so no pair is ever implicitly re-examined.

Gap: `O(n^2) -> O(n)` by using the "shorter wall is always safe to
discard" argument to skip whole classes of pairs instead of checking them.

## 6. Edge cases

- **Minimum input, `n = 2`** — the loop body runs exactly once (`left = 0`,
  `right = 1`), computes the only possible pair, and then `left == right`
  isn't reached since `left` or `right` moves to meet the other
  immediately after — same mechanics as the last row of both traces
  above, just with only one row before termination.
- **All-equal heights** — traced above (`[2,2,2]`): every move is a tie,
  broken consistently in the same direction, and the widest pair (checked
  first) already is the answer.
- **Zero-height bar** (`height[i] == 0`, allowed per the `0 <= height[i]`
  constraint) — any pair including it has `limitingHeight = 0`, so its
  area is `0` and it can never set `best`; the two-pointer logic doesn't
  need a special case, since `min(0, anything) = 0` falls out naturally.
- **Strictly increasing or decreasing array** — one pointer (`left` for
  increasing, `right` for decreasing) ends up moving on every single
  iteration until the loop ends, since one side is always the limiting,
  shorter wall; the trace mechanics are identical to the rows above, just
  with the same comparison winning every time.
