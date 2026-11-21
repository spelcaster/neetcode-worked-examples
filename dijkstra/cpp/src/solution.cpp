#include "solution.hpp"
#include <functional>
#include <queue>
#include <unordered_map>
#include <utility>

std::unordered_map<int, int> Solution::shortestPath(int n, std::vector<std::vector<int>>& edges, int src) {
  using Item = std::pair<int, int>;

  std::vector<int> distances(n, -1);
  std::vector<bool> visited(n, false);

  std::vector<std::vector<Item>> adjacencyList(n);

  for (const auto& edge : edges) {
    int u = edge[0], v = edge[1], w = edge[2];
    adjacencyList[u].push_back({v, w});

  }

  std::priority_queue<Item, std::vector<Item>, std::greater<Item>> pq;

  distances[src] = 0;
  pq.push({0, src});

  while (!pq.empty()) {
    auto [d, u] = pq.top();
    pq.pop();

    if (visited[u]) {
      continue;
    }

    visited[u] = true;

    for (auto& [v, w] : adjacencyList[u]) {
      if ((distances[v] == -1) || (d + w < distances[v])) {
        distances[v] = d + w;
        pq.push({distances[v], v});

      }
    }
  }

  std::unordered_map<int, int> result;

  for (int i = 0; i < n; i++) {
    result[i] = distances[i];
  }

  return result;
}
