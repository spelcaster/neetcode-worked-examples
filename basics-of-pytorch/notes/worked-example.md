# Basics of PyTorch — Worked Example

Problem: [notes/problem.md](problem.md)

## 1. Problem restatement

Given inputs that are already `torch.Tensor` objects, determine the
result of four PyTorch tensor operations — reshape, column-wise mean,
column-wise concatenation, and mean-squared-error loss — and return
each result as a tensor, rounded to 4 decimal places
(`torch.round(tensor, decimals=4)`).

## 2. Pick the approach

None of these four operations require a hand-rolled algorithm — the key
insight is that PyTorch already expresses each one as a single built-in
tensor op, so the "approach" is: call the one matching op on the input
tensor(s), then round the result:

| operation     | shape insight                                  | PyTorch op                 |
|----------------|------------------------------------------------|------------------------------|
| `reshape`      | total element count is fixed; only the layout (rows × cols) changes | `torch.reshape(t, (-1, 2))` |
| `average`      | collapsing dimension 0 means "one value per column" | `torch.mean(t, dim=0)`      |
| `concatenate`  | joining along dim 1 means "same rows, more columns" | `torch.cat((t1, t2), dim=1)` |
| `get_loss`     | elementwise squared difference, then averaged  | `torch.nn.functional.mse_loss(pred, target)` |

## 3. Walk through a concrete example

**`reshape`** — input `to_reshape = tensor([[1., 2., 3., 4., 5., 6.]])`
(shape `(1, 6)`, 6 elements total), target shape `(-1, 2)`:

| step | action                           | tensor state                              |
|------|-----------------------------------|---------------------------------------------|
| 1    | start                             | shape `(1, 6)`: `[[1,2,3,4,5,6]]`           |
| 2    | `torch.reshape(t, (-1, 2))`       | `-1` infers rows = `6 / 2 = 3` → shape `(3, 2)`: `[[1,2],[3,4],[5,6]]` |
| 3    | `torch.round(result, decimals=4)` | unchanged (already exact) → `tensor([[1,2],[3,4],[5,6]])` |

**`average`** — input `to_avg = tensor([[1., 2.], [3., 4.]])` (shape `(2, 2)`):

| step | action                  | tensor state                      |
|------|---------------------------|--------------------------------------|
| 1    | start                     | `[[1,2],[3,4]]`, shape `(2, 2)`      |
| 2    | `torch.mean(t, dim=0)`    | average each column: `[(1+3)/2, (2+4)/2] = [2.0, 3.0]`, shape `(2,)` |

**Edge case trace** — `get_loss` with a mismatch, `prediction =
tensor([1., 2., 3.])`, `target = tensor([1., 2., 4.])`:

| step | action                             | value                              |
|------|-------------------------------------|---------------------------------------|
| 1    | elementwise diff                    | `[0.0, 0.0, -1.0]`                    |
| 2    | square                               | `[0.0, 0.0, 1.0]`                      |
| 3    | mean (`mse_loss`)                    | `(0+0+1)/3 = 0.33333...`              |
| 4    | `torch.round(loss, decimals=4)`      | `0.3333`                               |

Unlike `reshape`/`average`/`concatenate`, `get_loss` collapses to a
**scalar** — a 0-dimensional tensor, `shape = ()`, not `shape = (1,)`.
`torch.round` still works fine on a 0-d tensor, but code that assumes
the result is indexable (e.g. `result[0]`) would break here.

## 4. Generalize into pseudocode

```
function reshape(to_reshape):
    r = torch_reshape(to_reshape, (-1, 2))
    return torch_round(r, decimals=4)

function average(to_avg):
    m = torch_mean(to_avg, dim=0)
    return torch_round(m, decimals=4)

function concatenate(cat_one, cat_two):
    c = torch_cat((cat_one, cat_two), dim=1)
    return torch_round(c, decimals=4)

function get_loss(prediction, target):
    loss = mse_loss(prediction, target)
    return torch_round(loss, decimals=4)
```

## 5. Complexity, worst vs. desired solution

- **Naive (manual Python loops over tensor elements)**: someone new to
  PyTorch might iterate over the tensor's elements in a Python `for`
  loop to reshape/average/concatenate by hand — e.g. building the
  reshaped structure by manually slicing every 2 elements, or summing
  columns with a loop and dividing by row count. This is **O(n)** time
  either way (every element is visited once), the same asymptotic
  complexity as the tensor ops below — but it drops out of PyTorch's
  vectorized C/CUDA execution into per-element Python-level iteration
  (slow in practice despite equal Big-O), and it reimplements logic the
  library already provides, risking off-by-one slicing or the wrong
  reduction axis for no complexity benefit.
- **Tensor ops (desired, as in the starter code's hints)**:
  `torch.reshape`, `torch.mean`, `torch.cat`, and
  `torch.nn.functional.mse_loss` are each still **O(n)** time (and O(n)
  space for the output, O(1) extra beyond that) — reshape doesn't even
  copy data when the input is contiguous, it's a view — but the work
  happens as one vectorized call instead of n Python-level iterations,
  with axis/shape bookkeeping handled by the library instead of
  hand-written loop bounds.

Gap: both are **O(n)**, so this isn't a Big-O win — the gap is
correctness and performance-per-element: one call to a batched,
axis-aware, vectorized primitive versus several hand-written loops that
each risk an off-by-one or wrong-axis bug and lose vectorization.

## 6. Edge cases

- **`reshape` with a size that isn't evenly divisible by 2**: letting
  `-1` infer the row count only works when the total element count is
  divisible by 2; an odd total would make `torch.reshape(t, (-1, 2))`
  raise a runtime error — the problem's constraints assume this doesn't
  happen, but the failure mode is a hard error, not silent truncation.
- **`average` on a single row** (`to_avg = tensor([[1., 2.]])`):
  "averaging" over `dim=0` with only one row just returns that row
  unchanged, `[1.0, 2.0]` — not an error, since the mean of one value is
  itself.
- **`concatenate` with zero-width inputs** (`cat_one =
  tensor([[]]`), `cat_two = tensor([[]])`\)): concatenating along `dim=1`
  yields shape `(1, 0)` — one row, zero columns, not an error, since
  `dim=0` (the row count) still matches between the two inputs.
- **`get_loss` with identical prediction and target**: every elementwise
  diff is `0`, so the squared mean is exactly `0.0` — the scalar-shape
  (`shape = ()`) edge case from section 3 still applies here.
- **Rounding a value that's already exact** (traced in `reshape`
  above): `torch.round(x, decimals=4)` on an integer-valued float like
  `2.0` leaves it as `2.0` — rounding is a no-op whenever the value
  already has 4 or fewer decimal digits.
