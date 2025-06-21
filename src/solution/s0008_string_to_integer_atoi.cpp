/******************************
__DEBUG_FUNCTION<myAtoi>__
Question 8: String to Integer (atoi)
Difficulty: Med.

Implement the myAtoi(string s) function, which converts a string to a 32-bit signed integer.
The algorithm for myAtoi(string s) is as follows:

  Whitespace: Ignore any leading whitespace (" ").
  Signedness: Determine the sign by checking if the next character is '-' or '+', assuming positivity if neither present.
  Conversion: Read the integer by skipping leading zerosuntil a non-digit character is encountered or the end of the string is reached. If no digits were read, then the result is 0.
  Rounding: If the integer is out of the 32-bit signed integer range [-2^31, 2^31 - 1], then round the integer to remain in the range. Specifically, integers less than -2^31 should be rounded to -2^31, and integers greater than 2^31 - 1 should be rounded to 2^31 - 1.

Return the integer as the final result.

Example 1:

Input: s = "42"
Output: 42
Explanation:

The underlined characters are what is read in and the caret is the current reader position.
Step 1: "42" (no characters read because there is no leading whitespace)
         ^
Step 2: "42" (no characters read because there is neither a '-' nor '+')
         ^
Step 3: "42" ("42" is read in)
           ^

Example 2:

Input: s = " -042"
Output: -42
Explanation:

Step 1: "   -042" (leading whitespace is read and ignored)
            ^
Step 2: "   -042" ('-' is read, so the result should be negative)
             ^
Step 3: "   -042" ("042" is read in, leading zeros ignored in the result)
               ^

Example 3:

Input: s = "1337c0d3"
Output: 1337
Explanation:

Step 1: "1337c0d3" (no characters read because there is no leading whitespace)
         ^
Step 2: "1337c0d3" (no characters read because there is neither a '-' nor '+')
         ^
Step 3: "1337c0d3" ("1337" is read in; reading stops because the next character is a non-digit)
             ^

Example 4:

Input: s = "0-1"
Output: 0
Explanation:

Step 1: "0-1" (no characters read because there is no leading whitespace)
         ^
Step 2: "0-1" (no characters read because there is neither a '-' nor '+')
         ^
Step 3: "0-1" ("0" is read in; reading stops because the next character is a non-digit)
          ^

Example 5:

Input: s = "words and 987"
Output: 0
Explanation:
Reading stops at the first non-digit character 'w'.


Constraints:

  0 <= s.length <= 200
  s consists of English letters (lower-case and upper-case), digits (0-9), ' ', '+', '-', and '.'.

URL: https://leetcode.com/problems/string-to-integer-atoi
******************************/

#include <limits>
#include <string>
#include <gtest/gtest.h>

using namespace std;

//==============================================================================

/* O(n) time; O(1) space
 * Simple state machine that does the job.
 * I'm sure there are simpler solution out there, but this is imo more "maintainable".
 */
class Solution {
  enum struct state {
    whitespace,
    signedness,
    conversion,
    other,
  };

public:
  int myAtoi(string const& s) {
    static constexpr int const max10 = numeric_limits<int>::max() / 10;
    static constexpr int const min10 = numeric_limits<int>::min() / 10;

    auto state = state::whitespace;
    auto sign  = -1; // -1 means positive
    auto ans   = 0;

    for (auto it = s.begin(); it != s.end(); it++) {
      switch (state) {
        case state::whitespace: {
          if (*it == ' ') {
            break;
          }
          state = state::signedness;
          [[fallthrough]];
        }
        case state::signedness: {
          if (*it == '+') {
            sign  = -1;
            state = state::conversion;
            break;
          } else if (*it == '-') {
            sign  = 1;
            state = state::conversion;
            break;
          }
          state = state::conversion;
          [[fallthrough]];
        }
        case state::conversion: {
          if ('0' <= *it && *it <= '9') {
            if (ans > max10 || (ans == max10 && *it > '7')) {
              if (sign == -1) {
                // positive
                return numeric_limits<int>::max();
              } else {
                // negative
                // ans is still positive at this point, so 8 would also overflow
                // it = 8 -> exactly min
                // it = 9 -> underflow
                return numeric_limits<int>::min();
              }
            }
            ans = 10 * ans + static_cast<int>(*it - '0');
            break;
          }
          state = state::other;
          [[fallthrough]];
        }
        case state::other: break;
      }
      if (state == state::other) {
        break;
      }
    }

    return ans * (-1 * sign);
  }
};

//==============================================================================

TEST(Test, s0008_string_to_integer_atoi) {
  auto s = Solution{};
  EXPECT_EQ(42, s.myAtoi("42"));
  EXPECT_EQ(-42, s.myAtoi("-042"));
  EXPECT_EQ(1337, s.myAtoi("1337c0d3"));
  EXPECT_EQ(0, s.myAtoi("0-1"));
  EXPECT_EQ(0, s.myAtoi("words and 987"));
  EXPECT_EQ(0, s.myAtoi("+-12"));
  EXPECT_EQ(-2147483648, s.myAtoi("-91283472332"));
  EXPECT_EQ(-2147483648, s.myAtoi("-2147483648"));
}
