#include "solution.hpp"
#include <iostream>
#include <cmath>
#include <cctype>

bool Solution::isPalindrome(std::string s) {
    std::string::iterator it = s.begin();
    std::string::reverse_iterator rit = s.rbegin();
    int mSize = static_cast<int>(std::ceil(s.length() / 2));
    while (std::distance(s.begin(), it) < mSize) {
        this->adjustForwardPointer(s, it);

        if (it == s.end()) {
          return false;
        }

        this->adjustReversePointer(s, rit);

        int a = std::tolower(*it);
        int b = std::tolower(*rit);

        if (a != b) {
            return false;
        }

        ++it;
        ++rit;
    }

    return true;
}


void Solution::adjustForwardPointer(std::string s, std::string::iterator& it) {
    while (!isValid(*it) && it != s.end()) {
        ++it;
    }
}

void Solution::adjustReversePointer(std::string s, std::string::reverse_iterator& rit) {
    while (!isValid(*rit) && rit != s.rend()) {
        ++rit;
    }
}

bool Solution::isValid(int v) {
    return (v >= 48 && v <= 57)
        || (v >= 65 && v <= 90)
        || (v >= 97 && v <= 122);
}
