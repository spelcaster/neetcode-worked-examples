# Worked Example: Reverse a Linked List

See [problem.md](problem.md) for the full statement.

## 1. Restatement

Given: the head node of a singly linked list, `head -> n2 -> n3 -> ... -> null`.
Determine: the head of the same nodes relinked in the opposite order,
`nk -> ... -> n3 -> n2 -> head -> null`.

## 2. Pick the approach

Each node only points forward (`next`), so to reverse the list every
node's `next` pointer has to be flipped to point at its predecessor
instead. The access pattern is "visit each node once, rewire it, move
on" — a single left-to-right pass with three tracked pointers
(`prev`, `curr`, `next`) is enough: no auxiliary storage is needed because
the only thing that changes is which direction each node points, not
the node's position or value.

## 3. Trace through the sample input

Input: `head = [1, 2, 3, 4, 5]` (i.e. `1 -> 2 -> 3 -> 4 -> 5 -> null`).

Maintain `prev` (reversed portion built so far) and `curr` (node being
rewired). At each step, save `curr.next` before overwriting it, or the
rest of the list is lost.

| step | curr (before) | next = curr.next | curr.next := prev | prev (after) | curr (after) |
|------|---------------|-------------------|--------------------|--------------|--------------|
| 0    | —             | —                  | —                  | null         | 1            |
| 1    | 1             | 2                  | 1 -> null          | 1            | 2            |
| 2    | 2             | 3                  | 2 -> 1             | 2            | 3            |
| 3    | 3             | 4                  | 3 -> 2             | 3            | 4            |
| 4    | 4             | 5                  | 4 -> 3             | 4            | 5            |
| 5    | 5             | null               | 5 -> 4             | 5            | null         |

Loop stops when `curr` is `null`. Return `prev`, which is `5`, and
following `next` pointers from there gives `5 -> 4 -> 3 -> 2 -> 1 -> null`
— matches the expected output `[5, 4, 3, 2, 1]`.

**Edge case trace — empty list**, `head = null`:

| step | curr (before) | next = curr.next | curr.next := prev | prev (after) | curr (after) |
|------|---------------|-------------------|--------------------|--------------|--------------|
| 0    | —             | —                  | —                  | null         | null         |

The loop condition (`curr != null`) is false immediately, so the body
never runs. Return `prev`, which is still `null` — matches the expected
output `[]`, with no special-casing needed.

## 4. Pseudocode

```
function reverseList(head):
    prev = null
    curr = head
    while curr != null:
        next = curr.next   # save before overwriting
        curr.next = prev   # reverse the pointer
        prev = curr        # advance prev
        curr = next         # advance curr
    return prev
```

## 5. Complexity: brute force vs. desired

**Brute force — collect values, rebuild the list.** Walk the list once
to copy every node's value into an array, then walk the array
back-to-front building a brand-new chain of nodes (or reuse the
original nodes but still needs to index them in reverse order, which
means first storing them all). This is O(n) time but O(n) **extra**
space for the array/node list, and it does strictly more work than
necessary: the original nodes already contain everything needed to
rebuild the list, so copying them out and back in again is wasted
effort that only exists because the naive approach doesn't rewire
pointers in place.

**Optimal — in-place pointer reversal (steps 2-4).** O(n) time (one
pass, each node visited exactly once) and O(1) extra space (three
pointers, regardless of list length). The gap: O(n) extra space -> O(1)
by rewiring each node's existing `next` pointer as we pass it instead
of allocating a parallel structure to hold the reversed order.

(A recursive solution is also commonly seen; it reaches the same O(n)
time but costs O(n) space on the call stack, so it shares the brute
force's space complexity despite looking more elegant.)

## 6. Edge cases

- **Empty list (`head = null`)** — traced in step 3: the loop body
  never executes, `prev` stays `null`, which is the correct output `[]`.
- **Single node (`head = [1]`)** — `curr = 1`, `next = null`,
  `curr.next = null` (unchanged), `prev = 1`, `curr = null`; loop ends,
  return `prev = 1`. The list `[1]` is its own reverse, and the
  algorithm does nothing harmful to it.
- **Two nodes (`head = [1, 2]`, example 2)** — same mechanics as the
  5-node trace, stopping after step 2: returns `prev = 2` with
  `2 -> 1 -> null`, matching `[2, 1]`.
- **Already-reversed check isn't needed** — because the algorithm
  always rebuilds the chain from scratch in one direction, there's no
  code path that depends on the input's current order being "forward";
  it would do the same work (and produce the same correct result) if
  handed a list that happened to already be sorted ascending vs.
  descending.
