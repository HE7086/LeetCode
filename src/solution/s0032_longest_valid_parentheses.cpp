/******************************
__DEBUG_FUNCTION<longestValidParentheses>__
Question 32: Longest Valid Parentheses
Difficulty: Hard

Given a string containing just the characters '(' and ')', return the length of the longest valid (well-formed) parentheses substring.

Example 1:

Input: s = "(()"
Output: 2
Explanation: The longest valid parentheses substring is "()".

Example 2:

Input: s = ")()())"
Output: 4
Explanation: The longest valid parentheses substring is "()()".

Example 3:

Input: s = ""
Output: 0


Constraints:

  0 <= s.length <= 3 * 10^4
  s[i] is '(', or ')'.

URL: https://leetcode.com/problems/longest-valid-parentheses
******************************/

#include <algorithm>
#include <stack>
#include <string_view>
#include <gtest/gtest.h>

using namespace std;

//==============================================================================

/* O(n) time; O(n) space
 *
 * Using a stack to keep track for the index of the open parentheses.
 * When the substring is valid, calculate the length using the saved index.
 * If the entire input string is valid, we'll need an extra dummy(-1) for the starting index.
 */
class Solution {
public:
  int longestValidParentheses(string_view s) {
    int ans = 0;
    auto st = stack<int>{};
    st.push(-1); // dummy
    for (int i = 0; static_cast<size_t>(i) < s.length(); i++) {
      if (s[i] == '(') {
        st.push(i);
      } else /* s[i] == ')' */ {
        st.pop();
        if (st.empty()){
          st.push(i); // next starting index
        } else {
          ans = max(ans, i - st.top());
        }
      }
    }
    return ans;
  }
};

//==============================================================================

TEST(Test, s0032_longest_valid_parentheses) {
  auto s = Solution{};
  EXPECT_EQ(2, s.longestValidParentheses("()(()"));
  EXPECT_EQ(2, s.longestValidParentheses("(()"));
  EXPECT_EQ(4, s.longestValidParentheses(")()())"));
  EXPECT_EQ(0, s.longestValidParentheses(""));
  EXPECT_EQ(2, s.longestValidParentheses("()"));
}

