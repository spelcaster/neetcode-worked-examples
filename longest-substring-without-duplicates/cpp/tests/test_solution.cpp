#include <cassert>
#include <iostream>
#include <string>

#include "solution.hpp"

int main() {
    Solution* sol = new Solution();

    assert(sol->lengthOfLongestSubstring("zxyzxyz") == 3);
    assert(sol->lengthOfLongestSubstring("xxxx") == 1);
    assert(sol->lengthOfLongestSubstring("") == 0);
    assert(sol->lengthOfLongestSubstring("a") == 1);
    assert(sol->lengthOfLongestSubstring("xyz") == 3);

    std::cout << "all tests passed" << std::endl;
    return 0;
}
