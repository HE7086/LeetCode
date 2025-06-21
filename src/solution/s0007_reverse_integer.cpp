/******************************
__DEBUG_FUNCTION<reverse>__
Question 7: Reverse Integer
Difficulty: Med.

Given a signed 32-bit integer x, return x with its digits reversed. If reversing x causes the value to go outside the signed 32-bit integer range [-2^31, 2^31 - 1], then return 0.
Assume the environment does not allow you to store 64-bit integers (signed or unsigned).

Example 1:

Input: x = 123
Output: 321

Example 2:

Input: x = -123
Output: -321

Example 3:

Input: x = 120
Output: 21


Constraints:

  -2^31 <= x <= 2^31 - 1

URL: https://leetcode.com/problems/reverse-integer
******************************/

#include <limits>
#include <gtest/gtest.h>

using namespace std;

//==============================================================================

/* O(log(n)) time; O(1) space
 * Very trivial question. One only needs to pay attention to the bondries.
 * We can either determine whether an overflow would happen by comparing the input beforehand,
 * or do the check in every iteration.
 *
 * There are 4 edge cases we need to consider:
 *   - the reversed integer is larger than 214748364 -> when multiplied by 10 it would overflow
 *   - the reversed integer is exactly 214748364 -> overflow when the remainder to be added are larger than 7
 *   - and the negative equivalents.
 *
 * Note: for pure performance it seems string manipulation is faster.
 * You need to hook the program to directly read inputs from stdin (which is pretty cheaty).
 */

class Solution {
public:
  int reverse(int x) {
    static constexpr int const max10 = numeric_limits<int>::max() / 10;
    static constexpr int const min10 = numeric_limits<int>::min() / 10;

    int ans = 0;
    while (x != 0) {
      int y = x % 10;
      x /= 10;

      if (ans > max10 ||
          ans < min10 || // ans would overflow in next step
          (ans == max10 && y > 7) ||
          (ans == min10 && y < -8) // 2^32 = 2147483648
      ) {
        return 0;
      }
      ans = 10 * ans + y;
    }
    return ans;
  }
};

//==============================================================================

TEST(Test, s0007_reverse_integer) {
  auto s = Solution{};
  EXPECT_EQ(321, s.reverse(123));
  EXPECT_EQ(-321, s.reverse(-123));
  EXPECT_EQ(21, s.reverse(120));
  EXPECT_EQ(987654321, s.reverse(1234567890));
  EXPECT_EQ(0, s.reverse(2147483647));
  EXPECT_EQ(0, s.reverse(1534236469));
}
