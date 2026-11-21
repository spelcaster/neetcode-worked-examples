# C++ STL `priority_queue`

Reference: https://www.geeksforgeeks.org/cpp/implement-min-heap-using-stl/

## What it is

`std::priority_queue` is a **container adapter** — it doesn't implement its own
storage, it wraps another container (by default `std::vector`) and exposes a
restricted interface that always keeps the "highest priority" element
accessible in O(1) at the top. Internally it maintains a **binary heap** over
the underlying container, so insertion and removal are O(log n).

```cpp
#include <queue>

template<
  class T,
  class Container = std::vector<T>,
  class Compare = std::less<T>
> class priority_queue;
```

- `T` — the element type stored.
- `Container` — the underlying storage (`std::vector` by default; must support
  `front()`, `push_back()`, `pop_back()` — `std::deque` also works).
- `Compare` — the ordering used to decide priority.

## Max-heap by default

With no `Compare` specified, `priority_queue<T>` is a **max-heap**: `top()`
always returns the *largest* element according to `std::less<T>`.

```cpp
std::priority_queue<int> pq; // max-heap
pq.push(3);
pq.push(1);
pq.push(4);
pq.top(); // 4
```

## Min-heap via `std::greater`

To get a **min-heap** (smallest element on top), pass `std::greater<T>` as the
comparator. This flips the ordering `Compare` uses internally — it doesn't
change what `top()` means (still "highest priority"), it changes which
element counts as highest priority.

```cpp
std::priority_queue<int, std::vector<int>, std::greater<int>> pq; // min-heap
pq.push(3);
pq.push(1);
pq.push(4);
pq.top(); // 1
```

This is the form used for Dijkstra's algorithm (see
[`dijkstra/cpp/src/solution.cpp`](../../dijkstra/cpp/src/solution.cpp)): you
want the vertex with the *smallest* known distance popped first, so the
queue is built as:

```cpp
std::priority_queue<std::pair<int,int>, std::vector<std::pair<int,int>>, std::greater<std::pair<int,int>>> pq;
```

## Ordering of `std::pair`

When `T` is `std::pair<A, B>`, both `std::less` and `std::greater` compare
**lexicographically**: `.first` is compared first, and `.second` is only used
as a tiebreaker when both `.first` values are equal. This is why queue
entries for graph algorithms are usually stored as `{priority, id}` (e.g.
`{distance, vertex}`) rather than `{id, priority}` — you want the comparison
driven by the priority value, not the id.

## Core operations

| Operation      | Effect                                  | Complexity |
|-----------------|------------------------------------------|------------|
| `push(val)`     | insert an element                        | O(log n)   |
| `emplace(args…)`| construct an element in place            | O(log n)   |
| `pop()`         | remove the top element (no return value) | O(log n)   |
| `top()`         | read (not remove) the top element        | O(1)       |
| `empty()`       | check if the queue has no elements       | O(1)       |
| `size()`        | number of elements                       | O(1)       |

Note `pop()` doesn't return the removed value — the usual pattern is to read
`top()` first, then call `pop()`:

```cpp
auto best = pq.top();
pq.pop();
```

## Custom comparator for non-primitive types

For types without a natural `<`/`>` ordering, pass a custom comparator
(function object, lambda type, or struct with `operator()`):

```cpp
auto cmp = [](const Task& a, const Task& b) {
  return a.priority > b.priority; // smaller priority value = higher priority
};
std::priority_queue<Task, std::vector<Task>, decltype(cmp)> pq(cmp);
```

## Why a heap instead of sorting every time

A heap only maintains the *partial* ordering needed to answer "what's on
top" in O(1) and to restore that invariant in O(log n) after a push or pop —
it never fully sorts the underlying container. This is why `priority_queue`
is the right structure for greedy algorithms like Dijkstra that repeatedly
need "give me the smallest/largest remaining item" without caring about the
full order of everything else.
