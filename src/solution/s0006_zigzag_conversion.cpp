/******************************
Question 6: Zigzag Conversion

The string "PAYPALISHIRING" is written in a zigzag pattern on a given number of rows like this: (you may want to display this pattern in a fixed font for better legibility)

P   A   H   N
A P L S I I G
Y   I   R

And then read line by line: "PAHNAPLSIIGYIR"
Write the code that will take a string and make this conversion given a number of rows:

string convert(string s, int numRows);


Example 1:

Input: s = "PAYPALISHIRING", numRows = 3
Output: "PAHNAPLSIIGYIR"

Example 2:

Input: s = "PAYPALISHIRING", numRows = 4
Output: "PINALSIGYAHRPI"
Explanation:
P     I    N
A   L S  I G
Y A   H R
P     I

Example 3:

Input: s = "A", numRows = 1
Output: "A"


Constraints:

    1 <= s.length <= 1000
    s consists of English letters (lower-case and upper-case), ',' and '.'.
    1 <= numRows <= 1000

URL: https://leetcode.com/problems/zigzag-conversion
******************************/

#include <string>
#include <gtest/gtest.h>

using namespace std;

//==============================================================================

/* An example of bad question with bad description.
 *
 * Zigzag pattern for numRows = 5:
 *  0         8          16
 *  1      7  9       15 17
 *  2    6   10    14    18
 *  3  5     11 13       19
 *  4        12          20 ...
 *
 * Result after conversion:
 *  s[0], s[8], s[16], ..., s[1], s[7], s[9], ...
 *
 * Pattern for the index in s[index]:
 *   - first and last row:
 *     index = row + n * (2 * numRows - 2)
 *   - middle rows:
 *     - column:
 *       index = row + n * (2 * numRows - 2)
 *     - numbers on the oblique line between columns:
 *       index = column + (2 * numRows - 2) - 2 * i
 *
 * We only need to generate all the indexes and insert corresponding char into the result.
 * Alternatively we can also first generate a matrix of indexes and sort the input inplace.
 * But sizeof(char) < sizeof(int) -> we need less memory by inserting.
 *
 * O(n) time; O(n) space
 */

class Solution {
public:
  string convert(string const& s, int numRows) {
    int const interval = 2 * numRows - 2;
    auto ans = string{};
    ans.reserve(s.size());

    if (numRows == 1) {
      // allow nrvo
      ans = s;
    } else {
      for (int i = 0; i < numRows; i++) {
        for (int j = 0; i + j < static_cast<int>(s.size());) {
          ans += s[i + j];

          if (j % interval == 0) {
            // oblique line
            if (interval == 2 * i) {
              // last row
              j += interval;
            } else {
              // middle rows
              j += interval - 2 * i;
            }
          } else {
            // next column
            j += 2 * i;
          }
        }
      }
    }
    return ans;
  }
};

//==============================================================================

TEST(Test, s0006_zigzag_conversion) {
  auto s = Solution{};
  EXPECT_EQ("A", s.convert("A", 1));
  EXPECT_EQ("AB", s.convert("AB", 1));
  EXPECT_EQ("ACB", s.convert("ABC", 2));
  EXPECT_EQ("ACBD", s.convert("ABCD", 2));
  EXPECT_EQ("AEBDFCG", s.convert("ABCDEFG", 3));
  EXPECT_EQ("AEBDFC", s.convert("ABCDEF", 3));
  EXPECT_EQ("AEBDC", s.convert("ABCDE", 3));
  EXPECT_EQ("ABDC", s.convert("ABCD", 3));
  EXPECT_EQ("ABC", s.convert("ABC", 3));
  EXPECT_EQ("AB", s.convert("AB", 3));
  EXPECT_EQ("A", s.convert("A", 3));
  EXPECT_EQ("PAHNAPLSIIGYIR", s.convert("PAYPALISHIRING", 3));
  EXPECT_EQ("PINALSIGYAHRPI", s.convert("PAYPALISHIRING", 4));
}

