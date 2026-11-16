# Invert Binary Tree

https://neetcode.io/problems/invert-a-binary-tree

## Description

You are given the `root` of a binary tree. Invert the binary tree and
return its root.

Inverting a binary tree means swapping the left and right children of
every node in the tree.

Starter signature (C++):

```cpp
/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode() : val(0), left(nullptr), right(nullptr) {}
 *     TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
 *     TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
 * };
 */
class Solution {
public:
    TreeNode* invertTree(TreeNode* root) {

    }
};
```

## Examples

**Example 1:**
```
Input: root = [1,2,3,4,5,6,7]
Output: [1,3,2,7,6,5,4]
```

**Example 2:**
```
Input: root = [3,2,1]
Output: [3,1,2]
```

**Example 3:**
```
Input: root = []
Output: []
```

(Trees are given level-order/BFS, left-to-right, with `null` omitted gaps
written out where present — this is the standard LeetCode tree-array
notation.)

## Constraints

- `0 <= the number of nodes in the tree <= 100`
- `-100 <= Node.val <= 100`
