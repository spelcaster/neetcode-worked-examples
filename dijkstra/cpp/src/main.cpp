#include <iostream>
#include <vector>

#include "solution.hpp"

int main() {
    int n;
    std::cout << "Enter number of vertices: ";
    std::cin >> n;

    int edgeCount;
    std::cout << "Enter number of edges: ";
    std::cin >> edgeCount;

    std::vector<std::vector<int>> edges(edgeCount, std::vector<int>(3));
    std::cout << "Enter " << edgeCount << " edges as 'u v w': " << std::endl;
    for (std::vector<int>& edge : edges) {
        std::cin >> edge[0] >> edge[1] >> edge[2];
    }

    int src;
    std::cout << "Enter source vertex: ";
    std::cin >> src;

    Solution* solution = new Solution();
    std::unordered_map<int, int> distances = solution->shortestPath(n, edges, src);

    for (int vertex = 0; vertex < n; vertex++) {
        std::cout << vertex << ": " << distances[vertex] << std::endl;
    }
    return 0;
}
