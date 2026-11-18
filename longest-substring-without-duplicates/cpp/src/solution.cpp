#include "solution.hpp"
#include <unordered_map>

int Solution::lengthOfLongestSubstring(string s) {
  int left = 0;
  int right = 0;

  std::unordered_map<char, bool> hashSet;

  return this->search(s, left, right, hashSet);
}

int Solution::search(string& s, int left, int right, std::unordered_map<char, bool>& hashSet) {
  if (!this->isValidPosition(right, s)) {
    return hashSet.size();
  }

  char c = s[right];
  auto it = hashSet.find(c);

  if (it != hashSet.end()) {
    left++;
    right++;
    return this->search(s, left, right, hashSet);
  }

  hashSet[c] = true;
  right++;

  return this->search(s, left, right, hashSet);
}

bool Solution::isValidPosition(int pos, string& s) {
  // short-circuit
  return pos >= 0 && static_cast<size_t>(pos) < s.size();
}
