# Vectorized vs. Loop-Based Softmax

Context: `softmax/python/src/solution.py` was rewritten from an explicit Python-loop
implementation to a vectorized NumPy implementation. Both compute the same
numerically-stable softmax, but the new version is better for several concrete reasons.

## Before

```python
maxVal = max(z)

z -= maxVal

k = []

for v in z:
    k.append(np.exp(v))

sumZ = np.sum(k)

result = []

for v in z:
    j = np.round(np.exp(v) / sumZ, 4)

    result.append(j)

return np.array(result, dtype=np.float64)
```

## After

```python
shifted = z - np.max(z)
exps = np.exp(shifted)
return np.round(exps / np.sum(exps), 4)
```

## Why the new version is better

### 1. Vectorization instead of Python-level loops
The old code iterates over `z` twice in plain Python (`for v in z: ...`), calling
`np.exp` once per element. Each iteration pays Python's interpreter overhead on top
of the NumPy call. The new code calls `np.exp(shifted)` once on the whole array,
letting NumPy run the exponentiation in a single vectorized C loop. For any
non-trivial array size this is significantly faster and scales much better.

### 2. No redundant computation
The old code computes `np.exp(v)` **twice** per element:
- once while building `k` (to get `sumZ`)
- once again while building `result` (to get `j`)

The new code computes `exps = np.exp(shifted)` once and reuses it both for the sum
and for the final division. Same result, half the exponentiations.

### 3. Fewer intermediate Python objects
The old code builds two plain Python lists (`k` and `result`), then wraps `result` in
`np.array(..., dtype=np.float64)` at the end. That's extra allocations and type
conversions (NumPy scalar -> Python list element -> back to NumPy array). The new
code stays in NumPy array land the whole time (`shifted`, `exps`, and the final
division all are/produce arrays directly), avoiding that round-trip.

### 4. In-place mutation of the input removed
The old code does `z -= maxVal`, mutating the caller's input array in place as a side
effect — surprising behavior for a "pure" math function. The new code computes
`shifted = z - np.max(z)`, which creates a new array and leaves the caller's `z`
untouched.

### 5. Readability
The new version reads almost exactly like the mathematical definition of softmax:

```
softmax(z)_i = exp(z_i - max(z)) / sum(exp(z - max(z)))
```

with one line per step (shift, exponentiate, normalize), instead of manual
accumulation loops that obscure that structure.

## Takeaway

Same algorithm (max-subtraction trick for numerical stability), but the rewrite
leans on NumPy's vectorized operations to be faster, avoid duplicate work, avoid
unnecessary allocations, and avoid mutating the input — while also being shorter
and closer to the mathematical formula it implements.
