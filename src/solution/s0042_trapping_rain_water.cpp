/******************************
__DEBUG_FUNCTION<trap>__
Question 42: Trapping Rain Water
Difficulty: Hard

Given n non-negative integers representing an elevation map where the width of each bar is 1, compute how much water it can trap after raining.

Example 1:

Input: height = [0,1,0,2,1,0,1,3,2,1,2,1]
Output: 6
Explanation: The above elevation map (black section) is represented by array [0,1,0,2,1,0,1,3,2,1,2,1]. In this case, 6 units of rain water (blue section) are being trapped.

Example 2:

Input: height = [4,2,0,3,2,5]
Output: 9


Constraints:

  n == height.length
  1 <= n <= 2 * 10^4
  0 <= height[i] <= 10^5

URL: https://leetcode.com/problems/trapping-rain-water
******************************/

#include <algorithm>
#include <vector>
#include <gtest/gtest.h>

using namespace std;

//==============================================================================

/* O(n) time; O(1) space
 * Two pointer strategy. Move the pointer that has lower height towards middle,
 * and calculate the value for each cell while moving.
 * Basically the same as 11 (container with most water)
 *
 * ans = min(left_max, right_max) - min(height[left], height[right])
 */
class Solution {
public:
  int trap(vector<int> const& height) {
    int left = 0;
    int right = height.size() - 1;
    int left_max = height[left];
    int right_max = height[right];
    int ans = 0;
    while (left < right) {
      left_max = std::max(left_max, height[left]);
      right_max = std::max(right_max, height[right]);

      if (height[left] < height[right]) {
        ans += std::min(left_max, right_max) - height[left];
        left++;
      } else {
        ans += std::min(left_max, right_max) - height[right];
        right--;
      }
    }
    return ans;
  }
};

//==============================================================================

TEST(Test, s0042_trapping_rain_water) {
  auto s = Solution{};
  EXPECT_EQ(6, s.trap({0, 1, 0, 2, 1, 0, 1, 3, 2, 1, 2, 1}));
  EXPECT_EQ(9, s.trap({4, 2, 0, 3, 2, 5}));
}

