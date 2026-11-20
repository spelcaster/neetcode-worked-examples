# Reverse a Linked List

https://neetcode.io/problems/reverse-a-linked-list

> Note: the neetcode.io page could not be rendered (no working browser
> tool was available in this session), so this statement is reconstructed
> from knowledge of the well-known equivalent problem, LeetCode 206
> "Reverse Linked List". Verify against the live page if precision matters.

## Description

Given the `head` of a singly linked list, reverse the list, and return
the new head of the reversed list.

Each node in the list has an integer value and a pointer to the next
node (`nullptr`/`None` for the last node).

## Examples

**Example 1:**
```
Input:  head = [1, 2, 3, 4, 5]
Output: [5, 4, 3, 2, 1]
```

**Example 2:**
```
Input:  head = [1, 2]
Output: [2, 1]
```

**Example 3:**
```
Input:  head = []
Output: []
```

## Constraints

- The number of nodes in the list is in the range `[0, 5000]`.
- `-1000 <= Node.val <= 1000`
