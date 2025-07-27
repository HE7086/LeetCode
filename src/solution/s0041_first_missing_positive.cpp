/******************************
__DEBUG_FUNCTION<firstMissingPositive>__
Question 41: First Missing Positive
Difficulty: Hard

Given an unsorted integer array nums. Return the smallest positive integer that is not present in nums.
You must implement an algorithm that runs in O(n) time and uses O(1) auxiliary space.

Example 1:

Input: nums = [1,2,0]
Output: 3
Explanation: The numbers in the range [1,2] are all in the array.

Example 2:

Input: nums = [3,4,-1,1]
Output: 2
Explanation: 1 is in the array but 2 is missing.

Example 3:

Input: nums = [7,8,9,11,12]
Output: 1
Explanation: The smallest positive integer 1 is missing.


Constraints:

  1 <= nums.length <= 10^5
  -2^31 <= nums[i] <= 2^31 - 1

URL: https://leetcode.com/problems/first-missing-positive
******************************/

#include <utility>
#include <vector>
#include <gtest/gtest.h>

using namespace std;

//==============================================================================

/* O(n) time; O(1) space
 *
 * nums contains at most N positive values 1 ... N
 * We only need to put all values in range into its correct position,
 * and check for first incorrect position.
 *
 * cycle sort: nums[i] = i + 1
 */
class Solution {
public:
  int firstMissingPositive(vector<int> nums) {
    int n = static_cast<int>(nums.size());
    for (int i = 0; i < n;) {
      if (
          nums[i] <= 0 ||
          nums[i] > n                     // we can safely ignore any number not in [1, n]
          || nums[i] == nums[nums[i] - 1] // skip duplicates and values already in correct position
      ) {
        i++;
        continue;
      }
      swap(nums[i], nums[nums[i] - 1]);
    }

    // find the first value that is not in position
    for (int i = 0; i < n; i++) {
      if (nums[i] != i + 1) {
        return i + 1;
      }
    }
    return n + 1;
  }
};

//==============================================================================

TEST(Test, s0041_first_missing_positive) {
  auto s = Solution{};
  EXPECT_EQ(3, s.firstMissingPositive({1, 2, 0}));
  EXPECT_EQ(2, s.firstMissingPositive({3, 4, -1, 1}));
  EXPECT_EQ(1, s.firstMissingPositive({7, 8, 9, 11, 12}));
  EXPECT_EQ(2, s.firstMissingPositive({1, 1}));
}

