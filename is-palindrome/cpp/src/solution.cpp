#include "solution.hpp"
#include <iostream>
#include <cmath>
#include <cctype>

bool Solution::isPalindrome(std::string s) {
    std::string::iterator it = s.begin();
    std::string::reverse_iterator rit = s.rbegin();

    while (rit != s.rend()) {
        this->adjustReversePointer(s, rit);

        if (rit == s.rend()) {
          return true;
        }

        this->adjustForwardPointer(s, it);

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


void Solution::adjustForwardPointer(std::string& s, std::string::iterator& it) {
    while (it != s.end()) {
        int v = *it;

        if (isValid(v)) {
            return;
        }

        ++it;
    }
}

void Solution::adjustReversePointer(std::string& s, std::string::reverse_iterator& rit) {
    while (rit != s.rend()) {
        int v = *rit;

        if (isValid(v)) {
            return;
        }

        ++rit;
    }
}

bool Solution::isValid(int v) {
    return (v >= 48 && v <= 57)
        || (v >= 65 && v <= 90)
        || (v >= 97 && v <= 122);
}
