#include <iostream>
#include <string>

#include "solution.hpp"

int main() {
    std::string s;
    std::cout << "Enter a string of brackets: ";
    std::cin >> s;

    Solution* solution = new Solution();

    std::cout << (solution->isValid(s) ? "valid" : "invalid") << std::endl;
    return 0;
}
