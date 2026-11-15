#include <iostream>
#include <string>

#include "solution.hpp"

int main() {
    std::string s;
    std::cout << "Enter a string: ";
    std::getline(std::cin, s);

    Solution* solution = new Solution();

    std::cout << (solution->isPalindrome(s) ? "palindrome" : "not a palindrome") << std::endl;
    return 0;
}
