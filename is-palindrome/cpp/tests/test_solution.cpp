#include <cassert>
#include <iostream>
#include <string>

#include "solution.hpp"

int main() {
    Solution* sol = new Solution();
    assert(sol->isPalindrome("Was it a car or a cat I saw?") == true);
    assert(sol->isPalindrome("tab a cat") == false);

    //assert(sol->isPalindrome("..,?") == true);
    assert(sol->isPalindrome("a") == true);
    assert(sol->isPalindrome("racecar") == true);
    assert(sol->isPalindrome("ab") == false);

    std::cout << "all tests passed" << std::endl;
    return 0;
}
