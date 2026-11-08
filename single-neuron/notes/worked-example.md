# Single Neuron — Worked Example

Problem: https://neetcode.io/problems/single-neuron

## 1. Problem restatement

Given:
- `features`: a list of `n` feature vectors, each of length `d` (a batch of samples).
- `labels`: a list of `n` true binary labels (0 or 1), one per sample.
- `weights`: a length-`d` vector of weights.
- `bias`: a single scalar bias.

Determine: for each sample, run it through one neuron — a weighted sum of
its features plus a bias, passed through the sigmoid activation — to get
a predicted probability. Then compute the mean squared error (MSE)
between the predicted probabilities and the true labels.

Return a tuple `(probabilities, mse)`, where `probabilities` is the list
of per-sample sigmoid outputs and `mse` is a single float — both rounded
to 4 decimal places.

## 2. Pick the approach

This is a direct simulation, not a search or optimization problem: the
neuron's forward pass is a fixed formula applied independently to each
sample. The key insight is just decomposing the formula correctly:

```
z_i    = (x_i · weights) + bias      # linear combination
prob_i = sigmoid(z_i) = 1 / (1 + e^-z_i)
mse    = mean((label_i - prob_i)^2 for all i)
```

Each sample's probability depends only on its own feature vector, the
shared weights, and the shared bias — so a single pass over the samples
(computing one dot product each) is all that's needed. No data structure
beyond a list accumulator is required.

## 3. Walk through a concrete example

Input:
```
features = [[0.5, 1.0], [-1.5, -2.0], [2.0, 1.5]]
labels   = [0, 1, 0]
weights  = [0.7, -0.4]
bias     = -0.1
```

| i | x_i            | z_i = x_i·w + bias                          | prob_i = sigmoid(z_i) | label_i | (label_i - prob_i)^2 |
|---|----------------|----------------------------------------------|-----------------------|---------|-----------------------|
| 0 | [0.5, 1.0]     | 0.5·0.7 + 1.0·(-0.4) - 0.1 = -0.15            | 0.4626                | 0       | 0.2140                |
| 1 | [-1.5, -2.0]   | -1.5·0.7 + -2.0·(-0.4) - 0.1 = -0.35          | 0.4134                | 1       | 0.3441                |
| 2 | [2.0, 1.5]     | 2.0·0.7 + 1.5·(-0.4) - 0.1 = 0.70             | 0.6682                | 0       | 0.4465                |

`mse = (0.2140 + 0.3441 + 0.4465) / 3 = 1.0046 / 3 = 0.3349`

Result: `([0.4626, 0.4134, 0.6682], 0.3349)`.

**Edge case trace** — an all-zero sample with zero weights and zero bias:

```
features = [[0.0, 0.0]]
labels   = [1]
weights  = [0.0, 0.0]
bias     = 0.0
```

| i | x_i        | z_i | prob_i = sigmoid(0) | label_i | (label_i - prob_i)^2 |
|---|------------|-----|----------------------|---------|-----------------------|
| 0 | [0.0, 0.0] | 0.0 | 0.5000               | 1       | 0.2500                |

`mse = 0.2500 / 1 = 0.2500` → result `([0.5], 0.25)`.

This shows the "no information" behavior: with nothing pushing `z` away
from 0, sigmoid outputs exactly 0.5 (maximum uncertainty), regardless of
what the true label is.

## 4. Generalize into pseudocode

```
function single_neuron_model(features, labels, weights, bias):
    probabilities = []
    for x in features:
        z = sum(x[j] * weights[j] for j in range(len(weights))) + bias
        prob = 1 / (1 + exp(-z))
        probabilities.append(prob)

    mse = sum((labels[i] - probabilities[i])**2 for i in range(len(labels))) / len(labels)

    return (round each prob in probabilities to 4 decimals, round(mse, 4))
```

## 5. Complexity, worst vs. desired solution

There isn't a "slow" and a "fast" algorithm here the way there is for,
say, searching — the forward pass of a single neuron inherently has to
touch every feature of every sample once, so the naive approach and the
optimal approach share the same asymptotic complexity. The two real
implementation choices are:

- **Naive (plain Python loops)**: for each of the `n` samples, loop over
  its `d` features to compute the dot product by hand, then apply
  `exp()`. This is **O(n·d)** time and **O(n)** space (to hold the output
  probabilities) — there's no redundant recomputation to eliminate,
  because each sample's dot product is independent and only computed
  once.
- **Optimal (vectorized, e.g. NumPy)**: stack `features` into an `n×d`
  matrix and compute `X @ weights + bias` in one call, then apply
  sigmoid elementwise. This is still **O(n·d)** time and **O(n)** space
  asymptotically — the gain is a smaller constant factor from
  vectorized/BLAS execution, not a better Big-O class.

So the "desired" solution here is really about correctness and clean
decomposition (dot product → sigmoid → squared error → mean) rather than
beating a brute force. The one genuine pitfall is numerical: computing
`exp(-z)` for a very large positive `z` can overflow before the division
collapses it back toward 0 (Python's `math.exp` raises `OverflowError`
on sufficiently large arguments, unlike languages that would just return
`inf`). A stable sigmoid branches on the sign of `z` to avoid ever
exponentiating a large positive number.

## 6. Edge cases

- **Single sample (`n = 1`)**: the loop and the mean both degenerate
  gracefully — `mse` is just the one squared error, not an average.
  Traced in section 3's edge case.
- **All-zero weights and bias**: every `z_i = 0`, so every `prob_i =
  0.5` regardless of the input features — the neuron outputs pure
  uncertainty. Traced in section 3's edge case.
- **Very large positive or negative `z`**: `sigmoid` should saturate
  toward 1 or 0 respectively without raising an overflow error — this is
  the numerical pitfall from section 5, handled by a sign-aware
  implementation of sigmoid.
- **Rounding**: both the probabilities and the MSE must be rounded to
  4 decimal places independently; rounding `mse` from unrounded
  probabilities (not from the already-rounded display values) avoids
  compounding rounding error, so compute MSE from the full-precision
  probabilities first, then round both outputs at the end.
