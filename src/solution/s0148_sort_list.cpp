/******************************
__DEBUG_FUNCTION<sortList>__
Question 148: Sort List
Difficulty: Med.

Given the head of a linked list, return the list after sorting it in ascending order.

Example 1:

Input: head = [4,2,1,3]
Output: [1,2,3,4]

Example 2:

Input: head = [-1,5,3,4,0]
Output: [-1,0,3,4,5]

Example 3:

Input: head = []
Output: []


Constraints:

  The number of nodes in the list is in the range [0, 5 * 10^4].
  -10^5 <= Node.val <= 10^5


Follow up: Can you sort the linked list in O(n logn) time and O(1) memory (i.e. constant space)?

URL: https://leetcode.com/problems/sort-list
******************************/

#include <gtest/gtest.h>
#include <utils/list.hpp>

using namespace std;

//==============================================================================

/*
 * in-place merge sort
 */
class Solution {
public:
  ListNode* merge(ListNode* left, ListNode* right) {
    if (left == nullptr) {
      return right;
    }
    if (right == nullptr) {
      return left;
    }

    auto  dummy = ListNode{};
    auto* ptr   = &dummy;

    while (left != nullptr && right != nullptr) {
      if (left->val < right->val) {
        ptr->next = left;
        ptr       = left;
        left      = left->next;
      } else {
        ptr->next = right;
        ptr       = right;
        right     = right->next;
      }
    }
    while (left != nullptr) {
      ptr->next = left;
      ptr       = left;
      left      = left->next;
    }
    while (right != nullptr) {
      ptr->next = right;
      ptr       = right;
      right     = right->next;
    }
    return dummy.next;
  }

  ListNode* sortList(ListNode* head) {
    if (head == nullptr || head->next == nullptr) {
      return head;
    }

    // find mid using 2 pointers
    auto* slow = head;
    auto* fast = head->next;
    while (fast != nullptr && fast->next != nullptr) {
      slow = slow->next;
      fast = fast->next->next;
    }

    auto* left  = head;
    auto* right = slow->next;
    slow->next  = nullptr;

    left  = sortList(left);
    right = sortList(right);

    return merge(left, right);
  }
};

//==============================================================================

TEST(Test, s0148_sort_list) {
  {
    auto [a, _] = make_list_owned({4, 2, 1, 3});
    auto [b, _] = make_list_owned({1, 2, 3, 4});
    a           = Solution{}.sortList(a);
    EXPECT_EQ(*a, *b);
  }
  {
    auto [a, _] = make_list_owned({-1, 5, 3, 4, 0});
    auto [b, _] = make_list_owned({-1, 0, 3, 4, 5});
    a           = Solution{}.sortList(a);
    EXPECT_EQ(*a, *b);
  }
  {
    auto [a, _] = make_list_owned({});
    a           = Solution{}.sortList(a);
    EXPECT_EQ(a, nullptr);
  }
}

