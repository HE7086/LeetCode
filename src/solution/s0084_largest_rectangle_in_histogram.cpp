/******************************
__DEBUG_FUNCTION<largestRectangleArea>__
Question 84: Largest Rectangle in Histogram
Difficulty: Hard

Given an array of integers heights representing the histogram's bar height where the width of each bar is 1, return the area of the largest rectangle in the histogram.

Example 1:

Input: heights = [2,1,5,6,2,3]
Output: 10
Explanation: The above is a histogram where width of each bar is 1.
The largest rectangle is shown in the red area, which has an area = 10 units.

Example 2:

Input: heights = [2,4]
Output: 4


Constraints:

  1 <= heights.length <= 10^5
  0 <= heights[i] <= 10^4

URL: https://leetcode.com/problems/largest-rectangle-in-histogram
******************************/

#include <algorithm>
#include <ranges>
#include <stack>
#include <vector>
#include <gtest/gtest.h>

using namespace std;

//==============================================================================

/* O(n) time; O(n) space
 * monotonic stack: ascending height order
 * pop the stack until the top is shorter than current.
 */
class Solution {
public:
  int largestRectangleArea(vector<int> const& heights) {
#if __cpp_lib_ranges_concat >= 202403L
    auto val = views::concat(views::single(0), views::concat(heights, views::single(0)));
#else
    auto val    = vector<int>(nums.size() + 2);
    val.front() = 0;
    val.back()  = 0;
    copy(nums.begin(), nums.end(), next(val.begin()));
#endif
    int ans = 0;

    stack<int> indices;
    indices.push(0);

    for (size_t i = 1; i < val.size(); ++i) {
      while (val[i] < val[indices.top()]) {
        int height = val[indices.top()];
        indices.pop();
        ans = std::max(ans, height * (static_cast<int>(i) - indices.top() - 1));
      }
      indices.push(i);
    }

    return ans;
  }
};

//==============================================================================

TEST(Test, s0084_largest_rectangle_in_histogram) {
  EXPECT_EQ(4, Solution{}.largestRectangleArea({2, 4}));
  EXPECT_EQ(10, Solution{}.largestRectangleArea({2, 1, 5, 6, 2, 3}));
}
