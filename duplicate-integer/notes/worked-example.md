# Contains Duplicate — Worked Example

Problem: [./problem.md](./problem.md)

## 1. Problem restatement

Given: an array `nums` of `n` integers.

Determine: `true` if some value occurs more than once anywhere in
`nums`, `false` if every value is unique.

## 2. Pick the approach

The question being asked per element is "have I seen this value
before?" — a membership test repeated `n` times. A hash set answers
membership in O(1) average time, so the natural approach is to walk
the array once, and for each element check whether it's already in a
set before inserting it. The moment a lookup succeeds, the answer is
`true` without looking at the rest of the array; if the loop finishes
without a hit, every value was distinct.

## 3. Walk through a concrete example

**`nums = [1, 2, 3, 3]`** (Example 1 — expect `true`):

| i | nums[i] | in set already? | action        | set after        |
|---|---------|------------------|---------------|-------------------|
| 0 | 1       | no               | insert 1      | `{1}`             |
| 1 | 2       | no               | insert 2      | `{1, 2}`          |
| 2 | 3       | no               | insert 3      | `{1, 2, 3}`       |
| 3 | 3       | **yes**          | return `true` | `{1, 2, 3}`       |

The loop stops at `i = 3` without ever reaching the end of the array.

**`nums = [1, 2, 3, 4]`** (Example 2 — expect `false`):

| i | nums[i] | in set already? | action   | set after         |
|---|---------|------------------|----------|--------------------|
| 0 | 1       | no               | insert 1 | `{1}`              |
| 1 | 2       | no               | insert 2 | `{1, 2}`           |
| 2 | 3       | no               | insert 3 | `{1, 2, 3}`        |
| 3 | 4       | no               | insert 4 | `{1, 2, 3, 4}`     |

The loop runs to completion with no hit, so the answer is `false`.

**Edge case trace — `nums = []`:** the loop body never executes, so no
lookup ever happens; the function falls through to `false` — an empty
array correctly has no duplicate.

## 4. Generalize into pseudocode

```
function hasDuplicate(nums):
    seen = empty set
    for x in nums:
        if x in seen:
            return true
        seen.insert(x)
    return false
```

## 5. Complexity, worst vs. desired solution

- **Naive (nested loops)**: for each index `i`, scan every later index
  `j > i` and compare `nums[i]` to `nums[j]`. This is **O(n^2)** time
  and **O(1)** extra space. It's slow because it repeats work: the
  pair `(i, j)` is checked without remembering anything learned from
  earlier comparisons, so the same values get re-examined over and
  over as `i` advances.
- **Sort-and-scan (middle ground)**: sort `nums` (**O(n log n)**), then
  walk it once checking adjacent equal elements (**O(n)**) — **O(n log n)**
  time overall, **O(1)** extra space (or **O(n)** if the sort isn't
  in-place). Better than brute force but still pays a sorting cost that
  isn't fundamentally necessary for this question.
- **Hash set (desired)**: walk the array once, checking and inserting
  into a hash set — **O(n)** time (average case; hash insert/lookup is
  O(1) average), **O(n)** space for the set in the worst case (all
  elements distinct until the last one, or no duplicate at all). It
  avoids both the repeated pairwise comparisons of brute force and the
  sorting cost of the middle approach, trading O(n) extra memory for
  dropping the time from quadratic (or n log n) to linear.

Gap: `O(n^2) -> O(n)` by trading nothing fundamental, just O(n) extra
space for a hash set, for removing the repeated re-scanning that both
other approaches pay for in different forms.

## 6. Edge cases

- **Empty array** (`nums = []`): traced in section 3 — the loop never
  runs, so the function returns `false` immediately.
- **Single element** (`nums = [5]`): one insert, no lookup ever finds a
  match, returns `false` — a single value can never be its own
  duplicate.
- **All identical values** (`nums = [7, 7, 7]`): the very first repeat,
  at `i = 1`, triggers the early `return true` — the algorithm doesn't
  need to see every element to detect a duplicate.
- **Negative and boundary values** (`nums = [-10^9, 10^9, -10^9]`): the
  set stores raw integer values, so sign and magnitude don't matter —
  `-10^9` is found as a duplicate the same way any other value would
  be; no special-casing is needed for the constraint's value range.
