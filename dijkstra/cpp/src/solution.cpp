#include "solution.hpp"
#include <unordered_map>
#include <utility>

std::unordered_map<int, int> Solution::shortestPath(int n, std::vector<std::vector<int>>& edges, int src) {
  std::vector<int> distances(n, -1);

  std::vector<std::vector<std::pair<int, int>>> adjacencyList(n);

  for (const auto& edge : edges) {
    int u = edge[0], v = edge[1], w = edge[2];
    adjacencyList[u].push_back({v, w});

  }

  this->calculate(distances, adjacencyList, src, 0);

  std::unordered_map<int, int> result;

  for (int i = 0; i < n; i++) {
    result[i] = distances[i];
  }

  return result;
}

void Solution::calculate(std::vector<int>& distances, std::vector<std::vector<std::pair<int, int>>>& adjacencyList, int vertex, int weight) {
  if ((distances[vertex] >= 0) && (distances[vertex] < weight)) {
    return;
  } else if ((distances[vertex] < 0) || (distances[vertex] > weight)) {
    distances[vertex] = weight;
  }

  if (adjacencyList[vertex].empty()) {
    return;
  }

  for (const auto& pair : adjacencyList[vertex]) {
    this->calculate(distances, adjacencyList, pair.first, weight + pair.second);
  }
}
