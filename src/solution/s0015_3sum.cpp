/******************************
__DEBUG_FUNCTION<threeSum>__
Question 15: 3Sum
Difficulty: Med.

Given an integer array nums, return all the triplets [nums[i], nums[j], nums[k]] such that i != j, i != k, and j != k, and nums[i] + nums[j] + nums[k] == 0.
Notice that the solution set must not contain duplicate triplets.

Example 1:

Input: nums = [-1,0,1,2,-1,-4]
Output: [[-1,-1,2],[-1,0,1]]
Explanation:
nums[0] + nums[1] + nums[2] = (-1) + 0 + 1 = 0.
nums[1] + nums[2] + nums[4] = 0 + 1 + (-1) = 0.
nums[0] + nums[3] + nums[4] = (-1) + 2 + (-1) = 0.
The distinct triplets are [-1,0,1] and [-1,-1,2].
Notice that the order of the output and the order of the triplets does not matter.

Example 2:

Input: nums = [0,1,1]
Output: []
Explanation: The only possible triplet does not sum up to 0.

Example 3:

Input: nums = [0,0,0]
Output: [[0,0,0]]
Explanation: The only possible triplet sums up to 0.


Constraints:

  3 <= nums.length <= 3000
  -10^5 <= nums[i] <= 10^5

URL: https://leetcode.com/problems/3sum
******************************/

#include <algorithm>
#include <vector>
#include <gtest/gtest.h>

using namespace std;

//==============================================================================

/* O(n^3) time; O(n) space
 * Basic solution:
 * just straight forward iterate through all possible triplets, and ensure uniqueness with a set.
 */
/* class Solution {
 * public:
 *   vector<vector<int>> threeSum(vector<int> const& nums) {
 *     auto ans = set<vector<int>>{};
 *     for (size_t i = 0; i < nums.size(); i++) {
 *       for (size_t j = i + 1; j < nums.size(); j++) {
 *         for (size_t k = j + 1; k < nums.size(); k++) {
 *           if (nums[i] + nums[j] + nums[k] == 0) {
 *             auto a = vector<int>{nums[i], nums[j], nums[k]};
 *             sort(a.begin(), a.end());
 *             ans.insert(a);
 *           }
 *         }
 *       }
 *     }
 *
 *     auto result = vector<vector<int>>{};
 *     result.reserve(ans.size());
 *     move(ans.begin(), ans.end(), back_inserter(result));
 *     return result;
 *   }
 * };
 */

/* O(n^2 + n * log(n)) time; O(1) space (depending on sort algorithm)
 * Double pointer with one additional fixed pointer (base).
 * We iterate through all base pointers, and do a double pointer search in the rest of the elements.
 * We need a strategy to determine which pointer to move -> input must have some sort of order.
 * Since input is just vector<int> -> sort the input.
 * When the triplet sum is negative, we move the left pointer inwards -> the sum becomes larger, and vice versa.
 * To eliminate duplicates, each time we find a valid triplet, the pointer is moved until it meets a new value.
 * After each iteration the base pointer is also moved to a new value.
 * Because in the previous iteration cases like {0, 0, 0} is already checked, it's safe to move on.
 */
class Solution {
public:
  vector<vector<int>> threeSum(vector<int> nums) {
    sort(nums.begin(), nums.end());
    auto ans = vector<vector<int>>{};
    for (auto base = nums.begin(); base != nums.end(); base++) {
      if (base != nums.begin() && *prev(base) == *base) continue;

      auto left  = next(base);
      auto right = prev(nums.end());
      while (left < right) {
        int sum = *base + *left + *right;

        if (sum < 0) {
          left++;
        } else if (sum > 0) {
          right--;

        } else {
          ans.push_back({*base, *left, *right});
          left++;
          right--;
          // skip duplicates
          while (left < right && *prev(left) == *left) left++;
        }
      }

      // while (next(base) != nums.end() && *next(base) == *base) base++;
      // move the pointer at the start of the loop to save some time (?)
    }

    return ans;
  }
};

//==============================================================================

TEST(Test, s0015_3sum) {
  // clang-format off
  auto check = [](vector<vector<int>> expected, vector<int> input) {
    auto ans = vector<vector<int>>{expected};
    for (auto& a: ans) {
      sort(a.begin(), a.end());
    }
    sort(ans.begin(), ans.end());
    auto sol = Solution{}.threeSum(input);
    for (auto& s: sol) {
      sort(s.begin(), s.end());
    }
    sort(sol.begin(), sol.end());
    EXPECT_EQ(ans, sol);
  };
  check({}, {0, 1, 1});
  check({{0, 0, 0}}, {0, 0, 0});
  check({{-1, 0, 1}, {0, 0, 0}}, {-1, 0, 0, 0, 1, 2, 3});
  check({{-1, -1, 2}, {-1, 0, 1}}, {-1, 0, 1, 2, -1, -4});
  check({{-1, 0, 1}}, {1, -1, -1, 0});
  check({
          {-10, 5,  5},
          {-5,  0,  5},
          {-4,  2,  2},
          {-3,  -2, 5},
          {-3,  1,  2},
          {-2,  0,  2}
  },
      {2, -3, 0, -2, -5, -5, -4, 1, 2, -2, 2, 0, 2, -4, 5, 5, -10}
  );
  // clang-format on
}
