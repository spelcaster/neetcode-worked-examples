#include <cassert>
#include <iostream>
#include <vector>

#include "solution.hpp"

int main() {
    Solution* sol = new Solution();

    // notes/problem.md Example 1
    vector<int> heights1 = {1, 7, 2, 5, 4, 7, 3, 6};
    assert(sol->maxArea(heights1) == 36);

    // notes/problem.md Example 2
    vector<int> heights2 = {2, 2, 2};
    assert(sol->maxArea(heights2) == 4);

    // notes/worked-example.md edge case: minimum input, n = 2
    vector<int> heights3 = {1, 1};
    assert(sol->maxArea(heights3) == 1);

    // notes/worked-example.md edge case: zero-height bar
    vector<int> heights4 = {4, 0, 3};
    assert(sol->maxArea(heights4) == 6);

    // notes/worked-example.md edge case: strictly increasing array
    vector<int> heights5 = {1, 2, 3, 4, 5};
    assert(sol->maxArea(heights5) == 6);

    std::cout << "all tests passed" << std::endl;
    return 0;
}
