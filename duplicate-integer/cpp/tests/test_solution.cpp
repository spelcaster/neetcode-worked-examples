#include <cassert>
#include <iostream>
#include <vector>

#include "solution.hpp"

int main() {
    Solution* sol = new Solution();

    std::vector<int> example1 = {1, 2, 3, 3};
    assert(sol->hasDuplicate(example1) == true);

    std::vector<int> example2 = {1, 2, 3, 4};
    assert(sol->hasDuplicate(example2) == false);

    std::vector<int> empty = {};
    assert(sol->hasDuplicate(empty) == false);

    std::vector<int> single = {5};
    assert(sol->hasDuplicate(single) == false);

    std::vector<int> allSame = {7, 7, 7};
    assert(sol->hasDuplicate(allSame) == true);

    std::vector<int> boundary = {-1000000000, 1000000000, -1000000000};
    assert(sol->hasDuplicate(boundary) == true);

    std::cout << "all tests passed" << std::endl;
    return 0;
}
