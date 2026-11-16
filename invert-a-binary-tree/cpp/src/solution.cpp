#include "solution.hpp"
#include "tree_node.hpp"
#include <utility>

TreeNode* Solution::invertTree(TreeNode* root) {
  if (!root) {
    return root;
  }

  std::swap(root->left, root->right);

  this->invertTree(root->left);
  this->invertTree(root->right);

  return root;
}
