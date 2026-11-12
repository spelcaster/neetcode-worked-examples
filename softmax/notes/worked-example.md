# Softmax — Worked Example

Problem: https://neetcode.io/problems/softmax

## 1. Problem restatement

Given a list of `n` real-valued scores `scores = [x_1, x_2, ..., x_n]`,
determine the softmax probability distribution over them:

```
softmax(x_i) = e^(x_i) / sum(e^(x_j) for all j)
```

Return the resulting list of `n` probabilities, each rounded to 4
decimal places. The outputs should sum to (approximately) 1.0, since
softmax converts arbitrary real scores into a valid probability
distribution.

## 2. Pick the approach

This is a two-pass formula evaluation over a list, not a search problem
— each output depends on *every* input (through the shared denominator
sum), so the natural shape is:

1. First pass: exponentiate every score, accumulating the sum of all
   exponentials as you go.
2. Second pass: divide each exponentiated value by that total sum.

The key structural fact that drives the *robust* version of this
(covered in section 5): softmax is **shift-invariant** — subtracting
any constant `c` from every `x_i` before exponentiating doesn't change
the result, because that constant factors out of both numerator and
denominator and cancels:

```
e^(x_i - c) / sum(e^(x_j - c)) = e^(x_i)·e^(-c) / (e^(-c)·sum(e^(x_j))) = e^(x_i) / sum(e^(x_j))
```

This identity is what lets you keep every exponent in a safe range
without changing the answer.

## 3. Walk through a concrete example

Input: `scores = [1.0, 2.0, 3.0]`

| i | x_i | e^(x_i)  | running sum |
|---|-----|----------|-------------|
| 0 | 1.0 | 2.7183   | 2.7183      |
| 1 | 2.0 | 7.3891   | 10.1074     |
| 2 | 3.0 | 20.0855  | 30.1929     |

Second pass — divide each `e^(x_i)` by the total `30.1929`:

| i | e^(x_i) | e^(x_i) / 30.1929 | rounded |
|---|---------|----------------------|---------|
| 0 | 2.7183  | 0.0900                | 0.0900  |
| 1 | 7.3891  | 0.2447                | 0.2447  |
| 2 | 20.0855 | 0.6652                | 0.6652  |

Result: `[0.0900, 0.2447, 0.6652]` — sums to `0.9999` (rounding error
only; the unrounded values sum to exactly 1.0).

**Edge case trace** — large scores that overflow if exponentiated
directly, `scores = [1000.0, 1001.0, 1002.0]`:

Computing `e^(1000.0)` directly overflows a 64-bit float (Python's
`math.exp` raises `OverflowError` past roughly `e^(709)`). Applying the
shift-invariance identity from section 2 — subtract the max, `c = 1002.0`
— first:

| i | x_i    | x_i - c | e^(x_i - c) |
|---|--------|---------|-------------|
| 0 | 1000.0 | -2.0    | 0.1353      |
| 1 | 1001.0 | -1.0    | 0.3679      |
| 2 | 1002.0 | 0.0     | 1.0000      |

Sum = `1.5032`. Dividing: `[0.1353/1.5032, 0.3679/1.5032, 1.0/1.5032]` =
`[0.0900, 0.2447, 0.6652]` — **identical** to the first example, because
`[1000, 1001, 1002]` is just `[1, 2, 3]` shifted by a constant `999`.
This confirms the identity: shifting by the max turned three
exponents that would overflow into `[-2, -1, 0]`, which are always safe
(the largest is `e^0 = 1`), without changing a single output digit.

## 4. Generalize into pseudocode

```
function softmax(scores):
    c = max(scores)                        # shift for numerical safety
    exponentials = [exp(x - c) for x in scores]
    total = sum(exponentials)
    return [round(e / total, 4) for e in exponentials]
```

## 5. Complexity, worst vs. desired solution

As with other activation-function problems, there's no Big-O gap here
— any correct implementation touches each of the `n` scores a constant
number of times, so both versions below are **O(n)** time and **O(n)**
space (to hold the output list and the intermediate exponentials). The
real gap is numerical robustness, not asymptotic class:

- **Naive**: exponentiate each `x_i` directly (`e^(x_i)`), sum them,
  then divide. **O(n)** time, **O(n)** space. This is correct for small
  or moderate inputs, but for any score much above `~709` it overflows
  (`OverflowError` in Python, or `inf` in languages/libraries that don't
  raise), because `e^(x_i)` itself is computed without bound.
- **Max-shifted (desired)**: subtract `max(scores)` from every score
  before exponentiating, as derived in section 2 and traced in section 3.
  Still **O(n)** time and **O(n)** space — finding the max is one extra
  O(n) pass, not a different complexity class — but every exponent is
  now `≤ 0`, so `e^(x_i - c) ≤ 1` always, which can never overflow
  regardless of how large the input scores are.

So the gap is: both are O(n)/O(n), but the naive version silently
breaks on realistic large-magnitude inputs (e.g. raw logits from a
poorly-scaled model) while the shifted version is mathematically
guaranteed safe, at the cost of one extra pass to find the max.

## 6. Edge cases

- **All scores equal** (e.g. `[5.0, 5.0, 5.0]`): every `x_i - c = 0`,
  so every `e^(x_i - c) = 1`, and the result is a uniform distribution
  `[0.3333, 0.3333, 0.3333]` — ties in input produce ties in output, as
  expected for a fair softmax.
- **Single element** (`[7.0]`): `c = 7.0`, `e^(0) = 1`, sum = `1`,
  result `[1.0]` — softmax over one option always assigns it full
  probability.
- **Very large or widely-spread scores** (traced in section 3): handled
  safely by the max-shift trick; the naive direct-exponentiation
  version would overflow on the same input.
- **Negative scores** (e.g. `[-5.0, -1.0, 0.0]`): no special handling
  needed — the max-shift trick subtracts the (least negative) max just
  as it would for positive inputs, so every shifted exponent is still
  `≤ 0`.
