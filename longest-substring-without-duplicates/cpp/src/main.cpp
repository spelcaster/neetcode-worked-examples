#include <iostream>
#include <string>

#include "solution.hpp"

int main() {
    std::string s;
    std::cout << "Enter string: ";
    std::cin >> s;

    Solution* solution = new Solution();

    std::cout << solution->lengthOfLongestSubstring(s) << std::endl;
    return 0;
}
