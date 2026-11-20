#include <iostream>
#include <vector>

#include "solution.hpp"

int main() {
    int n;
    std::cout << "Enter number of bars: ";
    std::cin >> n;

    vector<int> heights(n);
    std::cout << "Enter " << n << " heights: " << std::endl;
    for (int& h : heights) {
        std::cin >> h;
    }

    Solution* solution = new Solution();
    std::cout << solution->maxArea(heights) << std::endl;
    return 0;
}
