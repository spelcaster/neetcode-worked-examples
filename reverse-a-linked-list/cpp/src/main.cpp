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

void printList(ListNode* head) {
    while (head) {
        std::cout << head->val;
        if (head->next) {
            std::cout << " -> ";
        }
        head = head->next;
    }
    std::cout << std::endl;
}

int main() {
    std::cout << "Enter space-separated integers (Ctrl+D to end): ";
    std::vector<int> values;
    int value;
    while (std::cin >> value) {
        values.push_back(value);
    }

    ListNode* head = buildList(values);

    Solution* solution = new Solution();
    ListNode* reversed = solution->reverseList(head);

    printList(reversed);
    return 0;
}
