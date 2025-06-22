/******************************
__DEBUG_FUNCTION<fourSum>__
Question 18: 4Sum
Difficulty: Med.

Given an array nums of n integers, return an array of all the unique quadruplets [nums[a], nums[b], nums[c], nums[d]] such that:

  0 <= a, b, c, d < n
  a, b, c, and d are distinct.
  nums[a] + nums[b] + nums[c] + nums[d] == target

You may return the answer in any order.

Example 1:

Input: nums = [1,0,-1,0,-2,2], target = 0
Output: [[-2,-1,1,2],[-2,0,0,2],[-1,0,0,1]]

Example 2:

Input: nums = [2,2,2,2,2], target = 8
Output: [[2,2,2,2]]


Constraints:

  1 <= nums.length <= 200
  -10^9 <= nums[i] <= 10^9
  -10^9 <= target <= 10^9

URL: https://leetcode.com/problems/4sum
******************************/

#include <algorithm>
#include <concepts>
#include <iterator>
#include <vector>
#include <gtest/gtest.h>

using namespace std;

//==============================================================================

/* for N-sum problem:
 * O(n^(N-1) + n * log(n)) time; O(1) space
 *
 * 4-sum is just 3-sum with an additional loop.
 * For 2-sum we can solve in linear time using 2 pointers and sort.
 * All N-sum problem can be solved with N-1-sum with an additional loop.
 * So here the code is generalized for any N-sum problem using a template.
 */
class Solution {
  template<int N, typename... Args>
    requires(N > 1 && (same_as<Args, int> && ...))
  static constexpr void n_sum(vector<vector<int>>& ans, auto begin, auto end, int target, Args... args) {
    if constexpr (N == 2) {
      auto left  = begin;
      auto right = prev(end);
      while (left < right) {
        int64_t sum = 0l + (args + ...) + *left + *right;
        if (sum < target) {
          left++;
        } else if (sum > target) {
          right--;
        } else {
          ans.push_back({args..., *left, *right});
          left++;
          right--;
          while (left < right && *prev(left) == *left) left++;
        }
      }
    } else {
      for (auto it = begin; it != end; it++) {
        if (it != begin && *prev(it) == *it) continue;
        n_sum<N - 1>(ans, next(it), end, target, args..., *it);
      }
    }
  }

public:
  vector<vector<int>> fourSum(vector<int> nums, int target) {
    auto ans = vector<vector<int>>{};
    if (nums.empty()) {
      return ans;
    }
    sort(nums.begin(), nums.end());

    n_sum<4>(ans, nums.begin(), nums.end(), target);

    return ans;
  }
};

//==============================================================================

TEST(Test, s0018_4sum) {
  // clang-format off
  auto check = [](vector<vector<int>> expected, vector<int> input, int target) {
    auto ans = vector<vector<int>>{expected};
    for (auto& a: ans) {
      sort(a.begin(), a.end());
    }
    sort(ans.begin(), ans.end());
    auto sol = Solution{}.fourSum(input, target);
    for (auto& s: sol) {
      sort(s.begin(), s.end());
    }
    sort(sol.begin(), sol.end());
    EXPECT_EQ(ans, sol);
  };
  check({{-2, -1, 1, 2}, {-2, 0, 0, 2}, {-1, 0, 0, 1}}, {1, 0, -1, 0, -2, 2}, 0);
  check({{2, 2, 2, 2}}, {2, 2, 2, 2, 2}, 8);
  check({}, {1000000000,1000000000,1000000000,1000000000}, -294967296);
  // clang-format on
}
