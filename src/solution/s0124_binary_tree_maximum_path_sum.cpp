/******************************
__DEBUG_FUNCTION<maxPathSum>__
Question 124: Binary Tree Maximum Path Sum
Difficulty: Hard

A path in a binary tree is a sequence of nodes where each pair of adjacent nodes in the sequence has an edge connecting them. A node can only appear in the sequence at most once. Note that the path does not need to pass through the root.
The path sum of a path is the sum of the node's values in the path.
Given the root of a binary tree, return the maximum path sum of any non-empty path.

Example 1:

Input: root = [1,2,3]
Output: 6
Explanation: The optimal path is 2 -> 1 -> 3 with a path sum of 2 + 1 + 3 = 6.

Example 2:

Input: root = [-10,9,20,null,null,15,7]
Output: 42
Explanation: The optimal path is 15 -> 20 -> 7 with a path sum of 15 + 20 + 7 = 42.


Constraints:

  The number of nodes in the tree is in the range [1, 3 * 10^4].
  -1000 <= Node.val <= 1000

URL: https://leetcode.com/problems/binary-tree-maximum-path-sum
******************************/

#include <algorithm>
#include <limits>
#include <gtest/gtest.h>
#include <utils/tree.hpp>

using namespace std;

//==============================================================================

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
  int max_path = numeric_limits<int>::min();
  int dfs(TreeNode* node) {
    if (!node) {
      return 0;
    }

    int left = max(0, dfs(node->left));
    int right = max(0, dfs(node->right));
    max_path = max(max_path, node->val + left + right);

    return node->val + max(left, right);
  }
public:
  int maxPathSum(TreeNode* root) {
    dfs(root);
    return max_path;
  }
};

//==============================================================================

TEST(Test, s0124_binary_tree_maximum_path_sum) {
  {
    SafeTree* a = new TreeNode(1);
    SafeTree* b = new TreeNode(2);
    SafeTree* c = new TreeNode(3);
    a->left = b;
    a->right = c;
    EXPECT_EQ(6, Solution{}.maxPathSum(a));
  }
  {
    SafeTree* a = new TreeNode(-10);
    SafeTree* b = new TreeNode(9);
    SafeTree* c = new TreeNode(20);
    SafeTree* d = new TreeNode(15);
    SafeTree* e = new TreeNode(7);
    a->left = b;
    a->right = c;
    c->left = d;
    c->right = e;
    EXPECT_EQ(42, Solution{}.maxPathSum(a));
  }
}

