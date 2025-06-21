/******************************
__DEBUG_FUNCTION<threeSumClosest>__
Question 16: 3Sum Closest
Difficulty: Med.

Given an integer array nums of length n and an integer target, find three integers in nums such that the sum is closest to target.
Return the sum of the three integers.
You may assume that each input would have exactly one solution.

Example 1:

Input: nums = [-1,2,1,-4], target = 1
Output: 2
Explanation: The sum that is closest to the target is 2. (-1 + 2 + 1 = 2).

Example 2:

Input: nums = [0,0,0], target = 1
Output: 0
Explanation: The sum that is closest to the target is 0. (0 + 0 + 0 = 0).


Constraints:

  3 <= nums.length <= 500
  -1000 <= nums[i] <= 1000
  -10^4 <= target <= 10^4

URL: https://leetcode.com/problems/3sum-closest
******************************/

#include <algorithm>
#include <iterator>
#include <ranges>
#include <vector>
#include <gtest/gtest.h>

using namespace std;

//==============================================================================

/* O(n^2 + n * log(n)) time; O(1) space
 * Basically exactly the same as s0015.
 */
class Solution {
public:
  int threeSumClosest(vector<int> nums, int target) {
    sort(nums.begin(), nums.end());
    int ans = ranges::fold_left_first(nums | views::take(3), plus{}).value();

    for (auto base = nums.begin(); base != nums.end(); base++) {
      // not necessary, we don't care about duplicates here
      if (base != nums.begin() && *prev(base) == *base) continue;

      auto left = next(base);
      auto right = prev(nums.end());
      while (left < right) {
        int sum = *base + *left + *right;
        if (sum == target) {
          return target;
        }
        if (abs(sum - target) < abs(ans - target)) {
          ans = sum;
        }
        if (sum < target) {
          left++;
        } else {
          right--;
        }
      }
    }
    return ans;
  }
};

//==============================================================================

TEST(Test, s0016_3sum_closest) {
  auto s = Solution{};
  EXPECT_EQ(2, s.threeSumClosest({-1, 2, 1, -4}, 1));
  EXPECT_EQ(0, s.threeSumClosest({0, 0, 0}, 1));
}

