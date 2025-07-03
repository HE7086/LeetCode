/******************************
__DEBUG_FUNCTION<search>__
Question 33: Search in Rotated Sorted Array
Difficulty: Med.

There is an integer array nums sorted in ascending order (with distinct values).
Prior to being passed to your function, nums is possibly rotated at an unknown pivot index k (1 <= k < nums.length) such that the resulting array is [nums[k], nums[k+1], ..., nums[n-1], nums[0], nums[1], ..., nums[k-1]] (0-indexed). For example, [0,1,2,4,5,6,7] might be rotated at pivot index 3 and become [4,5,6,7,0,1,2].
Given the array nums after the possible rotation and an integer target, return the index of target if it is in nums, or -1 if it is not in nums.
You must write an algorithm with O(log n) runtime complexity.

Example 1:
Input: nums = [4,5,6,7,0,1,2], target = 0
Output: 4
Example 2:
Input: nums = [4,5,6,7,0,1,2], target = 3
Output: -1
Example 3:
Input: nums = [1], target = 0
Output: -1

Constraints:

  1 <= nums.length <= 5000
  -10^4 <= nums[i] <= 10^4
  All values of nums are unique.
  nums is an ascending array that is possibly rotated.
  -10^4 <= target <= 10^4

URL: https://leetcode.com/problems/search-in-rotated-sorted-array
******************************/

#include <algorithm>
#include <vector>
#include <gtest/gtest.h>

using namespace std;

//==============================================================================

class Solution {
public:
  /* O(log(n)) time; O(1) space
   *
   * Since the original vector before rotation is sorted, we could use a binary-seach-ish algorithm.
   * Checking the middle point:
   *   * left <= mid  -> [left, mid] is sorted
   *   * mid <= right -> [mid, right] is sorted
   * Then we can easily check whether the target lies in the sorted range. (left <= target <= mid)
   * Based on this we can do binary search in O(log(n))
   */
  int search(vector<int> const& nums, int target) {
    int l = 0;
    int r = nums.size() - 1;

    while (l <= r) {
      int m = (l + r) / 2;

      if (nums[m] == target) {
        return m;
      } else if (nums[l] <= nums[m]) {
        // [l, m] is sorted
        if (nums[l] <= target && target <= nums[m]) {
          r = m - 1;
        } else {
          l = m + 1;
        }
      } else /* nums[m] < nums[r] */ {
        // [m, r] is sorted
        if (nums[m] <= target && target <= nums[r]) {
          l = m + 1;
        } else {
          r = m - 1;
        }
      }
    }

    return -1;
  }

  /* O(n) time; O(1) space
   *
   * first find the middle point where the vector is rotated
   * then do binary search on both partitions.
   *
   * is_sorted_until is typically O(n).
   */
  int search_stl(vector<int> const& nums, int target) {
    auto mid = is_sorted_until(nums.begin(), nums.end());

    auto it = lower_bound(nums.begin(), mid, target);
    if (it != mid && *it == target) {
      return distance(nums.begin(), it);
    }

    it = lower_bound(mid, nums.end(), target);
    if (it != nums.end() && *it == target) {
      return distance(nums.begin(), it);
    }

    return -1;
  }
};

//==============================================================================

TEST(Test, s0033_search_in_rotated_sorted_array) {
  auto s = Solution{};
  EXPECT_EQ(4, s.search({4, 5, 6, 7, 0, 1, 2}, 0));
  EXPECT_EQ(-1, s.search({4, 5, 6, 7, 0, 1, 2}, 3));
  EXPECT_EQ(-1, s.search({1}, 0));
}

