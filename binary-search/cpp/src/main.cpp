#include <iostream>
#include <vector>

#include "solution.hpp"

int main() {
    int n;
    std::cout << "Enter array length: ";
    std::cin >> n;

    std::vector<int> nums(n);
    std::cout << "Enter " << n << " sorted integers: ";
    for (int& num : nums) {
        std::cin >> num;
    }

    int target;
    std::cout << "Enter target: ";
    std::cin >> target;

    Solution* solution = new Solution();

    std::cout << solution->search(nums, target) << std::endl;
    return 0;
}
