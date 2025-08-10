/******************************
__DEBUG_FUNCTION<dailyTemperatures>__
Question 739: Daily Temperatures
Difficulty: Med.

Given an array of integers temperatures represents the daily temperatures, return an array answer such that answer[i] is the number of days you have to wait after the i^th day to get a warmer temperature. If there is no future day for which this is possible, keep answer[i] == 0 instead.

Example 1:
Input: temperatures = [73,74,75,71,69,72,76,73]
Output: [1,1,4,2,1,1,0,0]
Example 2:
Input: temperatures = [30,40,50,60]
Output: [1,1,1,0]
Example 3:
Input: temperatures = [30,60,90]
Output: [1,1,0]

Constraints:

  1 <= temperatures.length <= 10^5
  30 <= temperatures[i] <= 100

URL: https://leetcode.com/problems/daily-temperatures
******************************/

#include <stack>
#include <vector>
#include <gtest/gtest.h>

using namespace std;

//==============================================================================

class Solution {
public:
  vector<int> dailyTemperatures(vector<int> const& temperatures) {
    auto ans = vector<int>(temperatures.size(), 0);
    auto s = stack<int>{};

    for (size_t i = 0; i < temperatures.size(); i++) {
      while (!s.empty() && temperatures[s.top()] < temperatures[i]) {
        ans[s.top()] = i - s.top();
        s.pop();
      }
      s.push(i);
    }

    return ans;
  }
};

//==============================================================================

TEST(Test, s0739_daily_temperatures) {
  EXPECT_EQ((vector<int>{1, 1, 4, 2, 1, 1, 0, 0}),
      Solution{}.dailyTemperatures({73, 74, 75, 71, 69, 72, 76, 73}));
  EXPECT_EQ((vector<int>{1, 1, 1, 0}),
      Solution{}.dailyTemperatures({30, 40, 50, 60}));
  EXPECT_EQ((vector<int>{1, 1, 0}),
      Solution{}.dailyTemperatures({30, 60, 90}));
  EXPECT_EQ((vector<int>{2, 1, 1, 0}),
      Solution{}.dailyTemperatures({30, 30, 60, 90}));
}

