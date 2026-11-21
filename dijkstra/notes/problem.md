# Dijkstra's Algorithm

https://neetcode.io/problems/dijkstra/question

## Description

Implement Dijkstra's shortest path algorithm.

Given a weighted, directed graph, and a starting vertex, return the shortest
distance from the starting vertex to every vertex in the graph.

**Input:**
- `n` - the number of vertices in the graph, where `(2 <= n <= 100)`. Each
  vertex is labeled from `0` to `n - 1`.
- `edges` - a list of tuples, each representing a directed edge in the form
  `(u, v, w)`, where `u` is the source vertex, `v` is the destination vertex,
  and `w` is the weight of the edge, where `(1 <= w <= 10)`.
- `src` - the source vertex from which to start the algorithm, where
  `(0 <= src < n)`.

**Note:** If a vertex is unreachable from the source vertex, the shortest
path distance for the unreachable vertex should be `-1`.

**Function signature (C++):**
```cpp
unordered_map<int, int> shortestPath(int n, vector<vector<int>>& edges, int src);
```

## Examples

**Example 1:**

```
Input:
n = 5
edges = [[0,1,10], [0,2,3], [1,3,2], [2,1,4], [2,3,8], [2,4,2], [3,4,5]]
src = 0

Output: {{0:0}, {1:7}, {2:3}, {3:9}, {4:5}}
```

The graph (directed, weighted):
- 0 -> 1 (weight 10)
- 0 -> 2 (weight 3)
- 1 -> 3 (weight 2)
- 2 -> 1 (weight 4)
- 2 -> 3 (weight 8)
- 2 -> 4 (weight 2)
- 3 -> 4 (weight 5)

Shortest distances from vertex 0:
- to 0: 0 (the source itself)
- to 1: 7 (via 0 -> 2 -> 1, cost 3 + 4 = 7, cheaper than direct 0 -> 1 at cost 10)
- to 2: 3 (direct 0 -> 2)
- to 3: 9 (via 0 -> 2 -> 1 -> 3, cost 3 + 4 + 2 = 9, cheaper than 0 -> 2 -> 3 at cost 3 + 8 = 11)
- to 4: 5 (via 0 -> 2 -> 4, cost 3 + 2 = 5)

## Constraints

- `2 <= n <= 100`
- `1 <= w <= 10` for every edge weight
- `0 <= src < n`
- The graph is directed (edge `(u, v, w)` does not imply an edge back from `v` to `u`)
- A vertex unreachable from `src` must be reported with distance `-1`
