# Singly Linked List — Worked Example

Problem: [../notes/problem.md](../notes/problem.md)

**Given:** a sequence of operations (`insertHead`, `insertTail`, `get`,
`remove`, `getValues`) to apply to a `SinglyLinkedList`.
**Determine:** the return value of each query operation, and the final
contents of the list.

---

## Step 1 — Pick the data structure

The name of the problem *is* the data structure: a chain of nodes, each
holding a value and a pointer to the next node, with no random access.
That shape drives every operation:

- **`insertHead`** only ever touches the first node → naturally O(1) if
  we keep a `head` pointer.
- **`insertTail`** only ever touches the last node → O(1) too, but
  *only* if we keep a separate `tail` pointer. Without one, reaching the
  end means walking the whole list first.
- **`get(index)` / `remove(index)`** have no way to "jump" to a
  position — a singly linked list only knows "this node, then the next
  one". So both must walk node-by-node from `head` until they've taken
  `index` steps.

So the structure is: a `Node { val, next }`, plus `head`, `tail`, and a
`size` counter on the list itself (size makes bounds-checking `index`
an O(1) comparison instead of a full walk).

## Step 2 — Set up the state

```
Node { val: int, next: Node? }

SinglyLinkedList:
    head: Node? = null
    tail: Node? = null
    size: int = 0
```

## Step 3 — Walk through a concrete example

Using the first example from `notes/problem.md`:
`insertHead(2); insertHead(1); insertTail(3); get(1); remove(1); getValues()`

| step | operation      | action                                                                 | head → ... → tail | size |
|------|----------------|-------------------------------------------------------------------------|--------------------|------|
| 1    | `insertHead(2)`| list empty → new node `2` becomes both `head` and `tail`                | `2`                | 1    |
| 2    | `insertHead(1)`| new node `1`, `1.next = head (2)`, `head = 1`; `tail` unchanged          | `1 → 2`            | 2    |
| 3    | `insertTail(3)`| new node `3`, `tail.next = 3` (i.e. `2.next = 3`), `tail = 3`            | `1 → 2 → 3`        | 3    |
| 4    | `get(1)`       | `index=1` is in bounds (`0 <= 1 < 3`); walk from `head`: step 0 at `1`, step 1 at `2` → return `2` | `1 → 2 → 3` (unchanged) | 3 |
| 5    | `remove(1)`    | in bounds; walk to the node **before** index 1 (`prev = node 1`); unlink: `prev.next = prev.next.next` (`1.next = 3`); if the removed node was `tail`, fix `tail` (not needed here) → return `true` | `1 → 3` | 2 |
| 6    | `getValues()`  | walk from `head` to `null`, collecting values                           | `1 → 3`            | 2    |

Result: `get(1) -> 2`, `remove(1) -> true`, `getValues() -> [1, 3]` —
matches `notes/problem.md`.

### Failing / different-behavior trace: empty list

Using the second example from `notes/problem.md`:
`get(0); remove(0)` on a **fresh, empty** list (`head = tail = null`,
`size = 0`).

| step | operation  | action                                                          | result  |
|------|------------|------------------------------------------------------------------|---------|
| 1    | `get(0)`   | bounds check first: `0 <= 0` but `0 < size (0)` is **false** → out of bounds, never walks the list | `-1`    |
| 2    | `remove(0)`| same bounds check fails (`size == 0`) → never touches `head`/`tail` | `false` |

This is the behavior that makes the bounds check matter: without
checking `index < size` *before* walking, both operations would try to
dereference a `null` `head` and crash instead of returning the sentinel.

## Step 4 — Generalize into pseudocode

```
function get(index):
    if index < 0 or index >= size: return -1
    node = head
    repeat index times: node = node.next
    return node.val

function insertHead(val):
    node = Node(val, next = head)
    head = node
    if tail is null: tail = node     # list was empty
    size += 1

function insertTail(val):
    node = Node(val, next = null)
    if tail is null:                 # list was empty
        head = node
    else:
        tail.next = node
    tail = node
    size += 1

function remove(index):
    if index < 0 or index >= size: return false
    size -= 1
    if index == 0:
        head = head.next
        if head is null: tail = null   # list became empty
        return true
    prev = head
    repeat (index - 1) times: prev = prev.next
    removed = prev.next
    prev.next = removed.next
    if removed == tail: tail = prev
    return true

function getValues():
    result = []
    node = head
    while node is not null:
        result.append(node.val)
        node = node.next
    return result
```

## Step 5 — Complexity: brute force vs. desired

**Brute force (no `tail` pointer, no `size` counter):**
- `insertHead`: O(1) — unaffected either way.
- `insertTail`: **O(n)** — must walk from `head` all the way to the last
  node (the one whose `next` is `null`) before linking the new node.
- `get` / `remove`: O(index) to walk, **plus** an O(n) walk up front
  just to validate `index` against the list's length (no `size` field to
  check it in O(1)).
- This repeats work every call: `insertTail` re-discovers "where the end
  is" from scratch each time, even though the list already knew it was
  there a moment ago at the last `insertTail`/`insertHead` call.

**Desired (maintain `tail` and `size` incrementally):**
- `insertHead`: O(1).
- `insertTail`: **O(1)** — `tail` already points at the last node, no
  walk needed.
- `get` / `remove`: **O(index)** to walk to the target node — this part
  is inherent to a *singly* linked list (no random access) and isn't
  something the optimal solution avoids, but bounds-checking `index`
  against `size` first is O(1) instead of an extra O(n) walk, so an
  invalid call fails fast instead of walking first and failing late.
- Trade: O(1) extra space for the `tail` pointer and `size` counter,
  buys O(n) → O(1) on every `insertTail` call and fast-fails invalid
  `get`/`remove` calls.

## Step 6 — Edge cases

- **Empty list** (`head = tail = null`, `size = 0`): traced in Step 3 —
  `get`/`remove` must return their sentinel (`-1`/`false`) via the
  `size` check, never dereference `head`.
- **`index` out of bounds** (negative, or `>= size`): same bounds check
  as the empty-list case; must reject *before* walking, since walking an
  out-of-range index would run off the end (`next` of the last node is
  `null`) and crash on a null dereference.
- **`remove` the head (`index == 0`)**: no `prev` node exists — handled
  as a special case in Step 4's pseudocode (`head = head.next`) rather
  than trying to walk to a "node before index 0".
- **`remove` the tail**: after unlinking, the new last node must become
  the new `tail` — forgetting this leaves `tail` pointing at a removed
  node, corrupting the next `insertTail`.
- **List becomes empty after `remove`**: removing the only remaining
  node must reset *both* `head` and `tail` to `null` (Step 4's
  `if head is null: tail = null` after removing index 0 of a
  single-element list).
- **Single-element list**: `insertHead`/`insertTail` on an empty list
  both must set `head` *and* `tail` to the same new node — easy to miss
  if you only update one of them.
