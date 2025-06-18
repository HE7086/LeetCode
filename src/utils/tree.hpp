#pragma once
#include <algorithm>
#include <format>
#include <queue>
#include <set>
#include <string>

#define SafeTree TreeNode __attribute__((__cleanup__(clean_up)))

struct TreeNode {
  int       val;
  TreeNode* left;
  TreeNode* right;
  TreeNode()
      : val(0), left(nullptr), right(nullptr) {}
  TreeNode(int x)
      : val(x), left(nullptr), right(nullptr) {}
  TreeNode(int x, TreeNode* left, TreeNode* right)
      : val(x), left(left), right(right) {}

  bool operator==(TreeNode const& other) const {
    if (val != other.val) {
      return false;
    }
    if ((left == nullptr && other.left != nullptr) ||
        (left != nullptr && other.left == nullptr) ||
        (right == nullptr && other.right != nullptr) ||
        (right != nullptr && other.right == nullptr)) {
      return false;
    }
    if (left == nullptr && right == nullptr) {
      return true;
    }
    return (*left == *other.left) && (*right == *other.right);
  }

  size_t height() const {
    if (left == nullptr && right == nullptr) {
      return 0;
    }
    size_t height = 0;
    if (left != nullptr) {
      height = left->height();
    }
    if (right != nullptr) {
      height = std::max(height, right->height());
    }

    return 1 + height;
  }

  std::string graphviz() const {
    auto visited = std::set<TreeNode*>{};
    auto result  = std::string{"digraph G {"};

    visited.insert(const_cast<TreeNode*>(this));
    auto q = std::queue<TreeNode const*>{};
    q.push(this);

    while (!q.empty()) {
      auto const* node = q.front();
      q.pop();

      result += std::format("\"{}\" [label=\"{}\"];", static_cast<void const*>(node), node->val);

      if (node->left) {
        result += std::format(
            "\"{}\" -> \"{}\" [label=\"L\"];",
            static_cast<void const*>(node),
            static_cast<void const*>(node->left)
        );
        if (visited.find(node->left) == visited.end()) {
          q.push(node->left);
          visited.insert(node->left);
        }
      }

      if (node->right) {
        result += std::format(
            "\"{}\" -> \"{}\" [label=\"R\"];",
            static_cast<void const*>(node),
            static_cast<void const*>(node->right)
        );
        if (visited.find(node->right) == visited.end()) {
          q.push(node->right);
          visited.insert(node->right);
        }
      }
    }

    result += "}\n";

    return result;
  }
};

static inline void delete_nodes(TreeNode* node) {
  if (node == nullptr) {
    return;
  }
  delete_nodes(node->left);
  delete_nodes(node->right);
  delete node;
}

static inline void clean_up(TreeNode** node) {
  delete_nodes(*node);
  *node= nullptr;
}
