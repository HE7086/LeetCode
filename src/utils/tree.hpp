#pragma once
#include <algorithm>
#include <format>
#include <initializer_list>
#include <queue>
#include <set>
#include <string>

#define SafeTree TreeNode __attribute__((__cleanup__(clean_up_tree)))

/**
 * Helper struct for working with LeetCode's tree questions.
 * The struct definition is exactly the same as official one, with some helper function to make life easier.
 *
 * The "SafeTree" macro will auto release memory.
 * Make sure every node is declared using the macro.
 *
 * We can't use smart pointer here because the solution then will not be able to be submitted (yikes).
 *
 * Usage:
 * SafeTree* a = new TreeNode();
 * SafeTree* b = new TreeNode();
 * EXPECT_EQ(*a, *b);
 */
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
    if (left == nullptr && right == nullptr && other.left == nullptr && other.right == nullptr) {
      return true;
    }
    bool flag = false;
    if (left != nullptr) {
      flag = *left == *other.left;
    }
    if (right != nullptr) {
      flag = *right == *other.right;
    }
    return flag;
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
    auto result  = std::string{"digraph G {"};
    auto visited = std::set<TreeNode const*>{};
    auto queue   = std::queue<TreeNode const*>{};

    visited.insert(this);
    queue.push(this);

    while (!queue.empty()) {
      auto const* node = queue.front();
      queue.pop();

      result += std::format("\"{}\" [label=\"{}\"];", static_cast<void const*>(node), node->val);

      if (node->left) {
        result += std::format(
            "\"{}\" -> \"{}\" [label=\"L\"];",
            static_cast<void const*>(node),
            static_cast<void const*>(node->left)
        );
        if (visited.find(node->left) == visited.end()) {
          queue.push(node->left);
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
          queue.push(node->right);
          visited.insert(node->right);
        }
      }
    }

    result += "}\n";

    return result;
  }

  [[nodiscard]] static std::pair<TreeNode*, std::vector<TreeNode>>
  make_tree_owned(std::initializer_list<std::optional<int>> values) {
    if (values.size() == 0 || !values.begin()->has_value()) {
      return {nullptr, {}};
    }

    auto nodes = std::vector<TreeNode>{values.begin()->value()};
    nodes.reserve(values.size());

    auto it    = next(values.begin());
    auto queue = std::queue<TreeNode*>{};
    queue.push(&nodes.front());

    while (!queue.empty()) {
      auto* node = queue.front();
      queue.pop();

      if (it != values.end()) {
        if (it->has_value()) {
          nodes.emplace_back(it->value());
          node->left = &nodes.back();
          queue.push(node->left);
        }
        it++;
      }

      if (it != values.end()) {
        if (it->has_value()) {
          nodes.emplace_back(it->value());
          node->right = &nodes.back();
          queue.push(node->right);
        }
        it++;
      }
    }

    return {&nodes.front(), std::move(nodes)};
  }
};

static inline void clean_up_tree(TreeNode** node) {
  delete *node;
  *node = nullptr;
}
