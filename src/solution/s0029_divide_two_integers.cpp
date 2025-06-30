/******************************
__DEBUG_FUNCTION<divide>__
Question 29: Divide Two Integers
Difficulty: Med.

Given two integers dividend and divisor, divide two integers without using multiplication, division, and mod operator.
The integer division should truncate toward zero, which means losing its fractional part. For example, 8.345 would be truncated to 8, and -2.7335 would be truncated to -2.
Return the quotient after dividing dividend by divisor.
Note: Assume we are dealing with an environment that could only store integers within the 32-bit signed integer range: [-2^31, 2^31 - 1]. For this problem, if the quotient is strictly greater than 2^31 - 1, then return 2^31 - 1, and if the quotient is strictly less than -2^31, then return -2^31.

Example 1:

Input: dividend = 10, divisor = 3
Output: 3
Explanation: 10/3 = 3.33333.. which is truncated to 3.

Example 2:

Input: dividend = 7, divisor = -3
Output: -2
Explanation: 7/-3 = -2.33333.. which is truncated to -2.


Constraints:

  -2^31 <= dividend, divisor <= 2^31 - 1
  divisor != 0

URL: https://leetcode.com/problems/divide-two-integers
******************************/

#include <cstdint>
#include <cstdlib>
#include <limits>
#include <gtest/gtest.h>

using namespace std;

//==============================================================================

/*
 * O(log n) time; O(1) space
 * For 32-bit integer the worst case only requires 32 steps.
 * Basically just binary division.
 */
class Solution {
public:
  int divide(int dividend, int divisor) {
    uint32_t a;
    uint32_t b;

    if (dividend == numeric_limits<int>::min() && divisor == -1) [[unlikely]] {
      return numeric_limits<int>::max();
    }
    if (dividend == numeric_limits<int>::min()) {
      a = 1u + numeric_limits<int>::max();
    } else {
      a = abs(dividend);
    }
    if (divisor == numeric_limits<int>::min()) {
      b = 1u + numeric_limits<int>::max();
    } else {
      b = abs(divisor);
    }

    uint32_t ans = 0;
    for (int32_t x = 31; x >= 0; x--) {
      if (a >> x >= b) {
        ans += 1 << x;
        a -= b << x;
      }
    }
    bool positive = (dividend > 0) == (divisor > 0);
    return (2 * positive - 1) * ans;
  }
};

//==============================================================================

TEST(Test, s0029_divide_two_integers) {
  auto s = Solution{};
  EXPECT_EQ(3, s.divide(10, 3));
  EXPECT_EQ(-2, s.divide(7, -3));
  EXPECT_EQ(-2147483648, s.divide(-2147483648, 1));
  EXPECT_EQ(2147483647, s.divide(-2147483648, -1));
  EXPECT_EQ(2147483647, s.divide(2147483647, 1));
  EXPECT_EQ(-2147483647, s.divide(2147483647, -1));
}

