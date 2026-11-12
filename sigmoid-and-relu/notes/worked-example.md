# Sigmoid and ReLU — Worked Example

Problem: https://neetcode.io/problems/sigmoid-and-relu

## 1. Problem restatement

Given a single real-valued scalar `z`, determine the output of two
activation functions applied to it:

```
sigmoid(z) = 1 / (1 + e^(-z))
relu(z)    = max(0, z)
```

Return both results as a tuple `(sigmoid_value, relu_value)`, each
rounded to 4 decimal places.

## 2. Pick the approach

This is a direct formula evaluation, not a search or optimization
problem — both activations are closed-form functions of a single
number, so there's no data structure involved at all, just careful
arithmetic:

- `sigmoid` squashes any real number into `(0, 1)` — it's smooth and
  always produces a *fractional* probability-like output, even for
  extreme inputs (it just saturates toward 0 or 1).
- `relu` is a simple threshold — it passes positive values through
  unchanged and clips everything at or below 0 to exactly 0, with no
  smoothing at the boundary.

The two functions are independent of each other; computing one doesn't
help or require the other, so the "approach" is just: evaluate each
formula once and round each result.

## 3. Walk through a concrete example

Input: `z = 2.0`

| function  | formula                          | computation                        | result   |
|-----------|-----------------------------------|-------------------------------------|----------|
| `sigmoid` | `1 / (1 + e^(-z))`                 | `1 / (1 + e^(-2.0))` = `1 / 1.1353` | `0.8808` |
| `relu`    | `max(0, z)`                        | `max(0, 2.0)`                       | `2.0000` |

Result: `(0.8808, 2.0)`.

**Edge case trace** — a large-magnitude negative input, `z = -50.0`:

| function  | formula                   | computation                                    | result   |
|-----------|-----------------------------|-------------------------------------------------|----------|
| `sigmoid` | `1 / (1 + e^(-z))`          | `1 / (1 + e^(50.0))` → denominator is astronomically large | `0.0000` |
| `relu`    | `max(0, z)`                  | `max(0, -50.0)`                                 | `0.0000` |

Result: `(0.0, 0.0)` — but for *different reasons*: `sigmoid` never
actually reaches 0, it just rounds down to it after saturating extremely
close to 0; `relu` returns the literal value `0` directly because `z` is
negative. This distinction matters for implementation: computing
`e^(-z)` for a large negative `z` means computing `e^(50.0)`, a huge but
still finite float in Python — not an overflow — whereas for a large
*positive* `z` (e.g. `z = 1000`), `e^(-z)` underflows harmlessly to `0.0`
rather than erroring. The dangerous direction is large positive `z` fed
into `e^(-z)` the *other* way around (i.e. a sign error in the exponent)
— that would compute `e^(+1000)`, which raises `OverflowError` in
Python. A correct implementation never takes that path, but it's the
one failure mode worth checking for in review.

## 4. Generalize into pseudocode

```
function sigmoid_and_relu(z):
    sigmoid_value = 1 / (1 + exp(-z))
    relu_value = max(0, z)
    return (round(sigmoid_value, 4), round(relu_value, 4))
```

## 5. Complexity, worst vs. desired solution

There's no brute-force-vs-optimized gap here — both functions are O(1)
closed-form evaluations on a single scalar, so there's exactly one
reasonable algorithm, not a slow version to improve on. The only
meaningful implementation choice is *numerical robustness* of `sigmoid`,
not asymptotic complexity:

- **Naive `sigmoid`**: compute `1 / (1 + exp(-z))` directly as written.
  This is **O(1)** time and space, and is correct for the vast majority
  of inputs — but for very large positive `z` (e.g. `z = 1000`), `exp(-z)`
  underflows to `0.0`, which is fine; the real risk is a transposed
  implementation that computes `exp(z)` somewhere for large positive
  `z`, which overflows.
- **Numerically stable `sigmoid` (desired)**: branch on the sign of `z`
  — for `z >= 0` use `1 / (1 + exp(-z))` (exponent is always ≤ 0, safe),
  and for `z < 0` use the algebraically equivalent `exp(z) / (1 +
  exp(z))` (exponent is again always ≤ 0, safe). Still **O(1)** time and
  space — the gap isn't Big-O, it's that the stable version guarantees
  the exponent argument never grows large and positive, regardless of
  which direction `z` is extreme.

`relu` has no such pitfall — `max(0, z)` is already numerically trivial
in both directions.

## 6. Edge cases

- **`z = 0`**: `sigmoid(0) = 1 / (1 + e^0) = 1/2 = 0.5` (the function's
  inflection point); `relu(0) = max(0, 0) = 0` (the threshold boundary
  itself, not just "near" it).
- **Large positive `z`** (e.g. `z = 50`): `sigmoid` saturates toward
  `1.0` (never reaching it exactly, but rounding to it at 4 decimals);
  `relu` passes `z` through unchanged.
- **Large negative `z`** (e.g. `z = -50`, traced in section 3): `sigmoid`
  saturates toward `0.0`; `relu` clips to exactly `0.0` — same rounded
  output as sigmoid here, but for structurally different reasons, as
  discussed above.
- **Non-integer / fractional `z`** (e.g. `z = 0.37`): both formulas
  apply exactly as written — no special-casing for fractional input is
  needed, since both functions are defined over all real numbers.
