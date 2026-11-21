#pragma once

#include <unordered_map>
#include <vector>

class Solution {
public:
  std::unordered_map<int, int> shortestPath(int n, std::vector<std::vector<int>>& edges, int src);

  void calculate(std::vector<int>& distances, std::vector<std::vector<std::pair<int, int>>>& adjacencyList, int vertex, int weight);
};
