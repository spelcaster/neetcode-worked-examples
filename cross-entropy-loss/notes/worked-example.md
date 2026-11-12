# Cross Entropy Loss — Worked Example

Problem: https://neetcode.io/problems/cross-entropy-loss

## 1. Problem restatement

Given:
- `predicted_probs`: a list of `n` probability vectors, each of length
  `c` (e.g. the softmax output for each sample, one row per sample).
- `true_labels`: a list of `n` integers, the correct class index
  (0-indexed) for each sample.

Determine: the average cross-entropy loss across all `n` samples, where
each sample's loss is:

```
loss_i = -log(predicted_probs[i][true_labels[i]])
```

Return the mean of `loss_i` over all samples, rounded to 4 decimal
places.

## 2. Pick the approach

The full cross-entropy formula for one sample is a sum over *all*
classes against a one-hot true-label vector:

```
loss_i = -sum(y_ij * log(p_ij) for j in 0..c-1)
```

The key insight: since `y_i` is one-hot (`1` at the true class, `0`
everywhere else), every term in that sum vanishes except the one where
`j == true_labels[i]` — the `0 * log(p_ij)` terms contribute nothing.
So there's no need to ever materialize a one-hot vector or sum over all
`c` classes at all: because `true_labels` is already given as the
class *index* rather than a one-hot vector, each sample's loss is just
a single lookup and a single `log` call:

```
loss_i = -log(predicted_probs[i][true_labels[i]])
```

Then average those `n` per-sample losses.

## 3. Walk through a concrete example

Input:
```
predicted_probs = [[0.7, 0.2, 0.1], [0.1, 0.6, 0.3], [0.2, 0.2, 0.6]]
true_labels     = [0, 1, 2]
```

| i | true_labels[i] | predicted_probs[i][true_labels[i]] | loss_i = -log(p) |
|---|-----------------|-------------------------------------|--------------------|
| 0 | 0               | 0.7                                 | 0.3567             |
| 1 | 1               | 0.6                                 | 0.5108             |
| 2 | 2               | 0.6                                 | 0.5108             |

`mean = (0.3567 + 0.5108 + 0.5108) / 3 = 1.3783 / 3 = 0.4594`

Result: `0.4594`.

**Edge case trace** — a predicted probability of exactly `0` at the
true class, `predicted_probs = [[0.5, 0.0, 0.5]]`, `true_labels = [1]`:

`predicted_probs[0][1] = 0.0`, so `loss_0 = -log(0.0)`. Mathematically
this is `+infinity`, and in Python, `math.log(0.0)` raises
`ValueError: math domain error` rather than returning `inf` — so a
naive implementation crashes outright on this input. Clipping the
probability into a small safe range first, e.g.
`p = clip(predicted_probs[i][true_labels[i]], 1e-15, 1 - 1e-15)`, turns
this into `-log(1e-15) ≈ 34.5387` — a very large but *finite* loss that
correctly signals "the model was almost certainly wrong here," instead
of crashing or silently propagating `inf`/`nan` into the average.

## 4. Generalize into pseudocode

```
function cross_entropy_loss(predicted_probs, true_labels, epsilon = 1e-15):
    losses = []
    for i in range(len(true_labels)):
        p = predicted_probs[i][true_labels[i]]
        p_clipped = clip(p, epsilon, 1 - epsilon)
        losses.append(-log(p_clipped))
    return round(mean(losses), 4)
```

## 5. Complexity, worst vs. desired solution

- **Naive (materialize one-hot, sum over all classes)**: build a
  length-`c` one-hot vector for each sample's `true_labels[i]`, then
  compute the full dot-product sum `-sum(y_ij * log(p_ij) for j in
  0..c-1)`. This is **O(n·c)** time (every one of the `c` classes is
  visited for every one of the `n` samples) and **O(n·c)** space if the
  one-hot vectors are materialized up front — all of that work is wasted
  except for the single nonzero term per sample.
- **Direct indexing (desired)**: since `true_labels[i]` already *is*
  the index of the nonzero one-hot entry, skip constructing the one-hot
  vector entirely and index straight into `predicted_probs[i]`. This is
  **O(n)** time (one lookup + one `log` call per sample) and **O(1)**
  extra space (beyond the output), as derived in section 2.

Gap: **O(n·c) → O(n)**, by recognizing that `true_labels` already
encodes exactly the information a one-hot vector would — no sum over
unused classes is needed at all.

## 6. Edge cases

- **Perfect prediction** (`predicted_probs[i][true_labels[i]] = 1.0`):
  `loss_i = -log(1.0) = 0.0` exactly — no clipping needed, since `1.0`
  is already inside the safe range.
- **Zero probability at the true class** (traced in section 3): without
  clipping, `log(0.0)` raises a domain error in Python (or produces
  `-inf`/`nan` in libraries that don't raise) instead of a large finite
  penalty; clipping to `[epsilon, 1 - epsilon]` avoids this.
- **Single sample** (`n = 1`): the mean degenerates to that one sample's
  loss directly — no averaging effect.
- **Out-of-range class index**: if `true_labels[i]` were ever negative
  or `>= c`, the lookup `predicted_probs[i][true_labels[i]]` would
  raise an index error rather than silently returning a wrong value —
  this is a precondition of the input (valid class indices), not a case
  the algorithm needs to handle gracefully.
