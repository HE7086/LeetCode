/******************************
__DEBUG_FUNCTION<mergeTwoLists>__
Question 21: Merge Two Sorted Lists
Difficulty: Easy

You are given the heads of two sorted linked lists list1 and list2.
Merge the two lists into one sorted list. The list should be made by splicing together the nodes of the first two lists.
Return the head of the merged linked list.

Example 1:

Input: list1 = [1,2,4], list2 = [1,3,4]
Output: [1,1,2,3,4,4]

Example 2:

Input: list1 = [], list2 = []
Output: []

Example 3:

Input: list1 = [], list2 = [0]
Output: [0]


Constraints:

  The number of nodes in both lists is in the range [0, 50].
  -100 <= Node.val <= 100
  Both list1 and list2 are sorted in non-decreasing order.

URL: https://leetcode.com/problems/merge-two-sorted-lists
******************************/

#include <gtest/gtest.h>
#include <utils/list.hpp>

using namespace std;

//==============================================================================

/* O(n) time; O(1) space if merging in-place
 * Trivial list merging.
 * Just take care about the edge cases.
 */
class Solution {
public:
  ListNode* mergeTwoLists(ListNode* list1, ListNode* list2) {
    if (list1 == nullptr) {
      return list2;
    }
    if (list2 == nullptr) {
      return list1;
    }

    // using the smaller head as base.
    if (list1->val > list2->val) {
      return mergeTwoLists(list2, list1);
    }

    auto* ptr = list1;
    auto* ptr1 = list1->next;
    auto* ptr2 = list2;
    while (ptr1 != nullptr && ptr2 != nullptr) {
      if (ptr1->val < ptr2->val) {
        ptr->next = ptr1;
        ptr1 = ptr1->next;
      } else {
        ptr->next = ptr2;
        ptr2 = ptr2->next;
      }
      ptr = ptr->next;
    }
    if (ptr1 != nullptr) {
      ptr->next = ptr1;
    }
    if (ptr2 != nullptr) {
      ptr->next = ptr2;
    }
    return list1;
  }
};

//==============================================================================

TEST(Test, s0021_merge_two_sorted_lists) {
  auto s = Solution{};
  {
    auto [a, _] = ListNode::make_list_owned({1, 2, 4});
    auto [b, _] = ListNode::make_list_owned({1, 3, 4});
    auto [c, _] = ListNode::make_list_owned({1, 1, 2, 3, 4, 4});
    EXPECT_EQ(*c, *s.mergeTwoLists(a, b));
  }
  {
    auto [a, _] = ListNode::make_list_owned({});
    auto [b, _] = ListNode::make_list_owned({});
    EXPECT_EQ(nullptr, s.mergeTwoLists(a, b));
  }
  {
    auto [a, _] = ListNode::make_list_owned({});
    auto [b, _] = ListNode::make_list_owned({0});
    auto [c, _] = ListNode::make_list_owned({0});
    EXPECT_EQ(*c, *s.mergeTwoLists(a, b));
  }
}

