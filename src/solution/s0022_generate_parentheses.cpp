/******************************
__DEBUG_FUNCTION<generateParenthesis>__
Question 22: Generate Parentheses
Difficulty: Med.

Given n pairs of parentheses, write a function to generate all combinations of well-formed parentheses.

Example 1:
Input: n = 3
Output: ["((()))","(()())","(())()","()(())","()()()"]
Example 2:
Input: n = 1
Output: ["()"]

Constraints:

  1 <= n <= 8

URL: https://leetcode.com/problems/generate-parentheses
******************************/

#include <algorithm>
#include <cstdint>
#include <string>
#include <vector>
#include <gtest/gtest.h>

using namespace std;

//==============================================================================

/* Cn: calatan numbers of n
 * O(n * Cn) time; O(n * Cn) space
 *
 * For input n, there are Cn possible combinations.
 * For convenience the catalan numbers are generated at compile time here.
 *
 * Here we use a recursive backtracking strategy.
 * For each recursion we need to make sure:
 *   - there are more open braces than closed ones.
 *   - there are at most N open braces as well as closed ones.
 */
template<size_t N>
consteval array<uint64_t, N> gen_catalans() {
  array<uint64_t, N> values{};
  if (N > 0) {
    values[0] = 1;
  }

  for (size_t i = 1; i < N; i++) {
    uint64_t sum = 0;

    for (size_t j = 0; j < i; j++) {
      sum += values[j] * values[i - 1 - j];
    }
    values[i] = sum;
  }

  return values;
}
static constinit array<uint64_t, 8> catalan = gen_catalans<8>();

class Solution {
public:
  vector<string> generateParenthesis(int n) {
    [[assume(0 < n && n < 9)]];
    auto ans = vector<string>();
    ans.reserve(catalan[n]);
    auto str = string{};
    if (n > 7) {
      // starting from 8, SSO does not apply
      str.reserve(2 * n);
    }

    [&](this auto const& self, int open, int close) -> void {
      if (open == n && close == n) {
        ans.push_back(str);
        return;
      }

      if (open < n) {
        str.push_back('(');
        self(open + 1, close);
        str.pop_back();
      }
      if (open > close && close < n) {
        str.push_back(')');
        self(open, close + 1);
        str.pop_back();
      }
    }(0, 0);

    return ans;
  }
};

//==============================================================================

TEST(Test, s0022_generate_parentheses) {
  auto check = [](vector<string> expected, int n) {
    sort(expected.begin(), expected.end());
    auto actual = Solution{}.generateParenthesis(n);
    sort(actual.begin(), actual.end());
    EXPECT_EQ(expected, actual);
  };
  check({"()"}, 1);
  check({"(())", "()()"}, 2);
  check({"((()))", "(()())", "(())()", "()(())", "()()()"}, 3);
  check(
      {"(((())))", "((()()))", "((())())", "((()))()", "(()(()))", "(()()())", "(()())()", "(())(())", "(())()()",
       "()((()))", "()(()())", "()(())()", "()()(())", "()()()()"},
      4
  );
}
