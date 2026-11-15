#pragma once

#include <string>

class Solution {
public:
    bool isPalindrome(std::string s);

    void adjustForwardPointer(std::string& s, std::string::iterator& it);

    void adjustReversePointer(std::string& s, std::string::reverse_iterator& rit);

    bool isValid(int v);
};
