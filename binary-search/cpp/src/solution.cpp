#include "solution.hpp"
#include <iterator>
#include <iostream>

int Solution::search(vector<int>& nums, int target) {
  if (nums.empty()) {
    return -1;
  }

  return this->binarySearch(nums, 0, nums.size() - 1, target);
}

int Solution::binarySearch(vector<int>& nums, int left, int right, int target) {
  if (left > right) {
    return -1;
  }

  int mid = left + ((right - left) / 2);

  if (target == nums[mid]) {
    return mid;
  } else if (nums[mid] < target) {
    left = mid + 1;
  } else if (nums[mid] > target) {
    right = mid - 1;
  }

  return this->binarySearch(nums, left, right, target);
}
