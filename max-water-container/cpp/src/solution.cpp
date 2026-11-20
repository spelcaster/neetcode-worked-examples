#include "solution.hpp"
#include <algorithm>
#include <iterator>

int Solution::maxArea(vector<int>& heights) {
  int maxH = 0;

  if (heights.empty()) {
    return maxH;
  }

  std::vector<int>::iterator leftIt = heights.begin();
  std::vector<int>::reverse_iterator rightIt = heights.rbegin();

  while (leftIt < rightIt.base() - 1) {
    int left = static_cast<int>(std::distance(heights.begin(), leftIt));
    int right = static_cast<int>(std::distance(rightIt, heights.rend())) - 1;

    int hLeft = *leftIt;
    int hRight = *rightIt;

    int h = (right - left) * std::min(hLeft, hRight);

    if (h > maxH) {
      maxH = h;
    }

    if (hRight < hLeft) {
      rightIt++;
      continue;
    }

    leftIt++;
  }

  return maxH;
}
