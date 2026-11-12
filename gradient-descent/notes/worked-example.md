# Gradient Descent — Worked Example

Problem: https://neetcode.io/problems/gradient-descent

## 1. Problem restatement

Given:
- `iterations`: the number of gradient descent update steps to perform.
- `learning_rate`: the step size `α` used in each update.
- `init`: the starting value `x₀`.

Determine: the value of `x` after running `iterations` steps of gradient
descent on the objective function `f(x) = x²`, where each step updates
`x` using its gradient:

```
x = x - learning_rate * f'(x)
f'(x) = 2x
```

Return the final `x`, rounded to 5 decimal places.

## 2. Pick the approach

This is a direct simulation, not a search problem: gradient descent on a
fixed, known function is just "repeatedly nudge `x` opposite its slope."
The key insight is that `f(x) = x²` has a closed-form derivative (`2x`),
so each step is a pure arithmetic update — no numerical differentiation,
no stopping condition to detect convergence, just run exactly
`iterations` steps and stop.

```
repeat `iterations` times:
    gradient = 2 * x
    x = x - learning_rate * gradient
```

Each step depends only on the current `x`, so a single running variable
updated in a loop is all the state needed — no array, stack, or other
structure required.

## 3. Walk through a concrete example

Input: `iterations = 3`, `learning_rate = 0.1`, `init = 5`

| step | x (before) | gradient = 2x | update: x - 0.1·gradient | x (after) |
|------|------------|----------------|---------------------------|-----------|
| 1    | 5.0        | 10.0           | 5.0 - 0.1·10.0 = 4.0      | 4.0       |
| 2    | 4.0        | 8.0            | 4.0 - 0.1·8.0 = 3.2       | 3.2       |
| 3    | 3.2        | 6.4            | 3.2 - 0.1·6.4 = 2.56      | 2.56      |

Result: `2.56000`. Each step shrinks `x` toward 0 (the true minimizer of
`x²`) by 20% of its current value, since `x - 0.1·(2x) = x·(1 - 0.2) =
0.8x` — a predictable geometric decay toward the minimum.

**Edge case trace** — a learning rate too large for the curvature of
`f(x) = x²` causes *divergence* instead of convergence:

Input: `iterations = 3`, `learning_rate = 1.1`, `init = 5`

| step | x (before) | gradient = 2x | update: x - 1.1·gradient   | x (after) |
|------|------------|----------------|------------------------------|-----------|
| 1    | 5.0        | 10.0           | 5.0 - 1.1·10.0 = -6.0        | -6.0      |
| 2    | -6.0       | -12.0          | -6.0 - 1.1·(-12.0) = 7.2     | 7.2       |
| 3    | 7.2        | 14.4           | 7.2 - 1.1·14.4 = -8.64       | -8.64     |

Result: `-8.64000`. Here `x - 1.1·(2x) = x·(1 - 2.2) = -1.2x`, so each
step *overshoots* past 0 and grows in magnitude — the algorithm is still
applied exactly as specified, it just doesn't converge for this
learning rate. This shows the algorithm has no built-in safety check: it
blindly executes the fixed number of steps regardless of whether they
move `x` toward or away from the minimum.

## 4. Generalize into pseudocode

```
function get_minimizer(iterations, learning_rate, init):
    x = init
    repeat iterations times:
        gradient = 2 * x
        x = x - learning_rate * gradient
    return round(x, 5)
```

## 5. Complexity, worst vs. desired solution

There's no brute-force-vs-optimized gap here the way there is for, say,
searching or two-pointer problems — the task specifies an exact fixed
number of update steps, so there is exactly one algorithm to run, not a
slow version to improve on. The only "naive" mistake would be reaching
for something more complicated than the loop itself, e.g. symbolically
re-deriving `f'(x)` at every step via a numerical-differentiation
approximation (`(f(x+h) - f(x)) / h`) instead of using the closed-form
`2x` directly.

- **Unnecessary numerical differentiation**: computing the gradient via
  a finite-difference approximation each step is still **O(iterations)**
  time and **O(1)** space, but it introduces floating-point
  approximation error at every step for no benefit, since the true
  derivative is already known and exact.
- **Direct closed-form update (desired)**: **O(iterations)** time and
  **O(1)** space — the same asymptotic cost, but exact at every step
  since `f'(x) = 2x` is computed directly rather than approximated.

So the "gap" here isn't Big-O at all (both are `O(iterations)` / `O(1)`)
— it's correctness/precision: using the exact derivative avoids
compounding approximation error over many iterations.

## 6. Edge cases

- **`iterations = 0`**: the loop body never runs, so the function
  returns `init` unchanged, rounded to 5 decimals — gradient descent
  with no steps is a no-op.
- **`init = 0`**: the gradient `2·0 = 0` on the very first step, so `x`
  stays `0` forever regardless of `iterations` or `learning_rate` — `0`
  is the exact minimizer of `x²`, so there's nothing to descend toward.
- **Learning rate too large (`learning_rate > 1`)**: as traced in
  section 3's edge case, the update factor `(1 - 2·learning_rate)`
  becomes less than `-1` in magnitude, so `x` oscillates in sign and
  grows without bound instead of converging to 0.
- **Negative `init`**: by symmetry of `f(x) = x²`, the gradient `2x` is
  negative, so the update step `x - learning_rate·(2x)` still moves `x`
  toward 0 from the negative side — the algorithm doesn't need to treat
  sign as a special case.
