/******************************
__DEBUG_FUNCTION<mergeKLists>__
Question 23: Merge k Sorted Lists
Difficulty: Hard

You are given an array of k linked-lists lists, each linked-list is sorted in ascending order.
Merge all the linked-lists into one sorted linked-list and return it.

Example 1:

Input: lists = [[1,4,5],[1,3,4],[2,6]]
Output: [1,1,2,3,4,4,5,6]
Explanation: The linked-lists are:
[
  1->4->5,
  1->3->4,
  2->6
]
merging them into one sorted list:
1->1->2->3->4->4->5->6

Example 2:

Input: lists = []
Output: []

Example 3:

Input: lists = [[]]
Output: []


Constraints:

  k == lists.length
  0 <= k <= 10^4
  0 <= lists[i].length <= 500
  -10^4 <= lists[i][j] <= 10^4
  lists[i] is sorted in ascending order.
  The sum of lists[i].length will not exceed 10^4.

URL: https://leetcode.com/problems/merge-k-sorted-lists
******************************/

#include <vector>
#include <gtest/gtest.h>
#include <utils/list.hpp>

using namespace std;

//==============================================================================

/* class Solution {
 * public:
 *   ListNode* mergeKLists(vector<ListNode*>& lists) {
 *     auto ls = lists
 *       | views::filter([](auto* ptr) { return ptr != nullptr; })
 *       | ranges::to<vector<ListNode*>>();
 *
 *     if (ls.empty()) {
 *       return nullptr;
 *     }
 *
 *     auto compare = [](ListNode* a, ListNode* b) { return a->val < b->val; };
 *     auto advance = [&ls](auto& it) {
 *       *it = it.operator*()->next;
 *       if (*it == nullptr) {
 *         ls.erase(it);
 *       }
 *     };
 *     auto  me   = min_element(ls.begin(), ls.end(), compare);
 *     auto* head = *me;
 *     auto* ptr  = head;
 *     advance(me);
 *
 *     while (!ls.empty()) {
 *       auto minimum = min_element(ls.begin(), ls.end(), compare);
 *
 *       ptr->next = *minimum;
 *       ptr       = ptr->next;
 *       advance(minimum);
 *     }
 *
 *     return head;
 *   }
 * };
 */

/* O(n * log(k)) time; O(1) space if merge in-place
 *
 * Basically just repeat merge two lists.
 * Importantly the merges are done on adjancent lists like in merge sort,
 * so that one list won't grow too long where we need to start from the beginning each time.
 */
class Solution {
  ListNode* mergeTwoLists(ListNode* list1, ListNode* list2) {
    if (list1 == nullptr) {
      return list2;
    }
    if (list2 == nullptr) {
      return list1;
    }
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
public:
  ListNode* mergeKLists(vector<ListNode*>& lists) {
    if (lists.empty()) {
      return nullptr;
    }

    for (size_t step = 1; step < lists.size(); step *= 2) {
      for (size_t i = 0; i < lists.size() - step; i += step * 2) {
        lists[i] = mergeTwoLists(lists[i], lists[i + step]);
      }
    }
    return lists[0];
  }
};

//==============================================================================

TEST(Test, s0023_merge_k_sorted_lists) {
  auto s = Solution{};
  {
    auto input = vector<ListNode*>{};
    EXPECT_EQ(nullptr, s.mergeKLists(input));
  }
  {
    auto [a, as] = make_list_owned({});
    auto input   = vector<ListNode*>{a};
    EXPECT_EQ(nullptr, s.mergeKLists(input));
  }
  {
    auto [a, as] = make_list_owned({});
    auto [b, bs] = make_list_owned({1});
    auto [d, ds] = make_list_owned({1});
    auto input   = vector<ListNode*>{a, b};
    EXPECT_EQ(*d, *s.mergeKLists(input));
  }
  {
    auto [a, as] = make_list_owned({1});
    auto [b, bs] = make_list_owned({1, 2, 3});
    auto [d, ds] = make_list_owned({1, 1, 2, 3});
    auto input   = vector<ListNode*>{a, b};
    EXPECT_EQ(*d, *s.mergeKLists(input));
  }
  {
    auto [a, as] = make_list_owned({1, 4, 5});
    auto [b, bs] = make_list_owned({1, 3, 4});
    auto [c, cs] = make_list_owned({2, 6});
    auto [d, ds] = make_list_owned({1, 1, 2, 3, 4, 4, 5, 6});
    auto input   = vector<ListNode*>{a, b, c};
    EXPECT_EQ(*d, *s.mergeKLists(input));
  }
}
