#include <cassert>
#include <iostream>

#include "solution.hpp"
#include "tree_node.hpp"

namespace {

TreeNode* leaf(int val) {
    return new TreeNode(val);
}

}  // namespace

int main() {
    Solution solution;

    // Example 1: root = [1,2,3,4,5,6,7] -> [1,3,2,7,6,5,4]
    {
        TreeNode* n4 = leaf(4);
        TreeNode* n5 = leaf(5);
        TreeNode* n6 = leaf(6);
        TreeNode* n7 = leaf(7);
        TreeNode* n2 = new TreeNode(2, n4, n5);
        TreeNode* n3 = new TreeNode(3, n6, n7);
        TreeNode* root = new TreeNode(1, n2, n3);

        TreeNode* result = solution.invertTree(root);

        assert(result == root);
        assert(result->val == 1);
        assert(result->left == n3);
        assert(result->right == n2);
        assert(n3->left == n7);
        assert(n3->right == n6);
        assert(n2->left == n5);
        assert(n2->right == n4);
    }

    // Example 2: root = [3,2,1] -> [3,1,2]
    {
        TreeNode* n2 = leaf(2);
        TreeNode* n1 = leaf(1);
        TreeNode* root = new TreeNode(3, n2, n1);

        TreeNode* result = solution.invertTree(root);

        assert(result == root);
        assert(result->left == n1);
        assert(result->right == n2);
    }

    // Example 3: root = [] -> []
    {
        TreeNode* result = solution.invertTree(nullptr);
        assert(result == nullptr);
    }

    // Edge case: single node -> unchanged
    {
        TreeNode* root = leaf(1);
        TreeNode* result = solution.invertTree(root);
        assert(result == root);
        assert(result->left == nullptr);
        assert(result->right == nullptr);
    }

    // Edge case: skewed tree (left-only chain) -> becomes right-only chain
    {
        TreeNode* n3 = leaf(3);
        TreeNode* n2 = new TreeNode(2, n3, nullptr);
        TreeNode* root = new TreeNode(1, n2, nullptr);

        TreeNode* result = solution.invertTree(root);

        assert(result == root);
        assert(result->left == nullptr);
        assert(result->right == n2);
        assert(n2->left == nullptr);
        assert(n2->right == n3);
    }

    std::cout << "all tests passed" << std::endl;
    return 0;
}
