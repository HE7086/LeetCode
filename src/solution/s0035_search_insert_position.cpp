/******************************
__DEBUG_FUNCTION<searchInsert>__
Question 35: Search Insert Position
Difficulty: Easy

Given a sorted array of distinct integers and a target value, return the index if the target is found. If not, return the index where it would be if it were inserted in order.
You must write an algorithm with O(log n) runtime complexity.

Example 1:

Input: nums = [1,3,5,6], target = 5
Output: 2

Example 2:

Input: nums = [1,3,5,6], target = 2
Output: 1

Example 3:

Input: nums = [1,3,5,6], target = 7
Output: 4


Constraints:

  1 <= nums.length <= 10^4
  -10^4 <= nums[i] <= 10^4
  nums contains distinct values sorted in ascending order.
  -10^4 <= target <= 10^4

URL: https://leetcode.com/problems/search-insert-position
******************************/

#include <algorithm>
#include <vector>
#include <gtest/gtest.h>

using namespace std;

//==============================================================================

/* O(log(n)) time; O(1) space
 * Binary search. Same as 34.
 */
class Solution {
public:
  int searchInsert(vector<int>& nums, int target) {
    return distance(nums.begin(), lower_bound(nums.begin(), nums.end(), target));
  }
};

//==============================================================================

TEST(Test, s0035_search_insert_position) {
  auto check = [](int expected, vector<int> nums, int target) {
    EXPECT_EQ(expected, Solution{}.searchInsert(nums, target));
  };
  check(2, {1, 3, 5, 6}, 5);
  check(1, {1, 3, 5, 6}, 2);
  check(4, {1, 3, 5, 6}, 7);
}

