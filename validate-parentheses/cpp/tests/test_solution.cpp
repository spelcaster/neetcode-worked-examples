#include <cassert>
#include <iostream>
#include <string>

#include "solution.hpp"

int main() {
    Solution* sol = new Solution();
    assert(sol->isValid("()") == true);
    assert(sol->isValid("()[]{}") == true);
    assert(sol->isValid("{[()]}") == true);
    assert(sol->isValid("") == true);

    assert(sol->isValid("(]") == false);
    assert(sol->isValid("([)]") == false);
    assert(sol->isValid("(") == false);
    assert(sol->isValid("((") == false);
    assert(sol->isValid(")") == false);

    std::cout << "all tests passed" << std::endl;
    return 0;
}
