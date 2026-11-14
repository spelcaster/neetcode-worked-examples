#include "solution.hpp"

#include <unordered_map>
#include <vector>

bool Solution::hasDuplicate(std::vector<int>& nums) {
  std::unordered_map<int, bool> mapped;

  for (unsigned i = 0; i < nums.size(); i++) {
    if (mapped.find(nums.at(i)) != mapped.end()) {
      return true;
    }

    mapped[nums.at(i)] = true;

  }

  return false;
}
