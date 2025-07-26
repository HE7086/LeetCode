/******************************
__DEBUG_FUNCTION<searchRange>__
Question 34: Find First and Last Position of Element in Sorted Array
Difficulty: Med.

Given an array of integers nums sorted in non-decreasing order, find the starting and ending position of a given target value.
If target is not found in the array, return [-1, -1].
You must write an algorithm with O(log n) runtime complexity.

Example 1:
Input: nums = [5,7,7,8,8,10], target = 8
Output: [3,4]
Example 2:
Input: nums = [5,7,7,8,8,10], target = 6
Output: [-1,-1]
Example 3:
Input: nums = [], target = 0
Output: [-1,-1]

Constraints:

  0 <= nums.length <= 10^5
  -10^9 <= nums[i] <= 10^9
  nums is a non-decreasing array.
  -10^9 <= target <= 10^9

URL: https://leetcode.com/problems/find-first-and-last-position-of-element-in-sorted-array
******************************/

#include <algorithm>
#include <iterator>
#include <vector>
#include <gtest/gtest.h>

using namespace std;

//==============================================================================

/* O(log (n)) time; O(1) space
 * Do binary search twice for the lower and upper bounds.
 * When searching for the lower bound,
 * move the left pointer first when we find a smaller element.
 * When searching for the upper bound,
 * move the right pointer first when we find a greater element.
 * But stl already has queal_range, so..
 */
class Solution {
public:
  vector<int> searchRange(vector<int>& nums, int target) {
    auto [lower, upper] = equal_range(nums.begin(), nums.end(), target);

    if (lower == upper) {
      return {-1, -1};
    }

    return {static_cast<int>(distance(nums.begin(), lower)), static_cast<int>(distance(nums.begin(), upper)) - 1};
  }
};

//==============================================================================

TEST(Test, s0034_find_first_and_last_position_of_element_in_sorted_array) {
  auto check = [](vector<int> expected, vector<int> nums, int target) {
    EXPECT_EQ(expected, Solution{}.searchRange(nums, target));
  };
  check({3, 4}, {5, 7, 7, 8, 8, 10}, 8);
  check({-1, -1}, {5, 7, 7, 8, 8, 10}, 6);
  check({-1, -1}, {}, 0);
  check({0, 0}, {1}, 1);
}

