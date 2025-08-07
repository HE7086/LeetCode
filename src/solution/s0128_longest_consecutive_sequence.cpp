/******************************
__DEBUG_FUNCTION<longestConsecutive>__
Question 128: Longest Consecutive Sequence
Difficulty: Med.

Given an unsorted array of integers nums, return the length of the longest consecutive elements sequence.
You must write an algorithm that runs in O(n) time.

Example 1:

Input: nums = [100,4,200,1,3,2]
Output: 4
Explanation: The longest consecutive elements sequence is [1, 2, 3, 4]. Therefore its length is 4.

Example 2:

Input: nums = [0,3,7,2,5,8,4,6,0,1]
Output: 9

Example 3:

Input: nums = [1,0,1,2]
Output: 3


Constraints:

  0 <= nums.length <= 10^5
  -10^9 <= nums[i] <= 10^9

URL: https://leetcode.com/problems/longest-consecutive-sequence
******************************/

#include <algorithm>
#include <vector>
#include <gtest/gtest.h>

using namespace std;

//==============================================================================

/* O(n log(n)) time; O(1) space
 */
class Solution {
public:
  int longestConsecutive(vector<int> nums) {
    if (nums.empty()) {
      return 0;
    }
    sort(nums.begin(), nums.end());
    int ans = 1;
    int current = 1;

    for (size_t i = 1; i < nums.size(); i++) {
      if (nums[i - 1] == nums[i]) {
        continue;
      }
      if (nums[i - 1] == nums[i] - 1) {
        current++;
        ans = max(ans, current);
      } else {
        current = 1;
      }
    }
    return ans;
  }
};

//==============================================================================

TEST(Test, s0128_longest_consecutive_sequence) {
  auto s = Solution{};
  EXPECT_EQ(4, s.longestConsecutive({100, 4, 200, 1, 3, 2}));
  EXPECT_EQ(9, s.longestConsecutive({0, 3, 7, 2, 5, 8, 4, 6, 0, 1}));
  EXPECT_EQ(3, s.longestConsecutive({1, 0, 1, 2}));
}

