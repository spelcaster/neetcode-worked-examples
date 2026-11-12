# Singly Linked List

Source: https://neetcode.io/problems/singlyLinkedList/question

> **Note:** The neetcode.io page renders its content client-side with
> JavaScript — a plain fetch only returned the page shell/title, not the
> actual statement. The description, examples, and constraints below are
> reconstructed from knowledge of this NeetCode problem, not scraped
> verbatim. Double-check against the live page if precision matters.

## Description

Design and implement a Singly Linked List. Each node stores an integer
value and a pointer/reference to the next node.

Implement the `SinglyLinkedList` class:

- `SinglyLinkedList()` — Initializes the linked list (empty).
- `int get(int index)` — Returns the value of the `index`-th node
  (0-indexed). If `index` is out of bounds (negative, or >= length),
  return `-1`.
- `void insertHead(int val)` — Inserts a node with value `val` as the new
  head of the list.
- `void insertTail(int val)` — Inserts a node with value `val` as the new
  tail of the list.
- `bool remove(int index)` — Removes the node at the given `index`
  (0-indexed). Returns `true` if the removal succeeded, `false` if
  `index` was out of bounds.
- `List<int> getValues()` — Returns the values of all nodes, in order
  from head to tail, as an array/list. (Primarily a test/inspection
  helper, not part of the "core" operation set.)

## Examples

Operations are applied in sequence to a single `SinglyLinkedList`
instance; `getValues()` calls show the resulting state after each
mutation.

```
Input:
  insertHead(2)
  insertHead(1)
  insertTail(3)
  get(1)
  remove(1)
  getValues()

Output:
  get(1) -> 2
  remove(1) -> true
  getValues() -> [1, 3]
```

Explanation:
- `insertHead(2)`  -> list: [2]
- `insertHead(1)`  -> list: [1, 2]
- `insertTail(3)`  -> list: [1, 2, 3]
- `get(1)`         -> index 1 is value 2
- `remove(1)`      -> removes value 2 -> list: [1, 3], returns true
- `getValues()`    -> [1, 3]

```
Input:
  get(0)
  remove(0)

Output:
  get(0) -> -1      (list is empty)
  remove(0) -> false (list is empty, nothing to remove)
```

## Constraints

- `0 <= index , val <= 1000` (typical range; exact bound not critical to
  the logic).
- At most a few thousand calls are made to `insertHead`, `insertTail`,
  `get`, and `remove` combined.
- `get` and `remove` must return a sentinel (`-1` / `false`) rather than
  throwing or crashing when `index` is out of bounds — do not assume
  `index` is always valid.
