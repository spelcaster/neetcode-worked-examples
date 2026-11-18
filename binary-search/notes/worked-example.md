# Binary Search — Worked Example

Problem: [`notes/problem.md`](./problem.md)

## 1. Restatement

Given a sorted (ascending), distinct-valued array `nums` and a `target`
value, determine the index of `target` in `nums`, or `-1` if it isn't
present — in `O(log n)` time.

## 2. Pick the approach

A linear scan would work but ignores the one piece of structure the
problem hands us for free: the array is **sorted**. Sortedness means
that at any candidate index `mid`, comparing `nums[mid]` to `target`
tells us not just "match or no match" but *which half of the remaining
range the target must be in* (if it's there at all) — everything below
`mid` is smaller, everything above is larger. That "discard half the
search space per comparison" access pattern is exactly **binary
search**: maintain a shrinking `[left, right]` window, check the
midpoint, and move one of the two bounds past it depending on the
comparison.

## 3. Trace

### Successful case: `nums = [-1,0,2,4,6,8]`, `target = 4`

Indices: `0:-1 1:0 2:2 3:4 4:6 5:8`

| left | right | mid | nums[mid] | compare to target (4) | action |
|------|-------|-----|-----------|------------------------|--------|
| 0 | 5 | 2 | 2 | `2 < 4` | too small → left = mid + 1 = 3 |
| 3 | 5 | 4 | 6 | `6 > 4` | too big → right = mid - 1 = 3 |
| 3 | 3 | 3 | 4 | `4 == 4` | **match → return 3** |

Matches the expected output (`3`) from `notes/problem.md`.

### Failing case (not found): `nums = [-1,0,2,4,6,8]`, `target = 3`

| left | right | mid | nums[mid] | compare to target (3) | action |
|------|-------|-----|-----------|------------------------|--------|
| 0 | 5 | 2 | 2 | `2 < 3` | too small → left = mid + 1 = 3 |
| 3 | 5 | 4 | 6 | `6 > 3` | too big → right = mid - 1 = 3 |
| 3 | 3 | 3 | 4 | `4 > 3` | too big → right = mid - 1 = 2 |
| 3 | 2 | — | — | `left > right` | window empty → loop ends → **return -1** |

`3` sits between indices 2 and 3 in the sorted array, so it's genuinely
absent — the window shrinks to empty (`left` crosses past `right`)
without ever matching, which is exactly how the algorithm has to signal
"not found." Matches the expected output (`-1`).

## 4. Pseudocode

```
function search(nums, target):
    left = 0
    right = length(nums) - 1

    while left <= right:
        mid = left + (right - left) / 2   // avoids overflow vs (left+right)/2

        if nums[mid] == target:
            return mid
        else if nums[mid] < target:
            left = mid + 1
        else:
            right = mid - 1

    return -1
```

## 5. Complexity: brute force vs. optimal

**Brute force (linear scan):** walk every index from `0` to `n - 1` and
compare against `target`. `O(n)` time, `O(1)` space. It's slow not
because it does *repeated* work exactly, but because it does *unnecessary*
work — it never uses the fact that the array is sorted, so it can't skip
over ranges it already knows can't contain the target.

**Optimal (binary search):** each comparison throws away half of the
remaining candidates, because sortedness guarantees the target (if
present) can only be on one side of `mid`. That halving repeated gives
`O(log n)` time, still `O(1)` space (just three integer variables:
`left`, `right`, `mid` — no extra data structure).

Gap: `O(n)` → `O(log n)` time, same `O(1)` space, by exploiting the
sorted order to discard half the search space per step instead of
checking every element.

## 6. Edge cases

- **Single element, e.g. `nums = [5]`, `target = 5`**: `left = right = 0`,
  `mid = 0`, `nums[0] == target` → returns `0` on the very first
  comparison, same mechanism as the matching row in the trace above.
- **Single element, target absent, e.g. `nums = [5]`, `target = 2`**:
  `left = right = 0`, `mid = 0`, `nums[0] = 5 > 2` → `right = -1`, loop
  condition `left <= right` (`0 <= -1`) fails immediately → returns
  `-1`, same empty-window termination as the failing trace.
- **Target smaller than every element, e.g. `nums = [-1,0,2,4,6,8]`,
  `target = -5`**: every `mid` comparison finds `nums[mid] > target`, so
  `right` keeps shrinking down past `left` without ever reaching the low
  end — same "window closes" exit as the failing trace, just always
  moving `right` instead of a mix of both bounds.
- **Target larger than every element, e.g. `target = 9`**: symmetric to
  the above — every comparison finds `nums[mid] < target`, so `left`
  keeps climbing past `right` until the window closes and `-1` is
  returned.
- **Negative numbers in the array**: the comparisons (`<`, `==`, `>`)
  work identically regardless of sign, since the array is still totally
  ordered — no special-casing needed, as seen in both traces above where
  `-1` sits at index `0` without affecting the logic.
