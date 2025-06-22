/******************************
__DEBUG_FUNCTION<isValid>__
Question 20: Valid Parentheses
Difficulty: Easy

Given a string s containing just the characters '(', ')', '{', '}', '[' and ']', determine if the input string is valid.
An input string is valid if:

  Open brackets must be closed by the same type of brackets.
  Open brackets must be closed in the correct order.
  Every close bracket has a corresponding open bracket of the same type.


Example 1:

Input: s = "()"
Output: true

Example 2:

Input: s = "()[]{}"
Output: true

Example 3:

Input: s = "(]"
Output: false

Example 4:

Input: s = "([])"
Output: true


Constraints:

  1 <= s.length <= 10^4
  s consists of parentheses only '()[]{}'.

URL: https://leetcode.com/problems/valid-parentheses
******************************/

#include <stack>
#include <string>
#include <gtest/gtest.h>

using namespace std;

//==============================================================================

/* O(n) time; O(n) space
 * trivial parentheses matching using a stack.
 */
class Solution {
  static constexpr char get_pair(char c) {
    switch (c) {
      case ')': return '(';
      case ']': return '[';
      case '}': return '{';
      default: return c;
    }
  }

public:
  bool isValid(string const& str) {
    auto s = stack<char>{};
    for (char c : str) {
      char p = get_pair(c);
      if (c == p) {
        // not closing paren
        s.push(c);
      } else {
        if (s.empty()) {
          return false;
        }
        char last = s.top();
        if (last != p) {
          return false;
        }
        s.pop();
      }
    }
    return s.empty();
  }
};

//==============================================================================

TEST(Test, s0020_valid_parentheses) {
  auto s = Solution{};
  EXPECT_EQ(true, s.isValid("()"));
  EXPECT_EQ(true, s.isValid("()[]{}"));
  EXPECT_EQ(true, s.isValid("([])"));
  EXPECT_EQ(false, s.isValid("(]"));
  EXPECT_EQ(false, s.isValid("]"));
}

