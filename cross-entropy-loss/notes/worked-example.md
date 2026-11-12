# Cross-Entropy Loss — Worked Example

Problem: [../notes/problem.md](./problem.md)

> Note: this replaces an earlier version of this file that was written
> against a different (stale) signature — a single
> `cross_entropy_loss(predicted_probs, true_labels)` function taking
> class indices. The actual problem on neetcode.io asks for two
> methods, `binary_cross_entropy` and `categorical_cross_entropy`, both
> taking a `y_true`/`y_pred` pair. This version is traced against that
> real signature.

## 1. Problem restatement

Given, for the **binary** case:
- `y_true`: a 1D array of `n` labels, each `0` or `1`.
- `y_pred`: a 1D array of `n` predicted probabilities (that the label
  is `1`).

Determine: `-mean(y_i * ln(p_i) + (1 - y_i) * ln(1 - p_i))`, rounded to
4 decimal places.

Given, for the **categorical** case:
- `y_true`: an `n x C` one-hot array — row `i` has a `1` at the column
  for sample `i`'s true class, `0` elsewhere.
- `y_pred`: an `n x C` array of predicted per-class probabilities.

Determine: `-mean_i(sum_c(y_{i,c} * ln(p_{i,c})))`, rounded to 4 decimal
places.

## 2. Pick the approach

Both formulas are a per-sample penalty term averaged over all `n`
samples — the natural fit is to compute the array of per-sample terms
with vectorized NumPy operations, then take `np.mean(...)`, rather than
summing in a Python `for` loop. There's no index-juggling needed:

- **Binary**: `y_i` is already `0` or `1`, so
  `y_i * ln(p_i) + (1 - y_i) * ln(1 - p_i)` naturally selects the right
  term per sample just by being multiplied through — when `y_i = 1` the
  second term's `(1 - y_i)` factor zeroes it out, and vice versa.
- **Categorical**: `y_true` is one-hot, so `y_true * np.log(p)`
  followed by `np.sum(axis=1)` has exactly the same effect as indexing
  into the true class directly — every term except the true class's is
  multiplied by `0` and vanishes from the row sum. No explicit
  `argmax`/index lookup is needed; the one-hot structure does the
  selection for free.

Either way, `y_pred` must be clipped into `[1e-7, 1 - 1e-7]` first,
since `ln(0)` is undefined.

## 3. Walk through a concrete example

**Binary** — `y_true = [1, 0, 1]`, `y_pred = [0.9, 0.1, 0.8]`:

| i | y_i | p_i | term = -(y_i·ln(p_i) + (1-y_i)·ln(1-p_i)) |
|---|-----|-----|---------------------------------------------|
| 0 | 1   | 0.9 | -ln(0.9) = 0.10536                           |
| 1 | 0   | 0.1 | -ln(1 - 0.1) = -ln(0.9) = 0.10536            |
| 2 | 1   | 0.8 | -ln(0.8) = 0.22314                           |

`mean = (0.10536 + 0.10536 + 0.22314) / 3 = 0.43386 / 3 = 0.14462`

Rounded: `0.1446` — matches the problem's Example 1.

**Categorical** — `y_true = [[1,0,0], [0,1,0]]`,
`y_pred = [[0.9,0.05,0.05], [0.1,0.8,0.1]]`:

| i | y_i (row)   | p_i (row)         | nonzero class | term = -ln(p at that class) |
|---|-------------|-------------------|----------------|-------------------------------|
| 0 | [1,0,0]     | [0.9,0.05,0.05]   | 0              | -ln(0.9) = 0.10536            |
| 1 | [0,1,0]     | [0.1,0.8,0.1]     | 1              | -ln(0.8) = 0.22314            |

`mean = (0.10536 + 0.22314) / 2 = 0.32850 / 2 = 0.16425`

Rounded: `0.1643` — matches the problem's Example 2. Note that every
other entry in each row (e.g. `0.05`, `0.05` in row 0) is multiplied by
`y_{i,c} = 0` and never contributes — confirming the one-hot-zeroes-it
reasoning from step 2.

**Edge case trace** — a predicted probability of exactly `0` at the
position that matters, e.g. binary `y_true = [1]`, `y_pred = [0.0]`:

`term = -ln(0.0)`. Mathematically this is `+infinity`, and in Python,
`np.log(0.0)` produces `-inf` with a `RuntimeWarning: divide by zero`
rather than raising — so an unclipped implementation silently produces
`inf`, which then poisons the mean for every other sample too (`inf`
added to any finite loss is still `inf`). Clipping first,
`p = clip(0.0, 1e-7, 1 - 1e-7) = 1e-7`, turns this into
`-ln(1e-7) ≈ 16.11810` — a very large but finite loss that correctly
signals "almost certainly wrong," instead of propagating `inf` into the
average.

## 4. Generalize into pseudocode

```
function binary_cross_entropy(y_true, y_pred, epsilon = 1e-7):
    p = clip(y_pred, epsilon, 1 - epsilon)
    terms = -(y_true * ln(p) + (1 - y_true) * ln(1 - p))   # elementwise
    return round(mean(terms), 4)

function categorical_cross_entropy(y_true, y_pred, epsilon = 1e-7):
    p = clip(y_pred, epsilon, 1 - epsilon)
    per_sample = -sum(y_true * ln(p), axis=classes)         # row-wise sum
    return round(mean(per_sample), 4)
```

## 5. Complexity, worst vs. desired solution

- **Naive (nested Python `for` loops)**: iterate samples (and, for the
  categorical case, classes within each sample) in pure Python,
  accumulating a running sum with scalar `math.log` calls. This is
  **O(n)** for binary and **O(n·C)** for categorical — the same
  asymptotic class as the vectorized version, since every element must
  be read at least once either way — but every `log`, multiply, and
  add happens one Python-level operation at a time, with interpreter
  and bounds-checking overhead on each step. It's also easy to forget
  the clip and crash/warn on `log(0)` inside the loop.
- **Vectorized (desired)**: let NumPy's compiled elementwise ops do the
  multiply/log/sum/mean in one call each. Still **O(n)** / **O(n·C)**
  time and **O(n)** / **O(n·C)** space for the intermediate arrays — no
  asymptotic win is available here, since both approaches must touch
  every input element exactly once — but the per-element work moves
  from the Python interpreter into a single vectorized C loop, which is
  the realistic bottleneck difference at scale.

Gap: same Big-O, but Python-level per-element overhead vs. one
vectorized call — plus vectorization makes it natural to clip the
entire array once up front rather than remembering to guard every
individual `log` call.

## 6. Edge cases

- **Perfect prediction** (`p = 1.0` at the position `y = 1`, or
  `p = 0.0` at the position `y = 0` for binary): the active term is
  `-ln(1.0) = 0.0` — no clipping effect, since `1.0` is already inside
  the safe range.
- **Zero probability at the position that matters** (traced in section
  3): without clipping, `ln(0.0)` produces `-inf` (and a runtime
  warning) rather than a large finite penalty; clipping to
  `[1e-7, 1 - 1e-7]` avoids this and keeps the result finite.
- **Single sample** (`n = 1`): the mean degenerates to that one
  sample's term directly — no averaging effect.
- **Binary case expressed as 2-class categorical**: `y_true = [1, 0]`
  one-hot `[[0,1],[1,0]]` and matching `y_pred` should produce the same
  loss as the binary formula on the equivalent 0/1 labels — both
  formulas are really the same quantity, just indexed differently.
