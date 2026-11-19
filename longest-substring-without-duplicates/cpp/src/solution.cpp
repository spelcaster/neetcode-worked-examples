#include "solution.hpp"
#include <map>

int Solution::lengthOfLongestSubstring(string s) {
  int left = 0;
  int right = 0;
  int maxLength = 0;

  if (s.empty()) {
    return maxLength;
  }

  std::map<char, int> window;

  return this->search(s, maxLength, left, right, window);
}

int Solution::search(string& s, int maxLength, int left, int right, std::map<char, int>& window) {
  if (!this->isValidPosition(right, s)) {
    return maxLength;
  }

  char c = s[right];
  auto it = window.find(c);

  if (it != window.end()) {
    left = it->second + 1;
    auto nit = std::next(it);

    window.erase(window.begin(), nit);
  }

  window.insert({c, right});

  int windowSize = static_cast<int>(window.size());

  if (windowSize > maxLength) {
    maxLength = window.size();
  }

  right++;

  return this->search(s, maxLength, left, right, window);
}

bool Solution::isValidPosition(int pos, string& s) {
  // short-circuit
  return pos >= 0 && static_cast<size_t>(pos) < s.size();
}
