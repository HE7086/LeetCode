/******************************
Question 1: Two Sum

Given an array of integers numsand an integer target, return indices of the two numbers such that they add up to target.
You may assume that each input would have exactly one solution, and you may not use the same element twice.
You can return the answer in any order.

Example 1:

Input: nums = [2,7,11,15], target = 9
Output: [0,1]
Explanation: Because nums[0] + nums[1] == 9, we return [0, 1].

Example 2:

Input: nums = [3,2,4], target = 6
Output: [1,2]

Example 3:

Input: nums = [3,3], target = 6
Output: [0,1]


Constraints:

        2 <= nums.length <= 104
        -109 <= nums[i] <= 109
        -109 <= target <= 109
        Only one valid answer exists.


Follow-up:Can you come up with an algorithm that is less than O(n2)time complexity?
URL: https://leetcode.com/problems/two-sum
******************************/

#include <unordered_map>
#include <vector>
#include <gtest/gtest.h>

using namespace std;

//==============================================================================

class Solution {
public:
  vector<int> twoSum(vector<int> const& nums, int target) {
    auto map = unordered_map<int, int>{};
    for (int i = 0; i < nums.size(); ++i) {
      int n = target - nums[i];
      if (auto it = map.find(n); it != map.end()) {
        return {it->second, i};
      } else {
        map.insert({nums[i], i});
      }
    }
    return {};
  }
};

//==============================================================================

TEST(Test, s0001_two_sum) {
  auto s = Solution{};
  EXPECT_EQ((std::vector{0, 1}), s.twoSum({2, 7, 11, 15}, 9));
  EXPECT_EQ((std::vector{1, 2}), s.twoSum({3, 2, 4}, 6));
  EXPECT_EQ((std::vector{0, 1}), s.twoSum({3, 3}, 6));
}
