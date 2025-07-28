/******************************
__DEBUG_FUNCTION<isMatch>__
Question 44: Wildcard Matching
Difficulty: Hard

Given an input string (s) and a pattern (p), implement wildcard pattern matching with support for '?' and '*' where:

  '?' Matches any single character.
  '*' Matches any sequence of characters (including the empty sequence).

The matching should cover the entire input string (not partial).

Example 1:

Input: s = "aa", p = "a"
Output: false
Explanation: "a" does not match the entire string "aa".

Example 2:

Input: s = "aa", p = "*"
Output: true
Explanation: '*' matches any sequence.

Example 3:

Input: s = "cb", p = "?a"
Output: false
Explanation: '?' matches 'c', but the second letter is 'a', which does not match 'b'.


Constraints:

  0 <= s.length, p.length <= 2000
  s contains only lowercase English letters.
  p contains only lowercase English letters, '?' or '*'.

URL: https://leetcode.com/problems/wildcard-matching
******************************/

#include <optional>
#include <string>
#include <gtest/gtest.h>

using namespace std;

//==============================================================================

/* O(m * n) time; O(n) space
 *
 * Dynamic programming.
 */
/*
 * class Solution {
 *   struct Bool {
 *     bool val = false;
 *
 *     operator bool() const {
 *       return val;
 *     }
 *     Bool& operator=(bool b) {
 *       val = b;
 *       return *this;
 *     }
 *     Bool& operator&=(bool b) {
 *       val &= b;
 *       return *this;
 *     }
 *     bool operator&&(bool b) {
 *       return val && b;
 *     }
 *     Bool& operator|=(bool b) {
 *       val |= b;
 *       return *this;
 *     }
 *     bool operator||(bool b) {
 *       return val || b;
 *     }
 *     bool operator!() {
 *       return !val;
 *     }
 *     bool operator==(Bool const& other) {
 *       return val == other.val;
 *     }
 *   };
 *
 * public:
 *   bool isMatch(string s, string p) {
 *     // dp only depends on current and last row,
 *     // don't need to store the entire table.
 *     auto prev = vector<Bool>{p.length() + 1};
 *     auto curr = vector<Bool>{p.length() + 1};
 *     prev[0] = true;
 *
 *     // leading asterisks allow empty strings.
 *     for (size_t i = 1; i < p.length() + 1; i++) {
 *       if (p[i - 1] == '*') {
 *         prev[i] = true;
 *       } else {
 *         break;
 *       }
 *     }
 *
 *     for (size_t i = 1; i < s.length() + 1; i++) {
 *       curr[0] = false;
 *       for (size_t j = 1; j < p.length() + 1; j++) {
 *         if (p[j - 1] == '*') {
 *           // asterisk: left or up
 *           curr[j] = curr[j - 1] || prev[j];
 *         } else if (p[j - 1] == '?' || s[i - 1] == p[j - 1]) {
 *           // question mark / matched char: diagonal
 *           curr[j] = prev[j - 1];
 *         } else {
 *           curr[j] = false;
 *         }
 *       }
 *       prev = curr;
 *     }
 *
 *     return prev[p.length()];
 *   }
 * };
 */

/* O(m + n) time; O(1) space
 *
 * Greedy Backtracking.
 */
class Solution {
public:
  bool isMatch(string s, string p) {
    auto i     = s.begin();
    auto j     = p.begin();
    auto match = s.begin();

    auto asterisk = p.end();

    while (i != s.end()) {
      if (j != p.end() && (*i == *j || *j == '?')) {
        // matched char or question mark: move forward
        i++;
        j++;
      } else if (j != p.end() && *j == '*') {
        // asterisk: save location and move forward
        asterisk = j;
        match    = i;
        j++;
      } else if (asterisk != p.end()) {
        // no match: backtracking
        j = asterisk + 1;
        match++;
        i = match;
      } else {
        // unable to backtrack, abort
        return false;
      }
    }

    while (j != p.end() && *j == '*') {
      // consume trailing asterisks
      j++;
    }

    return j == p.end();
  }
};

//==============================================================================

TEST(Test, s0044_wildcard_matching) {
  auto s = Solution{};
  EXPECT_EQ(false, s.isMatch("aa", "a"));
  EXPECT_EQ(false, s.isMatch("cb", "?a"));
  EXPECT_EQ(true, s.isMatch("adceb", "*a*b"));
  EXPECT_EQ(true, s.isMatch("aa", "*"));
  EXPECT_EQ(true, s.isMatch("aa", "**"));
  EXPECT_EQ(true, s.isMatch("aa", "**a"));
  EXPECT_EQ(true, s.isMatch("aa", "?a"));
}
