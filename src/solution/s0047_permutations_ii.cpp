/******************************
__DEBUG_FUNCTION<permuteUnique>__
Question 47: Permutations II
Difficulty: Med.

Given a collection of numbers, nums, that might contain duplicates, return all possible unique permutations in any order.

Example 1:

Input: nums = [1,1,2]
Output:
[[1,1,2],
 [1,2,1],
 [2,1,1]]

Example 2:

Input: nums = [1,2,3]
Output: [[1,2,3],[1,3,2],[2,1,3],[2,3,1],[3,1,2],[3,2,1]]


Constraints:

  1 <= nums.length <= 8
  -10 <= nums[i] <= 10

URL: https://leetcode.com/problems/permutations-ii
******************************/

#include <vector>
#include <gtest/gtest.h>

using namespace std;

//==============================================================================

/* see 46. permutations
 */
class Solution {
public:
  vector<vector<int>> permuteUnique(vector<int> nums) {
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

TEST(Test, s0047_permutations_ii) {
  auto s = Solution{};
  EXPECT_EQ((vector<vector<int>>{{1,2,3},{1,3,2},{2,1,3},{2,3,1},{3,1,2},{3,2,1}}), s.permuteUnique({1, 2, 3}));
  EXPECT_EQ((vector<vector<int>>{{1, 1, 2}, {1, 2, 1}, {2, 1, 1}}), s.permuteUnique({1, 1, 2}));
  EXPECT_EQ((vector<vector<int>>{{1}}), s.permuteUnique({1}));
}

