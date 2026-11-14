#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "solution.hpp"

int main() {
    std::cout << "Enter nums as space-separated integers: ";
    std::string line;
    std::getline(std::cin, line);

    std::istringstream iss(line);
    std::vector<int> nums;
    int value;
    while (iss >> value) {
        nums.push_back(value);
    }

    Solution* solution = new Solution();

    std::cout << (solution->hasDuplicate(nums) ? "true" : "false") << std::endl;
    return 0;
}
