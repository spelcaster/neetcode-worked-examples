#pragma once

#include <string>
#include <unordered_map>

using std::string;

class Solution {
public:
    int lengthOfLongestSubstring(string s);
    int search(string& s, int left, int right, std::unordered_map<char, bool>& hashSet);
    bool isValidPosition(int pos, string& s);
};
