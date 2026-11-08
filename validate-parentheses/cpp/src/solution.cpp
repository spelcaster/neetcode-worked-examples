#include "solution.hpp"
#include <stack>

bool Solution::isValid(std::string s) {
    if (s.empty()) {
        return true;
    }

    int length = s.length();

    if (length % 2 > 0) {
        return false;
    }

    char prev;
    std::stack<char> stack;

    auto matches = [](char open, char close) {
        return (open == '(' && close == ')') ||
            (open == '[' && close == ']') ||
            (open == '{' && close == '}');
    };

    auto isClosing = [](char &character) {
        return character == ')' ||
            character == ']' ||
            character == '}';
    };

    for (char character : s) {
        if (!prev) {
            prev = character;
            stack.push(character);
            continue;
        }

        if (matches(prev, character)) {
            stack.pop();
            prev = 0;

            if (!stack.empty()) {
                prev = stack.top();
            }

            continue;
        }

        if (isClosing(character)) {
            return false;
        }

        prev = character;
        stack.push(character);
    }

    return stack.empty();
}
