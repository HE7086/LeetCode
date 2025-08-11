/******************************
__DEBUG_FUNCTION<reverseKGroup>__
Question 25: Reverse Nodes in k-Group
Difficulty: Hard

Given the head of a linked list, reverse the nodes of the list k at a time, and return the modified list.
k is a positive integer and is less than or equal to the length of the linked list. If the number of nodes is not a multiple of k then left-out nodes, in the end, should remain as it is.
You may not alter the values in the list's nodes, only nodes themselves may be changed.

Example 1:

Input: head = [1,2,3,4,5], k = 2
Output: [2,1,4,3,5]

Example 2:

Input: head = [1,2,3,4,5], k = 3
Output: [3,2,1,4,5]


Constraints:

  The number of nodes in the list is n.
  1 <= k <= n <= 5000
  0 <= Node.val <= 1000


Follow-up: Can you solve the problem in O(1) extra memory space?

URL: https://leetcode.com/problems/reverse-nodes-in-k-group
******************************/

#include <gtest/gtest.h>
#include <utils/list.hpp>

using namespace std;

//==============================================================================

/* O(n) time; O(1) space
 * First create a function that reverse the list in a range
 * Then iterate over the list with fast and slow pointers and reverse each interval.
 */
class Solution {
  // reverse the list in [begin, end) and return new begin
  // 1, 2, 3 -> 2, 1, 3
  ListNode* reverse(ListNode* begin, ListNode* end) {
    if (!begin || !begin->next) return begin;

    auto* ptr = begin;
    auto* a   = begin;
    auto* b   = a->next;

    // first element will become last after reverse
    a->next = end;

    while (b != end) {
      auto* c = b->next;

      ptr     = b;
      b->next = a;

      a = b;
      b = c;
    }

    return ptr;
  }

public:
  ListNode* reverseKGroup(ListNode* head, int k) {
    auto  dummy = ListNode(0, head);
    auto* prev  = &dummy;
    auto* begin = head;
    auto* end   = head;

    while (begin) {
      for (int i = 0; i < k; i++) {
        if (!end) {
          // not sufficient elements
          return dummy.next;
        }
        end = end->next;
      }
      prev->next = reverse(begin, end);
      prev       = begin;
      begin      = end;
    }

    return dummy.next;
  }
};

//==============================================================================

TEST(Test, s0025_reverse_nodes_in_k_group) {
  auto s = Solution{};
  {
    auto [a, _] = ListNode::make_list_owned({1, 2});
    auto [b, _] = ListNode::make_list_owned({2, 1});
    EXPECT_EQ(*b, *s.reverseKGroup(a, 2));
  }
  {
    auto [a, _] = ListNode::make_list_owned({1, 2, 3});
    auto [b, _] = ListNode::make_list_owned({2, 1, 3});
    EXPECT_EQ(*b, *s.reverseKGroup(a, 2));
  }
  {
    auto [a, _] = ListNode::make_list_owned({1, 2, 3});
    auto [b, _] = ListNode::make_list_owned({3, 2, 1});
    EXPECT_EQ(*b, *s.reverseKGroup(a, 3));
  }
  {
    auto [a, _] = ListNode::make_list_owned({1, 2, 3, 4, 5});
    auto [b, _] = ListNode::make_list_owned({2, 1, 4, 3, 5});
    EXPECT_EQ(*b, *s.reverseKGroup(a, 2));
  }
  {
    auto [a, _] = ListNode::make_list_owned({1, 2, 3, 4, 5});
    auto [b, _] = ListNode::make_list_owned({3, 2, 1, 4, 5});
    EXPECT_EQ(*b, *s.reverseKGroup(a, 3));
  }
}
