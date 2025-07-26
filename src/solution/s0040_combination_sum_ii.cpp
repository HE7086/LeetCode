/******************************
__DEBUG_FUNCTION<combinationSum2>__
Question 40: Combination Sum II
Difficulty: Med.

Given a collection of candidate numbers (candidates) and a target number (target), find all unique combinations in candidates where the candidate numbers sum to target.
Each number in candidates may only be used once in the combination.
Note: The solution set must not contain duplicate combinations.

Example 1:

Input: candidates = [10,1,2,7,6,1,5], target = 8
Output:
[
[1,1,6],
[1,2,5],
[1,7],
[2,6]
]

Example 2:

Input: candidates = [2,5,2,1,2], target = 5
Output:
[
[1,2,2],
[5]
]


Constraints:

  1 <= candidates.length <= 100
  1 <= candidates[i] <= 50
  1 <= target <= 30

URL: https://leetcode.com/problems/combination-sum-ii
******************************/

#include <algorithm>
#include <vector>
#include <gtest/gtest.h>

using namespace std;

//==============================================================================

/* O(2^n) time; O(n) space
 * recursive backtracking.
 */
class Solution {
public:
  vector<vector<int>> combinationSum2(vector<int>& candidates, int target) {
    sort(candidates.begin(), candidates.end());
    auto ans  = vector<vector<int>>{};
    auto temp = vector<int>{};

    [&](this auto self, size_t index, int sum) -> void {
      if (sum == target) {
        ans.push_back(temp);
      }
      if (sum >= target || index >= candidates.size()) {
        return;
      }
      temp.push_back(candidates[index]);
      self(index + 1, sum + temp.back());
      temp.pop_back();

      int c = candidates[index];
      while (index < candidates.size() && candidates[index] == c) {
        index++;
      }
      self(index, sum);
    }(0, 0);

    return ans;
  }
};

//==============================================================================

TEST(Test, s0040_combination_sum_ii) {
  auto check = [](vector<vector<int>> expected, vector<int> candidates, int target) {
    for (auto& v : expected) {
      sort(v.begin(), v.end());
    }
    sort(expected.begin(), expected.end());

    auto ans = Solution{}.combinationSum2(candidates, target);
    for (auto& v : ans) {
      sort(v.begin(), v.end());
    }
    sort(ans.begin(), ans.end());

    EXPECT_EQ(expected, ans);
  };

  check(
      {
          {1, 1, 6},
          {1, 2, 5},
          {1, 7},
          {2, 6}
  },
      {10, 1, 2, 7, 6, 1, 5}, 8
  );
  check(
      {
          {1, 2, 2},
          {5}
  },
      {2, 5, 2, 1, 2}, 5
  );
}

