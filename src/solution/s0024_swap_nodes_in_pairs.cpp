/******************************
__DEBUG_FUNCTION<swapPairs>__
Question 24: Swap Nodes in Pairs
Difficulty: Med.

Given a linked list, swap every two adjacent nodes and return its head. You must solve the problem without modifying the values in the list's nodes (i.e., only nodes themselves may be changed.)

Example 1:

Input: head = [1,2,3,4]
Output: [2,1,4,3]
Explanation:


Example 2:

Input: head = []
Output: []

Example 3:

Input: head = [1]
Output: [1]

Example 4:

Input: head = [1,2,3]
Output: [2,1,3]


Constraints:

  The number of nodes in the list is in the range [0, 100].
  0 <= Node.val <= 100

URL: https://leetcode.com/problems/swap-nodes-in-pairs
******************************/

#include <gtest/gtest.h>
#include <utils/list.hpp>

using namespace std;

//==============================================================================

/* O(n) time; O(n) space (for recursive call stack)
 * Pretty straight forward with recursive call.
 */
class Solution {
public:
  ListNode* swapPairs(ListNode* head) {
    if (head == nullptr || head->next == nullptr) {
      return head;
    }
    auto* next_head = head->next->next;

    auto* ptr       = head->next;
    ptr->next       = head;
    ptr->next->next = swapPairs(next_head);

    return ptr;
  }

  /* O(n) time; O(1) space iterative version
   * needs more temporary values, but don't require recursive call stack
   */
  ListNode* swapPairs_it(ListNode* head) {
    auto result = ListNode(-1, head);

    auto* ptr  = head;
    auto* prev = &result;

    while (ptr && ptr->next) {
      auto* next_head = ptr->next->next;
      auto* temp      = ptr->next;

      temp->next = ptr;
      ptr->next  = next_head;
      prev->next = temp;

      prev = ptr;
      ptr  = next_head;
    }

    return result.next;
  }
};

//==============================================================================

TEST(Test, s0024_swap_nodes_in_pairs) {
  auto s = Solution{};
  {
    auto [a, _] = make_list_owned({});
    EXPECT_EQ(nullptr, s.swapPairs(a));
  }
  {
    auto [a, _] = make_list_owned({1});
    auto [b, _] = make_list_owned({1});
    EXPECT_EQ(*b, *s.swapPairs(a));
  }
  {
    auto [a, _] = make_list_owned({1, 2, 3});
    auto [b, _] = make_list_owned({2, 1, 3});
    EXPECT_EQ(*b, *s.swapPairs(a));
  }
  {
    auto [a, _] = make_list_owned({1, 2, 3, 4});
    auto [b, _] = make_list_owned({2, 1, 4, 3});
    EXPECT_EQ(*b, *s.swapPairs(a));
  }
}
