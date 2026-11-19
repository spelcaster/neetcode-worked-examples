#pragma once

#include <string>
#include <map>

using std::string;

class Solution {
public:
    int lengthOfLongestSubstring(string s);
    int search(string& s, int length, int left, int right, std::map<char, int>& window);
    bool isValidPosition(int pos, string& s);
};
