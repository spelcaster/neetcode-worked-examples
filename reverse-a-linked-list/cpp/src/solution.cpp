#include "solution.hpp"

ListNode* Solution::reverseList(ListNode* head) {
  if (!head) {
    return head;
  }

  ListNode* curr = head;
  ListNode* prev = nullptr;

  while (curr) {
    ListNode* next = curr->next;
    curr->next = prev;

    prev = curr;

    curr = next;
  }

  return prev;
}
