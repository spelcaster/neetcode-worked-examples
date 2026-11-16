# Invert Binary Tree — worked example

See [notes/problem.md](./problem.md) for the full problem statement.

## 1. Problem restatement

Given: the root of a binary tree.
Determine: a tree where every node's left and right children have been
swapped, returned via the same root.

## 2. Pick the approach

A binary tree is already a recursive structure: "invert this tree" is
defined in terms of "invert the left subtree" and "invert the right
subtree." That recursive shape -> recursive DFS that, at each node, swaps
the two children and then recurses into each (now-swapped) child. The
base case is the empty subtree (`nullptr`), which has nothing to swap.

An equivalent iterative version (stack or queue holding nodes still to
visit) works too — the key insight is the same either way: every node
needs exactly one swap of its two child pointers, independent of the
others, so a single full traversal suffices.

## 3. Walk through a concrete example

Tree from Example 1 (`root = [1,2,3,4,5,6,7]`):

```
        1
       / \
      2   3
     / \ / \
    4  5 6  7
```

Using `invert(node)`: swap `node.left`/`node.right`, then recurse into
each child, in DFS call order:

| call | node's children before swap | action | children after swap |
|---|---|---|---|
| `invert(1)` | left=2, right=3 | swap | left=3, right=2 |
| `invert(3)` (now root's left) | left=6, right=7 | swap | left=7, right=6 |
| `invert(7)` | none (leaf) | no-op | — |
| `invert(6)` | none (leaf) | no-op | — |
| `invert(2)` (now root's right) | left=4, right=5 | swap | left=5, right=4 |
| `invert(5)` | none (leaf) | no-op | — |
| `invert(4)` | none (leaf) | no-op | — |

Resulting tree:

```
        1
       / \
      3   2
     / \ / \
    7  6 5  4
```

Read off level-order: `[1,3,2,7,6,5,4]` — matches Example 1's expected
output.

**Edge case traced the same way — empty tree (Example 3):**

| call | node | action |
|---|---|---|
| `invert(nullptr)` | node is `nullptr` | base case hit immediately, return `nullptr`, no swap happens |

Output: `[]`, matching Example 3. The recursion never gets a chance to
touch anything, which is exactly correct — there's nothing to invert.

## 4. Generalize into pseudocode

```
function invertTree(node):
    if node == null:
        return null
    swap(node.left, node.right)
    invertTree(node.left)
    invertTree(node.right)
    return node
```

(The two recursive calls can happen in either order, or even be replaced
by an iterative BFS/DFS using an explicit queue/stack — the invariant is
just "every node gets its children swapped exactly once.")

## 5. Complexity, worst vs. desired solution

**Naive — rebuild a brand-new tree:** allocate a fresh `TreeNode` for
every node visited, copying over `val` and attaching freshly-inverted
copies of the children, instead of reusing the existing nodes.
- Time: O(n) — still visits every node once.
- Space: O(n) for the n newly-allocated nodes, plus O(h) for the
  recursion stack (h = tree height).
- Why it's worse: it does the same traversal work as the in-place
  version but adds a full tree's worth of redundant allocation and
  copying that accomplishes nothing extra — the original nodes already
  have everything needed, they just need their two pointers swapped.

**Optimal — invert in place:** reuse every existing node, just swap its
`left`/`right` pointers via DFS (or BFS).
- Time: O(n) — each node visited once, O(1) work per node.
- Space: O(h) — only the recursion call stack (or, for an iterative BFS
  version, O(w) for a queue holding at most one level's worth of nodes,
  w = max width). No new nodes allocated.
- Why it's better: same O(n) traversal, but O(1) extra work per node
  instead of an O(1) allocation + copy per node, and no new memory for
  nodes that already exist.

Gap: both are O(n) time (you can't do better than visiting every node),
but naive carries O(n) extra space for copies that optimal avoids
entirely by mutating in place — O(n) auxiliary space -> O(h) auxiliary
space.

## 6. Edge cases

- **Empty tree** (`root = []`, Example 3) — traced above: base case
  returns `nullptr` immediately, no swap attempted. Output `[]`.
- **Single node** (e.g. `root = [1]`) — `invert(1)`: both children are
  already `nullptr`, so `swap(nullptr, nullptr)` is a no-op, and both
  recursive calls immediately hit the base case. Output `[1]`, unchanged
  — correct, since a single node is its own mirror image.
- **Already-symmetric subtree** (e.g. a node whose two children are
  themselves leaves with no further structure, as in `6` and `7` in the
  trace above) — still gets swapped like any other node; "looks the
  same" structurally doesn't exempt a node from the swap, only matters
  when the *values* happen to be equal, which doesn't affect correctness
  here since the problem inverts structure, not value equality.
- **Skewed tree** (every node has only a left child, or only a right
  child) — each `invert` call swaps a present child with a `nullptr`
  child, same O(1) work per node; recursion depth equals the full node
  count n in this case, so the O(h) stack space becomes O(n) — worth
  knowing as the worst case for the "optimal" solution's space bound.
