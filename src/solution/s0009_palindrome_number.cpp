/******************************
__DEBUG_FUNCTION<isPalindrome>__
Question 9: Palindrome Number
Difficulty: Easy

Given an integer x, return true if x is a palindrome, and false otherwise.

Example 1:

Input: x = 121
Output: true
Explanation: 121 reads as 121 from left to right and from right to left.

Example 2:

Input: x = -121
Output: false
Explanation: From left to right, it reads -121. From right to left, it becomes 121-. Therefore it is not a palindrome.

Example 3:

Input: x = 10
Output: false
Explanation: Reads 01 from right to left. Therefore it is not a palindrome.


Constraints:

  -2^31<= x <= 2^31- 1


Follow up: Could you solve it without converting the integer to a string?
URL: https://leetcode.com/problems/palindrome-number
******************************/

#include <gtest/gtest.h>

using namespace std;

//==============================================================================

/* O(log(n)) time; O(1) space
 * Simply reuse the reverse function from s0007.
 * Nothing worth noting.
 */
static inline int reverse(int x) {
  static constexpr int const max10 = numeric_limits<int>::max() / 10;

  [[assume(x >= 0)]];
  int ans = 0;
  while (x != 0) {
    int y = x % 10;
    x /= 10;

    if (ans > max10 || (ans == max10 && y > 7)) {
      // anything that could overflow is for sure not palindrome
      // biggest palindrome: 2147447412
      return 0;
    }
    ans = 10 * ans + y;
  }
  return ans;
}

class Solution {
public:
  bool isPalindrome(int x) {
    if (x < 0) {
      return false;
    }
    return x == reverse(x);
  }
};

//==============================================================================

TEST(Test, s0009_palindrome_number) {
  auto s = Solution{};
  EXPECT_EQ(true, s.isPalindrome(0));
  EXPECT_EQ(true, s.isPalindrome(121));
  EXPECT_EQ(false, s.isPalindrome(-121));
  EXPECT_EQ(false, s.isPalindrome(10));
  EXPECT_EQ(true, s.isPalindrome(2147447412));
  EXPECT_EQ(false, s.isPalindrome(2147483647));
}
