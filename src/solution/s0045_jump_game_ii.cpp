/******************************
__DEBUG_FUNCTION<jump>__
Question 45: Jump Game II
Difficulty: Med.

You are given a 0-indexed array of integers nums of length n. You are initially positioned at nums[0].
Each element nums[i] represents the maximum length of a forward jump from index i. In other words, if you are at nums[i], you can jump to any nums[i + j] where:

  0 <= j <= nums[i] and
  i + j < n

Return the minimum number of jumps to reach nums[n - 1]. The test cases are generated such that you can reach nums[n - 1].

Example 1:

Input: nums = [2,3,1,1,4]
Output: 2
Explanation: The minimum number of jumps to reach the last index is 2. Jump 1 step from index 0 to 1, then 3 steps to the last index.

Example 2:

Input: nums = [2,3,0,1,4]
Output: 2


Constraints:

  1 <= nums.length <= 10^4
  0 <= nums[i] <= 1000
  It's guaranteed that you can reach nums[n - 1].

URL: https://leetcode.com/problems/jump-game-ii
******************************/

#include <algorithm>
#include <vector>
#include <gtest/gtest.h>

using namespace std;

//==============================================================================

/* O(n^2) time; O(n) space
 *
 * Dynamic Programming.
 */
/*class Solution {
 * public:
 *   int jump(vector<int> const& nums) {
 *     auto dp = vector<int>(nums.size(), nums.size());
 *     dp[0] = 0;
 *
 *     for (size_t i = 0; i < nums.size(); i++) {
 *       for (size_t j = i + 1; j < i + nums[i] + 1; j++) {
 *         if (j >= nums.size()) {
 *           break;
 *         }
 *         if (dp[j] > dp[i] + 1) {
 *           dp[j] = dp[i] + 1;
 *         }
 *       }
 *     }
 *
 *     return dp.back();
 *   }
 * };
 */

/* O(n) time; O(1) space
 *
 * Greedy BFS
 */
class Solution {
public:
  int jump(vector<int> const& nums) {
    size_t start = 0; // start index of current iteration
    size_t last  = 0; // farthest index reached in last iteration
    int    ans   = 0;

    while (last < nums.size() - 1) {
      size_t max = 0;
      for (size_t i = start; i < last + 1; i++) {
        max = std::max(max, i + nums[i]);
      }

      start = last + 1;
      last  = max;
      ans++;
    }
    return ans;
  }
};

//==============================================================================

TEST(Test, s0045_jump_game_ii) {
  auto s = Solution{};
  EXPECT_EQ(2, s.jump({2, 3, 1, 1, 4}));
  EXPECT_EQ(2, s.jump({2, 3, 0, 1, 4}));
}

