#include "solution.hpp"
#include "tree_node.hpp"

TreeNode* Solution::invertTree(TreeNode* root) {
  if (root == nullptr) {
    return root;
  }

  if (root->left) {
    this->invertTree(root->left);
  }

  if (root->right) {
    this->invertTree(root->right);
  }

  TreeNode* tmpTree = root->left;

  root->left = root->right;
  root->right = tmpTree;

  return root;
}
