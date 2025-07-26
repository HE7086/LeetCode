/******************************
__DEBUG_FUNCTION<combinationSum>__
Question 39: Combination Sum
Difficulty: Med.

Given an array of distinct integers candidates and a target integer target, return a list of all unique combinations of candidates where the chosen numbers sum to target. You may return the combinations in any order.
The same number may be chosen from candidates an unlimited number of times. Two combinations are unique if the frequency of at least one of the chosen numbers is different.
The test cases are generated such that the number of unique combinations that sum up to target is less than 150 combinations for the given input.

Example 1:

Input: candidates = [2,3,6,7], target = 7
Output: [[2,2,3],[7]]
Explanation:
2 and 3 are candidates, and 2 + 2 + 3 = 7. Note that 2 can be used multiple times.
7 is a candidate, and 7 = 7.
These are the only two combinations.

Example 2:

Input: candidates = [2,3,5], target = 8
Output: [[2,2,2,2],[2,3,3],[3,5]]

Example 3:

Input: candidates = [2], target = 1
Output: []


Constraints:

  1 <= candidates.length <= 30
  2 <= candidates[i] <= 40
  All elements of candidates are distinct.
  1 <= target <= 40

URL: https://leetcode.com/problems/combination-sum
******************************/

#include <algorithm>
#include <vector>
#include <gtest/gtest.h>

using namespace std;

//==============================================================================

/* let n = candidates.size();
 *     t = target
 *
 * O(n^t * t) time; O(t) space
 * recursive backtracking.
 */
class Solution {
public:
  vector<vector<int>> combinationSum(vector<int> const& candidates, int target) {
    auto ans  = vector<vector<int>>{};
    auto temp = vector<int>{};

    [&](this auto self, int index, int sum) -> void {
      for (size_t i = index; i < candidates.size(); i++) {
        temp.push_back(candidates[i]);
        sum += candidates[i];

        if (sum == target) {
          ans.push_back(temp);
        } else if (sum < target) {
          self(i, sum);
        }

        temp.pop_back();
        sum -= candidates[i];
      }
    }(0, 0);

    return ans;
  }
};

//==============================================================================

TEST(Test, s0039_combination_sum) {
  auto check = [](vector<vector<int>> expected, vector<int> candidates, int target) {
    for (auto& v : expected) {
      sort(v.begin(), v.end());
    }
    sort(expected.begin(), expected.end());

    auto ans = Solution{}.combinationSum(candidates, target);
    for (auto& v : ans) {
      sort(v.begin(), v.end());
    }
    sort(ans.begin(), ans.end());

    EXPECT_EQ(expected, ans);
  };

  check({}, {2}, 1);
  check(
      {
          {2, 2, 3},
          {7}
  },
      {2, 3, 6, 7}, 7
  );
  check(
      {
          {2, 2, 2, 2},
          {2, 3, 3},
          {3, 5}
  },
      {2, 3, 5}, 8
  );
  check({{4, 4}, {4, 2, 2}, {2, 2, 2, 2}, {8}}, {4, 2, 8}, 8);
}
