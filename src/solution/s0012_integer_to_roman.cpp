/******************************
Question 12: Integer to Roman
Difficulty: Med.

Seven different symbols represent Roman numerals with the following values:

      Symbol Value
      I      1
      V      5
      X      10
      L      50
      C      100
      D      500
      M      1000

Roman numerals are formed by appending the conversions of decimal place values from highest to lowest. Converting a decimal place value into a Roman numeral has the following rules:

  If the value does not start with 4 or 9, select the symbol of the maximal value that can be subtracted from the input, append that symbol to the result, subtract its value, and convert the remainder to a Roman numeral.
  If the value starts with 4 or 9 use the subtractive form representing one symbol subtracted from the following symbol, for example, 4 is 1 (I) less than 5 (V): IV and 9 is 1 (I) less than 10 (X): IX. Only the following subtractive forms are used: 4 (IV), 9 (IX), 40 (XL), 90 (XC), 400 (CD) and 900 (CM).
  Only powers of 10 (I, X, C, M) can be appended consecutively at most 3 times to represent multiples of 10. You cannot append 5 (V), 50 (L), or 500 (D) multiple times. If you need to append a symbol 4 times use the subtractive form.

Given an integer, convert it to a Roman numeral.

Example 1:

Input: num = 3749
Output: "MMMDCCXLIX"
Explanation:

3000 = MMM as 1000 (M) + 1000 (M) + 1000 (M)
 700 = DCC as 500 (D) + 100 (C) + 100 (C)
  40 = XL as 10 (X) less of 50 (L)
   9 = IX as 1 (I) less of 10 (X)
Note: 49 is not 1 (I) less of 50 (L) because the conversion is based on decimal places

Example 2:

Input: num = 58
Output: "LVIII"
Explanation:

50 = L
 8 = VIII

Example 3:

Input: num = 1994
Output: "MCMXCIV"
Explanation:

1000 = M
 900 = CM
  90 = XC
   4 = IV


Constraints:

  1 <= num <= 3999

URL: https://leetcode.com/problems/integer-to-roman
******************************/

#include <array>
#include <string>
#include <gtest/gtest.h>

using namespace std;

//==============================================================================

/* O(n) time; O(1) space
 * Trivial lookup table.
 */
/*
 * class Solution {
 *   static constexpr auto const symbols = array<pair<string_view, int>, 13>{
 *       // clang-format off
 *       {{"M", 1000},
 *        {"CM", 900},
 *        {"D",  500},
 *        {"CD", 400},
 *        {"C",  100},
 *        {"XC",  90},
 *        {"L",   50},
 *        {"XL",  40},
 *        {"X",   10},
 *        {"IX",   9},
 *        {"V",    5},
 *        {"IV",   4},
 *        {"I",    1}}
 *       // clang-format on
 *   };
 *
 * public:
 *   string intToRoman(int num) {
 *     [[assume(0 < num && num < 4000)]];
 *
 *     auto ans = string{};
 *     for (auto [symbol, value] : symbols) {
 *       if (num >= value) {
 *         size_t quot = num / value;
 *         ans.reserve(symbol.size() * quot);
 *         for (size_t i = 0; i < quot; i++) {
 *           ans.append(symbol);
 *         }
 *         num %= value;
 *       }
 *     }
 *     return ans;
 *   }
 * };
 */

/* O(1) time; O(1) space
 * Optimized lookup table for 0 < nums < 4000.
 * Use slightly more memory than the previous one.
 */
class Solution {
  static constexpr auto const one = array<char const*, 10>{
      {"", "I", "II", "III", "IV", "V", "VI", "VII", "VIII", "IX"}
  };
  static constexpr auto const ten = array<char const*, 10>{
      {"", "X", "XX", "XXX", "XL", "L", "LX", "LXX", "LXXX", "XC"}
  };
  static constexpr auto const hunderd = array<char const*, 10>{
      {"", "C", "CC", "CCC", "CD", "D", "DC", "DCC", "DCCC", "CM"}
  };
  static constexpr auto const thousand = array<char const*, 4>{
      {"", "M", "MM", "MMM"}
  };

public:
  string intToRoman(int num) {
    [[assume(0 < num && num < 4000)]];
    auto ans = string{};
    ans.append(thousand[num / 1000]);
    ans.append(hunderd[(num % 1000) / 100]);
    ans.append(ten[(num % 100) / 10]);
    ans.append(one[num % 10]);

    // assert(ans.capacity() == 15);
    // the longest possible answer is "MMMDCCCLXXXVIII" (3888)
    // which is exactly 15 chars long
    // short string optimization always applies (on 64-bit target)
    // -> reserve is not necessary here.
    return ans;
  }
};

//==============================================================================

TEST(Test, s0012_integer_to_roman) {
  auto s = Solution{};
  EXPECT_EQ("MMMDCCXLIX", s.intToRoman(3749));
  EXPECT_EQ("LVIII", s.intToRoman(58));
  EXPECT_EQ("MCMXCIV", s.intToRoman(1994));
  EXPECT_EQ("MMMDCCCLXXXVIII", s.intToRoman(3888));
}
