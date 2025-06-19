/******************************
Question 2: Add Two Numbers

You are given two non-empty linked lists representing two non-negative integers. The digits are stored in reverse order, and each of their nodes contains a single digit. Add the two numbers and return the sumas a linked list.
You may assume the two numbers do not contain any leading zero, except the number 0 itself.

Example 1:

Input: l1 = [2,4,3], l2 = [5,6,4]
Output: [7,0,8]
Explanation: 342 + 465 = 807.

Example 2:

Input: l1 = [0], l2 = [0]
Output: [0]

Example 3:

Input: l1 = [9,9,9,9,9,9,9], l2 = [9,9,9,9]
Output: [8,9,9,9,0,0,0,1]


Constraints:

    The number of nodes in each linked list is in the range [1, 100].
    0 <= Node.val <= 9
    It is guaranteed that the list represents a number that does not have leading zeros.

URL: https://leetcode.com/problems/add-two-numbers
******************************/

#include <gtest/gtest.h>
#include <utils/list.hpp>

using namespace std;

//==============================================================================

/* O(n) time; O(n) space
 * Basically just a full adder for a list, nothing fancy.
 * The most difficult part is actually writing a memory safe and easy to use version of the ListNode(SafeList here).
 */

class Solution {
public:
  ListNode* addTwoNumbers(ListNode* l1, ListNode* l2) {
    int       carry = 0;
    ListNode* l3    = new ListNode();
    ListNode* ptr   = l3;

    while (l1 != nullptr || l2 != nullptr) {
      int sum = carry;
      if (l1) {
        sum += l1->val;
        l1 = l1->next;
      }
      if (l2) {
        sum += l2->val;
        l2 = l2->next;
      }

      carry    = sum / 10;
      ptr->val = sum % 10;

      if (l1 || l2) {
        ptr->next = new ListNode();
        ptr       = ptr->next;
      }
    }

    if (carry != 0) {
      ptr->next = new ListNode(carry);
    }

    return l3;
  }
};

//==============================================================================

TEST(Test, s0002_add_two_numbers) {
  auto s = Solution{};
  {
    SafeList* a = make_nodes({2, 4, 3});
    SafeList* b = make_nodes({5, 6, 4});
    SafeList* c = make_nodes({7, 0, 8});
    SafeList* d = s.addTwoNumbers(a, b);
    EXPECT_EQ(*c, *d);
  }
  {
    SafeList* a = make_nodes({0});
    SafeList* b = make_nodes({0});
    SafeList* c = make_nodes({0});
    SafeList* d = s.addTwoNumbers(a, b);
    EXPECT_EQ(*c, *d);
  }
  {
    SafeList* a = make_nodes({9, 9, 9, 9, 9, 9, 9});
    SafeList* b = make_nodes({9, 9, 9, 9});
    SafeList* c = make_nodes({8, 9, 9, 9, 0, 0, 0, 1});
    SafeList* d = s.addTwoNumbers(a, b);
    EXPECT_EQ(*c, *d);
  }
}
