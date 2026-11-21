# Dijkstra's Algorithm — Worked Example

Problem: [`notes/problem.md`](./problem.md)

## 1. Restatement

Given a directed, positively-weighted graph with `n` vertices and a list of
weighted edges `(u, v, w)`, determine — starting from `src` — the shortest
total weight to reach every vertex, or `-1` for any vertex that can't be
reached at all.

## 2. Pick the approach

The access pattern here is "always extend the frontier from whichever known
distance is currently smallest" — at each step, the unfinished vertex with
the smallest tentative distance is guaranteed to already have its *true*
shortest distance, precisely because every edge weight is non-negative (no
edge can ever make a path shorter by being traversed, so nothing still
sitting in the frontier can beat it later). That "repeatedly pull the
current minimum, then relax its neighbors" pattern is **Dijkstra's
algorithm**, and "repeatedly pull the current minimum" is exactly what a
**min-priority queue (min-heap)** is for.

## 3. Trace

### Successful case: `n = 5`, `src = 0` (the example from `notes/problem.md`)

Adjacency list built from `edges`:
- `0 -> 1 (10)`, `0 -> 2 (3)`
- `1 -> 3 (2)`
- `2 -> 1 (4)`, `2 -> 3 (8)`, `2 -> 4 (2)`
- `3 -> 4 (5)`

`dist` starts as `[0, inf, inf, inf, inf]`, heap starts as `[(0,0)]`.

| pop (dist, vertex) | already finalized? | relax neighbors | heap after |
|---|---|---|---|
| (0, 0) | no → finalize `dist[0]=0` | `1`: `0+10=10 < inf` → `dist[1]=10`; `2`: `0+3=3 < inf` → `dist[2]=3` | `[(3,2),(10,1)]` |
| (3, 2) | no → finalize `dist[2]=3` | `1`: `3+4=7 < 10` → `dist[1]=7`; `3`: `3+8=11 < inf` → `dist[3]=11`; `4`: `3+2=5 < inf` → `dist[4]=5` | `[(5,4),(7,1),(10,1),(11,3)]` |
| (5, 4) | no → finalize `dist[4]=5` | `4` has no outgoing edges | `[(7,1),(10,1),(11,3)]` |
| (7, 1) | no → finalize `dist[1]=7` | `3`: `7+2=9 < 11` → `dist[3]=9` | `[(9,3),(10,1),(11,3)]` |
| (9, 3) | no → finalize `dist[3]=9` | `4`: `9+5=14`, not `< dist[4]=5` → no update | `[(10,1),(11,3)]` |
| (10, 1) | **yes**, `dist[1]` is already `7` → skip (stale heap entry) | — | `[(11,3)]` |
| (11, 3) | **yes**, `dist[3]` is already `9` → skip (stale heap entry) | — | `[]` |

Heap empty → done. `dist = [0, 7, 3, 9, 5]`, none left as `inf`, so the
result is `{0:0, 1:7, 2:3, 3:9, 4:5}` — matches the expected output in
`notes/problem.md`.

Note the two "stale entry" rows: a vertex can be pushed onto the heap more
than once (once per relaxation that improves its distance), so popping
checks whether it's already finalized and discards the entry if so, rather
than relaxing from a distance that's no longer the best known one.

### Edge case: unreachable vertices, `n = 5`, same edges, `src = 4`

Vertex `4` has no outgoing edges in this graph, so nothing is ever reachable
from it except itself.

| pop (dist, vertex) | already finalized? | relax neighbors | heap after |
|---|---|---|---|
| (0, 4) | no → finalize `dist[4]=0` | `4` has no outgoing edges | `[]` |

Heap empties immediately. `dist = [inf, inf, inf, inf, 0]` — every vertex
except the source is still `inf`, meaning it was never reached. Those get
reported as `-1`: result is `{0:-1, 1:-1, 2:-1, 3:-1, 4:0}`.

## 4. Pseudocode

```
function shortestPath(n, edges, src):
    adj = array of n empty lists
    for (u, v, w) in edges:
        adj[u].append((v, w))

    dist = array of n values, all infinity
    dist[src] = 0

    heap = min-priority-queue ordered by distance
    heap.push((0, src))
    finalized = set()

    while heap is not empty:
        (d, u) = heap.pop_min()
        if u in finalized:
            continue          // stale entry — already found a better/equal distance
        finalized.add(u)

        for (v, w) in adj[u]:
            if d + w < dist[v]:
                dist[v] = d + w
                heap.push((dist[v], v))

    result = {}
    for i in 0..n-1:
        result[i] = dist[i] if dist[i] != infinity else -1
    return result
```

## 5. Complexity: brute force vs. optimal

**Brute force (enumerate every path):** for each vertex, try every possible
route from `src` to it (DFS branching at every outgoing edge) and keep the
minimum total weight found. This is exponential in the worst case — the
number of simple paths through a graph grows combinatorially with the
number of edges — because the same sub-path to an intermediate vertex gets
walked and re-summed from scratch inside every longer path that happens to
pass through it, even though that sub-path's shortest distance never
changes once it's known.

**Optimal (Dijkstra with a min-heap):** every vertex is popped from the
heap and finalized exactly once (`O(log n)` per pop), and every edge causes
at most one push (`O(log n)` per push), for `O((n + E) log n)` time and
`O(n + E)` space (the distance array, the heap, and the adjacency list).
Each vertex's true shortest distance is computed once, the moment it's
popped as the current minimum, and is never recomputed afterward — the
"stale entry" rows in the trace above are exactly that: a vertex found
again via a worse route, immediately discarded instead of re-relaxed.

Gap: exponential path enumeration → `O((n + E) log n)` by exploiting the
non-negative-weight greedy property (the current minimum is always already
correct) instead of re-deriving the same shortest sub-paths over and over.

## 6. Edge cases

- **Unreachable vertices** — traced above with `src = 4`: any vertex whose
  `dist` entry is still `infinity` once the heap empties never had a path
  relax it, and must be reported as `-1` rather than left as "infinity" or
  omitted.
- **The source vertex itself** — `dist[src]` is seeded to `0` before the
  loop starts, so it's finalized on the very first pop in both traces above
  without needing an incoming edge.
- **A vertex reachable by multiple routes of different cost** — vertex `1`
  in the first trace is reachable directly (`0->1`, cost `10`) and via `2`
  (`0->2->1`, cost `7`); the relax check `d + w < dist[v]` is what lets the
  cheaper route win regardless of which one the algorithm happens to
  explore first, and is also exactly why the later `(10, 1)` pop is stale.
- **Non-negative weights only (`1 <= w <= 10`)** — this is what makes
  "the current minimum on the heap is already final" true in the first
  place; Dijkstra's greedy correctness would break if an edge could have
  negative weight, since a not-yet-visited vertex could then still produce
  a shorter path later.
