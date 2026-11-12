# Linear Regression: Forward Pass — Worked Example

Problem: https://neetcode.io/problems/linear-regression-forward

## 1. Problem restatement

Given:
- `features`: a list of `n` feature vectors, each of length `d` (a
  batch of samples).
- `weights`: a length-`d` vector of weights.
- `bias`: a single scalar bias.

Determine: for each sample, compute its linear regression prediction —
a weighted sum of its features plus the bias, with **no** activation
function applied afterward (unlike a classifying neuron, this is a
raw real-valued output, not squashed into a probability):

```
pred_i = (x_i · weights) + bias = sum(x_i[j] * weights[j] for j in 0..d-1) + bias
```

Return the list of `n` predictions, rounded to 4 decimal places.

## 2. Pick the approach

This is a direct simulation, not a search problem — each sample's
prediction is a fixed formula (dot product + bias) applied
independently to that sample alone. The key structural fact: a
sample's prediction depends only on *its own* feature vector and the
*shared* weights/bias — samples never interact with each other — so a
single pass over the samples, computing one dot product each, is all
that's needed:

```
for each sample x_i:
    pred_i = dot(x_i, weights) + bias
```

No data structure beyond a list accumulator is required — this is the
same shape as the linear part of `single-neuron`, just stopped one step
earlier, before any sigmoid/ReLU is applied.

## 3. Walk through a concrete example

Input:
```
features = [[1.0, 2.0], [3.0, -1.0], [0.0, 0.0]]
weights  = [2.0, 0.5]
bias     = 1.0
```

| i | x_i          | x_i · weights                     | + bias | pred_i |
|---|--------------|-------------------------------------|--------|--------|
| 0 | [1.0, 2.0]   | 1.0·2.0 + 2.0·0.5 = 3.0              | +1.0   | 4.0    |
| 1 | [3.0, -1.0]  | 3.0·2.0 + -1.0·0.5 = 5.5             | +1.0   | 6.5    |
| 2 | [0.0, 0.0]   | 0.0·2.0 + 0.0·0.5 = 0.0              | +1.0   | 1.0    |

Result: `[4.0, 6.5, 1.0]`.

**Edge case trace** — all-zero weights (a "blind" model that ignores
every feature):

```
features = [[5.0, -3.0], [100.0, 0.0]]
weights  = [0.0, 0.0]
bias     = 2.5
```

| i | x_i            | x_i · weights (= 0 regardless of x_i) | + bias | pred_i |
|---|----------------|------------------------------------------|--------|--------|
| 0 | [5.0, -3.0]    | 0.0                                        | +2.5   | 2.5    |
| 1 | [100.0, 0.0]   | 0.0                                        | +2.5   | 2.5    |

Result: `[2.5, 2.5]` — with zero weights, every sample collapses to the
same prediction (just the bias), no matter how different the input
features are. This shows the model has learned nothing from the
features in this state; the bias is the only thing driving the output.

## 4. Generalize into pseudocode

```
function linear_regression_forward(features, weights, bias):
    predictions = []
    for x in features:
        z = sum(x[j] * weights[j] for j in range(len(weights))) + bias
        predictions.append(z)
    return [round(p, 4) for p in predictions]
```

## 5. Complexity, worst vs. desired solution

As with `single-neuron`, there isn't a slow-vs-fast algorithmic gap
here — the forward pass inherently has to touch every feature of every
sample once, so the naive and optimal approaches share the same
asymptotic complexity:

- **Naive (plain Python loops)**: for each of the `n` samples, loop
  over its `d` features to compute the dot product by hand. This is
  **O(n·d)** time and **O(n)** space (for the output predictions) —
  there's no redundant recomputation to eliminate, since each sample's
  dot product is independent and computed exactly once.
- **Optimal (vectorized, e.g. NumPy)**: stack `features` into an
  `n×d` matrix and compute `X @ weights + bias` in a single call. Still
  **O(n·d)** time and **O(n)** space asymptotically — the gain is a
  smaller constant factor from vectorized/BLAS execution, not a better
  Big-O class.

So, like `single-neuron`, the "desired" solution here is about clean,
correct decomposition (dot product → add bias) rather than beating a
brute force — there is no repeated work in the naive version to begin
with.

## 6. Edge cases

- **All-zero weights**: every prediction collapses to exactly `bias`,
  regardless of the input features — traced in section 3.
- **Single feature (`d = 1`)**: the dot product degenerates to a single
  multiplication, `x_i[0] * weights[0] + bias` — still correct, no
  special-casing needed.
- **Single sample (`n = 1`)**: the loop runs once, returning a
  single-element list — same formula, no averaging or aggregation
  across samples to worry about (unlike `single-neuron`'s MSE step,
  this problem stops at the per-sample prediction).
- **Negative weights or features**: the dot product and sum handle
  signs naturally — e.g. sample `i=1` in section 3's main example has a
  negative feature (`-1.0`) multiplied by a positive weight, correctly
  reducing the dot product below what it would be with an all-positive
  input.
