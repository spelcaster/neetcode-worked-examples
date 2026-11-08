# Buy and Sell Crypto — Worked Example

Problem: https://neetcode.io/problems/buy-and-sell-crypto

**Given:** an array `prices` where `prices[i]` is the price on day `i`.
**Determine:** the maximum profit from buying on one day and selling on a
*strictly later* day. If no profit is possible, return `0`. You may only
hold one buy at a time (buy once, sell once).

---

## Step 1 — Pick the approach

The brute way to think about this is "for every possible buy day, check
every possible later sell day." But notice: for a *fixed* sell day `j`,
the best possible buy day is whichever earlier day had the **lowest
price** — nothing else about the other earlier days matters. That means
we don't need to remember every past price, only the **running minimum**
seen so far.

So: scan once, left to right, carrying two numbers —
- `minPrice` — the lowest price seen so far (the best buy point up to now)
- `bestProfit` — the best profit achievable if we sold at any day seen so far

At each day, selling *today* at `minPrice` would yield `price - minPrice`;
keep the max of that against `bestProfit`.

## Step 2 — Set up the state

```
minPrice   = +infinity   // no day seen yet, so no valid buy price yet
bestProfit = 0           // profit so far; 0 covers "no profit found yet"
```

## Step 3 — Walk through a concrete example

### Worked (profitable) example: `prices = [10, 1, 5, 6, 7, 1]`

| day | price | minPrice (after) | profit if sold today (`price - minPrice`) | bestProfit (after) |
|-----|-------|-------------------|---------------------------------------------|----------------------|
| 0   | 10    | 10                | 10 - 10 = 0                                  | 0                    |
| 1   | 1     | 1                 | 1 - 1 = 0                                     | 0                    |
| 2   | 5     | 1                 | 5 - 1 = 4                                     | 4                    |
| 3   | 6     | 1                 | 6 - 1 = 5                                     | 5                    |
| 4   | 7     | 1                 | 7 - 1 = 6                                     | 6                    |
| 5   | 1     | 1                 | 1 - 1 = 0                                     | 6                    |

End of array. **`bestProfit = 6`** (buy at day 1 for `1`, sell at day 4
for `7`). ✅

### Worked (no-profit) example: `prices = [7, 6, 4, 3, 1]`

| day | price | minPrice (after) | profit if sold today | bestProfit (after) |
|-----|-------|-------------------|------------------------|----------------------|
| 0   | 7     | 7                 | 0                      | 0                    |
| 1   | 6     | 6                 | 0                      | 0                    |
| 2   | 4     | 4                 | 0                      | 0                    |
| 3   | 3     | 3                 | 0                      | 0                    |
| 4   | 1     | 1                 | 0                      | 0                    |

Prices only ever fall, so every day's "sell today" profit is `0` (the
running minimum is always *today's own price*, never an earlier, higher
one). **`bestProfit = 0`** — correctly reports "no profit possible"
without any special-case check. ❌➜0

## Step 4 — The algorithm, generalized

```
function maxProfit(prices):
    minPrice = +infinity
    bestProfit = 0

    for price in prices:
        minPrice = min(minPrice, price)
        bestProfit = max(bestProfit, price - minPrice)

    return bestProfit
```

## Step 5 — Complexity: brute force vs. desired

**Brute force (nested loop):** for every buy day `i`, scan every later
sell day `j > i` and track the best `prices[j] - prices[i]`.
- **Time:** O(n²) — for each of the `n` buy days, you rescan up to `n`
  remaining days.
- **Space:** O(1).
- **Why it's slow:** it keeps re-deriving the same information. Once you
  know the lowest price up to day `j - 1`, rescanning all of `0..j-1`
  again for the next `j` throws that work away and redoes it.

**Desired (single pass with running minimum):**
- **Time:** O(n) — one pass, constant work per element.
- **Space:** O(1) — just two running values.
- **Why it's fast:** it keeps exactly the one fact the brute force kept
  re-deriving (the running minimum) and updates it incrementally instead
  of recomputing it from scratch for every sell day.

**The gap:** O(n²) → O(n), by trading nothing for two O(1) running
variables — no auxiliary data structure needed, just a smarter loop
invariant.

## Step 6 — Edge cases to sanity-check

- **Single-element array** (`[5]`) → `bestProfit` stays `0`: there's no
  later day to sell on, and the loop never finds `price - minPrice > 0`
  since the only price is also the running minimum.
- **Empty array** → `bestProfit` stays `0` (the loop body never runs).
- **Strictly increasing prices** (`[1,2,3,4]`) → buy on day 0, sell on
  the last day; `minPrice` locks to the first price immediately and
  every later day just grows the profit.
- **Strictly decreasing prices** → traced above; answer is `0`.
- **All equal prices** (`[3,3,3]`) → every `price - minPrice` is `0`, so
  `bestProfit` stays `0`, correctly reflecting no profit is possible.
