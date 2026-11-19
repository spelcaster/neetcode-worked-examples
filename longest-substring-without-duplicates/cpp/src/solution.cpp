#include "solution.hpp"
#include <map>
#include <vector>

int Solution::lengthOfLongestSubstring(string s) {
  int maxLength = 0;

  if (s.empty()) {
    return maxLength;
  }

  int left = 0;
  int right = 0;

  std::vector<int> lastPos(256, -1);

  while (this->isValidPosition(right, s)) {
    int c = s[right];

    if (lastPos[c] >= left) {
      left = lastPos[c] + 1;
    }

    lastPos[c] = right;

    int windowSize = right - left + 1;

    if (windowSize > maxLength) {
      maxLength = windowSize;
    }

    right++;
  }

  return maxLength;
}

bool Solution::isValidPosition(int pos, string& s) {
  // short-circuit
  return pos >= 0 && static_cast<size_t>(pos) < s.size();
}
