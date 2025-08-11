/******************************
__DEBUG_FUNCTION<removeNthFromEnd>__
Question 19: Remove Nth Node From End of List
Difficulty: Med.

Given the head of a linked list, remove the n^th node from the end of the list and return its head.

Example 1:

Input: head = [1,2,3,4,5], n = 2
Output: [1,2,3,5]

Example 2:

Input: head = [1], n = 1
Output: []

Example 3:

Input: head = [1,2], n = 1
Output: [1]


Constraints:

  The number of nodes in the list is sz.
  1 <= sz <= 30
  0 <= Node.val <= 100
  1 <= n <= sz


Follow up: Could you do this in one pass?

URL: https://leetcode.com/problems/remove-nth-node-from-end-of-list
******************************/

#include <gtest/gtest.h>
#include <utils/list.hpp>

using namespace std;

//==============================================================================

/* O(n) time; O(n) space (for recursive call stack)
 * Find the end of the list, and backtrack the nth node from end.
 * When n is small the majority of the call stack is unnecessary and wasteful.
 */
/*
 * class Solution {
 *   int findEnd(ListNode*& node, int n) {
 *     if (node == nullptr) {
 *       return 1;
 *     } else {
 *       int h = findEnd(node->next, n);
 *       if (h == n) {
 *         auto* ptr = node;
 *         node = node->next;
 *       }
 *       return h + 1;
 *     }
 *   }
 * public:
 *   ListNode* removeNthFromEnd(ListNode* head, int n) {
 *     findEnd(head, n);
 *     return head;
 *   }
 * };
 */


/* O(n) time; O(1) space
 * Fast and slow pointer.
 * Move the fast pointer n steps first, then advance both pointer simultaneously,
 * until fast reaches end. The slow poniter will then be at nth element from end.
 */
class Solution {
public:
  ListNode* removeNthFromEnd(ListNode* head, int n) {
    auto* fast = head;
    auto* slow = head;
    for (int i = 0; i < n; i++) {
      fast = fast->next;
    }
    if (fast == nullptr) {
      // less than n nodes in total
      return head->next;
    }
    while (fast->next != nullptr) {
      fast = fast->next;
      slow = slow->next;
    }
    // remove target
    slow->next = slow->next->next;
    return head;
  }
};

//==============================================================================

TEST(Test, s0019_remove_nth_node_from_end_of_list) {
  auto s = Solution{};
  {
    auto [a, _] = ListNode::make_list_owned({1});
    EXPECT_EQ(nullptr, s.removeNthFromEnd(a, 1));
  }
  {
    auto [a, _] = ListNode::make_list_owned({1, 2, 3, 4, 5});
    auto [b, _] = ListNode::make_list_owned({1, 2, 3, 5});
    s.removeNthFromEnd(a, 2);
    EXPECT_EQ(*b, *a);
  }
  {
    auto [a, _] = ListNode::make_list_owned({1, 2});
    auto [b, _] = ListNode::make_list_owned({1});
    s.removeNthFromEnd(a, 1);
    EXPECT_EQ(*b, *a);
  }
}
