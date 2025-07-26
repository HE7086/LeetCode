/******************************
__DEBUG_FUNCTION<countAndSay>__
Question 38: Count and Say
Difficulty: Med.

The count-and-say sequence is a sequence of digit strings defined by the recursive formula:

  countAndSay(1) = "1"
  countAndSay(n) is the run-length encoding of countAndSay(n - 1).

Run-length encoding (RLE) is a string compression method that works by replacing consecutive identical characters (repeated 2 or more times) with the concatenation of the character and the number marking the count of the characters (length of the run). For example, to compress the string "3322251" we replace "33" with "23", replace "222" with "32", replace "5" with "15" and replace "1" with "11". Thus the compressed string becomes "23321511".
Given a positive integer n, return the n^th element of the count-and-say sequence.

Example 1:

Input: n = 4
Output: "1211"
Explanation:

countAndSay(1) = "1"
countAndSay(2) = RLE of "1" = "11"
countAndSay(3) = RLE of "11" = "21"
countAndSay(4) = RLE of "21" = "1211"

Example 2:

Input: n = 1
Output: "1"
Explanation:
This is the base case.


Constraints:

  1 <= n <= 30


Follow up: Could you solve it iteratively?
URL: https://leetcode.com/problems/count-and-say
******************************/

#include <string>
#include <gtest/gtest.h>

using namespace std;

//==============================================================================

/* let m = length of the strings generated
 * O(m * n) time; O(m) space
 *
 * Straightforward solution.
 */
class Solution {
public:
  string cas(string input) {
    auto   ans     = string{};
    char   current = input[0];
    size_t len     = 0;
    for (char c : input) {
      if (c == current) {
        len++;
      } else {
        ans += to_string(len);
        ans += current;
        len     = 1;
        current = c;
      }
    }
    ans += to_string(len);
    ans += current;
    return ans;
  }
  string countAndSay(int n) {
    [[assume(n > 0)]];
    auto ans = string{"1"};

    for (int i = 1; i < n; i++) {
      ans = cas(ans);
    }

    return ans;
  }
};

//==============================================================================

TEST(Test, s0038_count_and_say) {
  auto s = Solution{};
  EXPECT_EQ("1", s.countAndSay(1));
  EXPECT_EQ("11", s.countAndSay(2));
  EXPECT_EQ("21", s.countAndSay(3));
  EXPECT_EQ("1211", s.countAndSay(4));
}

