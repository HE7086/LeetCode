/******************************
__DEBUG_FUNCTION<isMatch>__
Question 10: Regular Expression Matching
Difficulty: Hard

Given an input string sand a pattern p, implement regular expression matching with support for '.' and '*' where:

  '.' Matches any single character.
  '*' Matches zero or more of the preceding element.

The matching should cover the entire input string (not partial).

Example 1:

Input: s = "aa", p = "a"
Output: false
Explanation: "a" does not match the entire string "aa".

Example 2:

Input: s = "aa", p = "a*"
Output: true
Explanation: '*' means zero or more of the preceding element, 'a'. Therefore, by repeating 'a' once, it becomes "aa".

Example 3:

Input: s = "ab", p = ".*"
Output: true
Explanation: ".*" means "zero or more (*) of any character (.)".


Constraints:

  1 <= s.length<= 20
  1 <= p.length<= 20
  s contains only lowercase English letters.
  p contains only lowercase English letters, '.', and'*'.
  It is guaranteed for each appearance of the character '*', there will be a previous valid character to match.

URL: https://leetcode.com/problems/regular-expression-matching
******************************/

#include <string>
#include <gtest/gtest.h>

using namespace std;

//==============================================================================

/* O(2^(m+n)) time; O(m+n) space
 * Basic solution: recursive backtracking.
 * For each '*' pattern, both zero and one-or-more matches' possibility is checked.
 */
/* class Solution {
 *   bool match(
 *       string::const_iterator it_s,
 *       string::const_iterator end_s,
 *       string::const_iterator it_p,
 *       string::const_iterator end_p
 *   ) {
 *     // end of pattern reached, check whether input is exhausted
 *     if (it_p == end_p) {
 *       return it_s == end_s;
 *     }
 *
 *     // current character matches the pattern
 *     bool first_match = (it_s != end_s && (*it_p == *it_s || *it_p == '.'));
 *
 *     // next pattern is '*'
 *     if (*next(it_p) == '*') {
 *
 *       // assume '*' matches 0 occurrences -> skip 'X*' pattern
 *       if (match(it_s, end_s, next(it_p, 2), end_p)) {
 *         return true;
 *       }
 *
 *       // assume '*' matches 1 or more occurrences
 *       if (first_match) {
 *         return match(next(it_s), end_s, it_p, end_p);
 *       }
 *
 *       // no valid path found
 *       return false;
 *
 *     } else {
 *
 *       // move forward both iterators
 *       return first_match && match(next(it_s), end_s, next(it_p), end_p);
 *     }
 *   }
 *
 * public:
 *   bool isMatch(string const& s, string const& p) {
 *     return match(s.cbegin(), s.cend(), p.cbegin(), p.cend());
 *   }
 * };
 */

/* O(mn) time; O(mn) space
 * Recursive Backtracking is working on overlapping subproblems.
 * By memorizing the solved subproblem we could speed up a lot.
 * The logic is basically the same as RB.
 *
 * Starting from (0,0) (start of string, start of pattern),
 * (x, y) = (x-1, y-1) && match (s[x], p[y])
 *
 * for '*' pattern, same as RB:
 *   (x, y) = (x, y - 2)
 *   # case zero: just skip 2 patterns
 *
 *   (x, y) = (x, y) || (x - 1, y)
 *   # case one-or-more: take 1 input, pattern don't move
 */
// no std::vector<bool> pls
struct Bool {
  bool val = false;
  operator bool() const {
    return val;
  }
  Bool& operator=(bool b) {
    val = b;
    return *this;
  }
  Bool& operator|=(bool b) {
    val |= b;
    return *this;
  }
};
class Solution {
public:
  bool isMatch(string const& s, string const& p) {
    auto dp = vector<vector<Bool>>{s.size() + 1, vector<Bool>{p.size() + 1}};
    // dp [index of input string] [index of current pattern]
    dp[0][0] = true;

    // pattern starts with 'X*' -> allow empty string
    // also handle cases like 'X**'
    for (int i = 1; i < static_cast<int>(p.size()); i++) {
      if (p[i] == '*') {
        dp[0][i + 1] = dp[0][i - 1];
      }
    }

    for (int i = 0; i < static_cast<int>(s.size()); i++) {
      for (int j = 0; j < static_cast<int>(p.size()); j++) {
        // current pattern is '*', check last
        if (p[j] == '*') {
          // zero occurrence, skip 2 patterns
          dp[i + 1][j + 1] = dp[i + 1][j - 1];

          // one or more occurrences
          if (p[j - 1] == '.' || s[i] == p[j - 1]) {
            dp[i + 1][j + 1] |= dp[i][j + 1];
          }

        } else if (p[j] == '.' || s[i] == p[j]) {
          // match next char
          dp[i + 1][j + 1] = dp[i][j];
        }
      }
    }

    return dp[s.size()][p.size()];
  }
};

//==============================================================================

TEST(Test, s0010_regular_expression_matching) {
  auto s = Solution{};
  EXPECT_EQ(false, s.isMatch("aa", "a"));
  EXPECT_EQ(false, s.isMatch("ab", ".*c"));
  EXPECT_EQ(false, s.isMatch("ab", "..."));
  EXPECT_EQ(false, s.isMatch("ab", "."));

  EXPECT_EQ(true, s.isMatch("", ".*"));
  EXPECT_EQ(true, s.isMatch("abc", "..."));
  EXPECT_EQ(true, s.isMatch("aa", "a*"));
  EXPECT_EQ(true, s.isMatch("ab", ".*"));
  EXPECT_EQ(true, s.isMatch("ab", "a.*"));
  EXPECT_EQ(true, s.isMatch("b", "a*b"));
  EXPECT_EQ(true, s.isMatch("abc", ".*c"));
  EXPECT_EQ(true, s.isMatch("abccc", ".*c"));
}

