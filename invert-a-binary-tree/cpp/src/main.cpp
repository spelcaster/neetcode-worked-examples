#include <iostream>
#include <queue>
#include <sstream>
#include <string>
#include <vector>

#include "solution.hpp"
#include "tree_node.hpp"

namespace {

std::vector<std::string> splitValues(const std::string& line) {
    std::vector<std::string> values;
    std::stringstream ss(line);
    std::string token;
    while (std::getline(ss, token, ',')) {
        values.push_back(token);
    }
    return values;
}

TreeNode* buildTree(const std::vector<std::string>& values) {
    if (values.empty() || values[0] == "null") {
        return nullptr;
    }

    TreeNode* root = new TreeNode(std::stoi(values[0]));
    std::queue<TreeNode*> queue;
    queue.push(root);

    size_t i = 1;
    while (!queue.empty() && i < values.size()) {
        TreeNode* node = queue.front();
        queue.pop();

        if (i < values.size() && values[i] != "null") {
            node->left = new TreeNode(std::stoi(values[i]));
            queue.push(node->left);
        }
        ++i;

        if (i < values.size() && values[i] != "null") {
            node->right = new TreeNode(std::stoi(values[i]));
            queue.push(node->right);
        }
        ++i;
    }

    return root;
}

void printTree(TreeNode* root) {
    if (!root) {
        std::cout << "[]" << std::endl;
        return;
    }

    std::vector<std::string> values;
    std::queue<TreeNode*> queue;
    queue.push(root);

    while (!queue.empty()) {
        TreeNode* node = queue.front();
        queue.pop();

        if (!node) {
            values.push_back("null");
            continue;
        }

        values.push_back(std::to_string(node->val));
        queue.push(node->left);
        queue.push(node->right);
    }

    while (!values.empty() && values.back() == "null") {
        values.pop_back();
    }

    std::cout << "[";
    for (size_t i = 0; i < values.size(); ++i) {
        std::cout << values[i];
        if (i + 1 < values.size()) {
            std::cout << ",";
        }
    }
    std::cout << "]" << std::endl;
}

}  // namespace

int main() {
    std::cout << "Enter root as level-order values (e.g. 1,2,3,null,4): ";
    std::string line;
    std::getline(std::cin, line);

    TreeNode* root = buildTree(splitValues(line));

    Solution solution;
    TreeNode* inverted = solution.invertTree(root);

    printTree(inverted);
    return 0;
}
