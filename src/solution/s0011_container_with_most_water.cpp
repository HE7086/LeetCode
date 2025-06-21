/******************************
__DEBUG_FUNCTION<maxArea>__
Question 11: Container With Most Water
Difficulty: Med.

You are given an integer array height of length n. There are n vertical lines drawn such that the two endpoints of the i^th line are (i, 0) and (i, height[i]).
Find two lines that together with the x-axis form a container, such that the container contains the most water.
Return the maximum amount of water a container can store.
Notice that you may not slant the container.

Example 1:

Input: height = [1,8,6,2,5,4,8,3,7]
Output: 49
Explanation: The above vertical lines are represented by array [1,8,6,2,5,4,8,3,7]. In this case, the max area of water (blue section) the container can contain is 49.

Example 2:

Input: height = [1,1]
Output: 1


Constraints:

  n == height.length
  2 <= n <= 10^5
  0 <= height[i] <= 10^4

URL: https://leetcode.com/problems/container-with-most-water
******************************/

#include <iterator>
#include <vector>
#include <gtest/gtest.h>

using namespace std;

//==============================================================================

/* O(n) time; O(1) space
 * Using 2 pointers for left and right border.
 * Since we are looking for maximum, only move the iterator with smaller value inwards.
 * This way we only need to traverse the vector once.
 */
class Solution {
public:
  int maxArea(vector<int> const& height) {
    int ans = 0;
    auto left = height.begin();
    auto right = prev(height.end());
    while (left < right) {
      ans = max(ans, static_cast<int>(right - left) * min(*left, *right));
      if (*left < *right) {
        left++;
      } else {
        right--;
      }
    }
    return ans;
  }
};

//==============================================================================

TEST(Test, s0011_container_with_most_water) {
  auto s = Solution{};
  EXPECT_EQ(1, s.maxArea({1, 1}));
  EXPECT_EQ(2, s.maxArea({1, 2, 1}));
  EXPECT_EQ(4, s.maxArea({2, 1, 2}));
  EXPECT_EQ(49, s.maxArea({1, 8, 6, 2, 5, 4, 8, 3, 7}));
}

