#include <cassert>
#include <iostream>
#include <vector>

#include "solution.hpp"

int main() {
    Solution* sol = new Solution();

    std::vector<int> nums1 = {-1, 0, 2, 4, 6, 8};
    assert(sol->search(nums1, 4) == 3);

    std::vector<int> nums2 = {-1, 0, 2, 4, 6, 8};
    assert(sol->search(nums2, 3) == -1);

    std::vector<int> nums3 = {5};
    assert(sol->search(nums3, 5) == 0);

    std::vector<int> nums4 = {5};
    assert(sol->search(nums4, 2) == -1);

    std::vector<int> nums5 = {-1, 0, 2, 4, 6, 8};
    assert(sol->search(nums5, -5) == -1);

    std::vector<int> nums6 = {-1, 0, 2, 4, 6, 8};
    assert(sol->search(nums6, 9) == -1);

    std::cout << "all tests passed" << std::endl;
    return 0;
}
