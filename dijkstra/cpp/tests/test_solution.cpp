#include <cassert>
#include <iostream>
#include <unordered_map>
#include <vector>

#include "solution.hpp"

int main() {
    Solution* sol = new Solution();

    // notes/problem.md Example 1
    std::vector<std::vector<int>> edges = {
        {0, 1, 10}, {0, 2, 3}, {1, 3, 2}, {2, 1, 4}, {2, 3, 8}, {2, 4, 2}, {3, 4, 5}
    };

    std::unordered_map<int, int> expected1 = {{0, 0}, {1, 7}, {2, 3}, {3, 9}, {4, 5}};
    assert(sol->shortestPath(5, edges, 0) == expected1);

    // notes/worked-example.md edge case: src has no outgoing edges, so every
    // other vertex is unreachable and should be reported as -1.
    std::unordered_map<int, int> expected2 = {{0, -1}, {1, -1}, {2, -1}, {3, -1}, {4, 0}};
    assert(sol->shortestPath(5, edges, 4) == expected2);

    std::vector<std::vector<int>> edges2 = {
      {0,1,3}, {1,2,1}, {2,0,4}
    };

    std::unordered_map<int, int> expected3 = {{0, 0}, {1, 3}, {2, 4}};
    assert(sol->shortestPath(3, edges2, 0) == expected3);

    std::cout << "all tests passed" << std::endl;
    return 0;
}
