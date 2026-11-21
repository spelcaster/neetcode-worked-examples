#pragma once

#include <unordered_map>
#include <vector>

class Solution {
public:
  std::unordered_map<int, int> shortestPath(int n, std::vector<std::vector<int>>& edges, int src);
};
