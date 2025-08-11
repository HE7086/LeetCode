/******************************
__DEBUG_FUNCTION<maxCoins>__
Question 312: Burst Balloons
Difficulty: Hard

You are given n balloons, indexed from 0 to n - 1. Each balloon is painted with a number on it represented by an array nums. You are asked to burst all the balloons.
If you burst the i^th balloon, you will get nums[i - 1] * nums[i] * nums[i + 1] coins. If i - 1 or i + 1 goes out of bounds of the array, then treat it as if there is a balloon with a 1 painted on it.
Return the maximum coins you can collect by bursting the balloons wisely.

Example 1:

Input: nums = [3,1,5,8]
Output: 167
Explanation:
nums = [3,1,5,8] --> [3,5,8] --> [3,8] --> [8] --> []
coins =  3*1*5    +   3*5*8   +  1*3*8  + 1*8*1 = 167
Example 2:

Input: nums = [1,5]
Output: 10


Constraints:

  n == nums.length
  1 <= n <= 300
  0 <= nums[i] <= 100

URL: https://leetcode.com/problems/burst-balloons
******************************/

#include <algorithm>
#include <ranges>
#include <vector>
#include <gtest/gtest.h>

using namespace std;

//==============================================================================

/* O(n^3) time; O(n^2) space
 *
 * dynamic programming.
 * for each sub ranges, the solution to the sub problem depends on the last element popped.
 *
 * dp[i][j] = best value from interval [i, j]
 */
class Solution {
public:
  int maxCoins(vector<int> const& nums) {
#if __cpp_lib_ranges_concat >= 202403L
    auto val = views::concat(views::single(1), views::concat(nums, views::single(1)));
#else
    auto val    = vector<int>(nums.size() + 2);
    val.front() = 1;
    val.back()  = 1;
    copy(nums.begin(), nums.end(), next(val.begin()));
#endif
    size_t n = val.size();

    vector<vector<int>> dp(n, vector<int>(n, 0));

    for (size_t i = 2; i < n; i++) {
      for (size_t j = 0; j < n - i; j++) {
        for (size_t k = j + 1; k < i + j; k++) {
          dp[j][i + j] = max(dp[j][i + j], dp[j][k] + dp[k][i + j] + val[j] * val[k] * val[i + j]);
        }
      }
    }
    return dp[0][n - 1];
  }
};

//==============================================================================

TEST(Test, s0312_burst_balloons) {
  EXPECT_EQ(167, Solution{}.maxCoins({3, 1, 5, 8}));
  EXPECT_EQ(10, Solution{}.maxCoins({1, 5}));
}

