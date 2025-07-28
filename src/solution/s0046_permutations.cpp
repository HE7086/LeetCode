/******************************
__DEBUG_FUNCTION<permute>__
Question 46: Permutations
Difficulty: Med.

Given an array nums of distinct integers, return all the possible permutations. You can return the answer in any order.

Example 1:
Input: nums = [1,2,3]
Output: [[1,2,3],[1,3,2],[2,1,3],[2,3,1],[3,1,2],[3,2,1]]
Example 2:
Input: nums = [0,1]
Output: [[0,1],[1,0]]
Example 3:
Input: nums = [1]
Output: [[1]]

Constraints:

  1 <= nums.length <= 6
  -10 <= nums[i] <= 10
  All the integers of nums are unique.

URL: https://leetcode.com/problems/permutations
******************************/

#include <algorithm>
#include <vector>
#include <gtest/gtest.h>

using namespace std;

//==============================================================================

/* see 31. next permutation
 */
class Solution {
public:
  vector<vector<int>> permute(vector<int> nums) {
    sort(nums.begin(), nums.end());
    auto ans = vector<vector<int>>{};
    ans.push_back(nums);
    while (next_permutation(nums.begin(), nums.end())) {
      ans.push_back(nums);
    }
    return ans;
  }
};

//==============================================================================

TEST(Test, s0046_permutations) {
  auto s = Solution{};
  EXPECT_EQ((vector<vector<int>>{{1,2,3},{1,3,2},{2,1,3},{2,3,1},{3,1,2},{3,2,1}}), s.permute({1, 2, 3}));
  EXPECT_EQ((vector<vector<int>>{{0, 1}, {1, 0}}), s.permute({0, 1}));
  EXPECT_EQ((vector<vector<int>>{{1}}), s.permute({1}));
}

