/******************************
Question 13: Roman to Integer
Difficulty: Easy

Roman numerals are represented by seven different symbols: I, V, X, L, C, D and M.

Symbol       Value
I             1
V             5
X             10
L             50
C             100
D             500
M             1000
For example, 2 is written as II in Roman numeral, just two ones added together. 12 is written as XII, which is simply X + II. The number 27 is written as XXVII, which is XX + V + II.
Roman numerals are usually written largest to smallest from left to right. However, the numeral for four is not IIII. Instead, the number four is written as IV. Because the one is before the five we subtract it making four. The same principle applies to the number nine, which is written as IX. There are six instances where subtraction is used:

  I can be placed before V (5) and X (10) to make 4 and 9.
  X can be placed before L (50) and C (100) to make 40 and 90.
  C can be placed before D (500) and M (1000) to make 400 and 900.

Given a roman numeral, convert it to an integer.

Example 1:

Input: s = "III"
Output: 3
Explanation: III = 3.

Example 2:

Input: s = "LVIII"
Output: 58
Explanation: L = 50, V= 5, III = 3.

Example 3:

Input: s = "MCMXCIV"
Output: 1994
Explanation: M = 1000, CM = 900, XC = 90 and IV = 4.


Constraints:

  1 <= s.length <= 15
  s contains only the characters ('I', 'V', 'X', 'L', 'C', 'D', 'M').
  It is guaranteed that s is a valid roman numeral in the range [1, 3999].

URL: https://leetcode.com/problems/roman-to-integer
******************************/

#include <string>
#include <iterator>
#include <gtest/gtest.h>

using namespace std;

//==============================================================================

/* O(n) time; O(1) space
 * Trival conversion.
 *
 * For inputs in [1, 3999] a lookup table is also feasible.
 */
class Solution {
  static constexpr int roman_char_to_int(char c) {
    switch (c) {
      case 'M': return 1000;
      case 'D': return 500;
      case 'C': return 100;
      case 'L': return 50;
      case 'X': return 10;
      case 'V': return 5;
      case 'I': return 1;
      default: return 0;
    }
  }

public:
  int romanToInt(string const& s) {
    int result = 0;
    for (auto it = s.begin(); it != s.end(); it++) {
      int val = roman_char_to_int(*it);
      if (next(it) != s.end() && val < roman_char_to_int(*next(it))) {
        result -= val;
      } else {
        result += val;
      }
    }
    return result;
  }
};

//==============================================================================

TEST(Test, s0013_roman_to_integer) {
  auto s = Solution{};
  EXPECT_EQ(3, s.romanToInt("III"));
  EXPECT_EQ(58, s.romanToInt("LVIII"));
  EXPECT_EQ(1994, s.romanToInt("MCMXCIV"));
}

