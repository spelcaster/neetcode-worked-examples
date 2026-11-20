#include <cassert>
#include <iostream>
#include <vector>

#include "solution.hpp"

ListNode* buildList(const std::vector<int>& values) {
    ListNode dummy;
    ListNode* tail = &dummy;
    for (int value : values) {
        tail->next = new ListNode(value);
        tail = tail->next;
    }
    return dummy.next;
}

std::vector<int> toVector(ListNode* head) {
    std::vector<int> values;
    while (head) {
        values.push_back(head->val);
        head = head->next;
    }
    return values;
}

int main() {
    Solution* sol = new Solution();

    assert(toVector(sol->reverseList(buildList({1, 2, 3, 4, 5}))) ==
           std::vector<int>({5, 4, 3, 2, 1}));
    assert(toVector(sol->reverseList(buildList({1, 2}))) ==
           std::vector<int>({2, 1}));
    assert(toVector(sol->reverseList(buildList({}))) ==
           std::vector<int>({}));
    assert(toVector(sol->reverseList(buildList({1}))) ==
           std::vector<int>({1}));

    std::cout << "all tests passed" << std::endl;
    return 0;
}
