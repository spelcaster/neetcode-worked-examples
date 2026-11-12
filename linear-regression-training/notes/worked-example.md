# Linear Regression: Training — Worked Example

Problem: https://neetcode.io/problems/linear-regression-training

## 1. Problem restatement

Given:
- `X`: an `n × d` design matrix — `n` samples, `d` columns, where the
  **first column is all 1s** (so the intercept/bias is folded into
  `theta` rather than tracked separately, unlike `linear-regression-forward`).
- `y`: a length-`n` vector of true target values.
- `alpha`: the learning rate.
- `iterations`: the number of gradient descent steps to run.

Determine: the coefficient vector `theta` (length `d`) that results
from running batch gradient descent on the mean-squared-error loss,
starting from `theta = [0, 0, ..., 0]`.

Return the final `theta`, rounded to 4 decimal places.

## 2. Pick the approach

This chains together two things already derived in this repo:
`linear-regression-forward`'s prediction formula, and `gradient-descent`'s
repeated-update loop — but now the "function being minimized" is MSE
loss over the *whole dataset* instead of a single scalar `x²`, so the
gradient is a vector, not a single number.

For each iteration:
```
predictions = X · theta                          # one prediction per sample
errors      = predictions - y                     # how far off each prediction is
gradient    = (1/n) * X^T · errors                 # average error, weighted by each feature
theta       = theta - alpha * gradient
```

The key insight: `errors` is a single length-`n` vector shared across
all `d` coefficients — computing it once per iteration (not once per
coefficient) is what makes this a single matrix-vector multiply rather
than `d` separate passes over the data.

## 3. Walk through a concrete example

Input: `X = [[1,1],[1,2],[1,3]]`, `y = [2,4,6]` (a perfect line
`y = 2x`), `alpha = 0.1`, `iterations = 2`, `theta = [0, 0]`.

**Iteration 1** (`theta = [0, 0]`):

| i | pred_i = X_i·theta | error_i = pred_i - y_i |
|---|----------------------|---------------------------|
| 0 | 0                    | -2                        |
| 1 | 0                    | -4                        |
| 2 | 0                    | -6                        |

`gradient = (1/3) * X^T·errors = (1/3) * [sum(errors), sum(x_i·error_i)]`
`= (1/3) * [-12, (1·-2 + 2·-4 + 3·-6)] = (1/3) * [-12, -28] = [-4, -9.3333]`

`theta = [0,0] - 0.1·[-4, -9.3333] = [0.4, 0.9333]`

**Iteration 2** (`theta = [0.4, 0.9333]`):

| i | pred_i = 0.4 + 0.9333·x_i | error_i = pred_i - y_i |
|---|-----------------------------|---------------------------|
| 0 | 1.3333                      | -0.6667                   |
| 1 | 2.2667                      | -1.7333                   |
| 2 | 3.2000                      | -2.8000                   |

`gradient = (1/3) * [sum(errors), sum(x_i·error_i)] = (1/3) * [-5.2, -12.5333] = [-1.7333, -4.1778]`

`theta = [0.4, 0.9333] - 0.1·[-1.7333, -4.1778] = [0.5733, 1.3511]`

Result: `theta = [0.5733, 1.3511]` — after just 2 steps, it's already
converging toward the true relationship (`theta_0 = 0`, `theta_1 = 2`),
but hasn't gotten there yet (gradient descent on this loss needs many
more iterations to converge fully).

**Edge case trace** — `iterations = 0`:

The loop body never executes, so `theta` stays at its initial value
`[0, 0]` — training for zero steps is a no-op, same as the
`iterations = 0` edge case in `gradient-descent`.

## 4. Generalize into pseudocode

```
function linear_regression_training(X, y, alpha, iterations):
    n = len(y)
    theta = zeros(len(X[0]))
    repeat iterations times:
        predictions = X · theta
        errors = predictions - y
        gradient = (1/n) * (X^T · errors)
        theta = theta - alpha * gradient
    return round(theta, 4)
```

## 5. Complexity, worst vs. desired solution

Each iteration inherently has to touch every one of the `n·d` entries
of `X` at least once (to compute predictions, then again to compute
the gradient), so the naive and optimal approaches share the same
asymptotic bound — the gap is about avoiding *wasted* repeated work
within that bound, not a different Big-O class:

- **Naive**: inside the iteration loop, recompute `X^T` (the transpose)
  from scratch every iteration before using it in the gradient step,
  even though `X` itself never changes across iterations. This is
  **O(iterations · n · d)** time and **O(n·d)** space overall, but with
  the transpose's `O(n·d)` cost paid redundantly `iterations` times for
  no reason — `X^T` is identical every time.
- **Optimal (desired)**: compute `X^T` **once**, before the loop
  starts, and reuse that same transposed matrix on every iteration.
  Still **O(iterations · n · d)** time overall (the per-iteration
  matrix-vector multiplies are unavoidable — that's the actual
  gradient-descent work), but the wasted re-transposition is eliminated
  entirely, saving an extra `O(n·d)` pass on every iteration after the
  first.

So the gap here isn't "O(n²) → O(n)" in the classic sense — it's
"stop redoing `iterations`-many copies of the same O(n·d) setup work
that only needed to happen once."

## 6. Edge cases

- **`iterations = 0`**: `theta` returned unchanged as `[0, 0, ..., 0]`
  — traced in section 3.
- **Learning rate too large**: as with `gradient-descent`, an `alpha`
  that's too large relative to the data's scale can cause `theta` to
  oscillate or diverge instead of converge — the algorithm has no
  built-in safety check and will run exactly `iterations` steps
  regardless of whether they help.
- **Perfectly linear data with enough iterations** (the setup in
  section 3, run longer): `theta` converges toward the exact underlying
  `[intercept, slope]` — here `[0, 2]` — since the MSE loss has a single
  global minimum for linearly separable, noise-free data.
- **Single feature column, i.e. `X` is just the bias column**
  (`d = 1`, every row `[1]`): the model degenerates to predicting the
  same constant for every sample — gradient descent on `theta_0` alone
  converges to `mean(y)`, the value that minimizes squared error against
  a constant prediction.
