/******************************
__DEBUG_FUNCTION<letterCombinations>__
Question 17: Letter Combinations of a Phone Number
Difficulty: Med.

Given a string containing digits from 2-9 inclusive, return all possible letter combinations that the number could represent. Return the answer in any order.
A mapping of digits to letters (just like on the telephone buttons) is given below. Note that 1 does not map to any letters.

1 -
2 abc
3 def
4 ghi
5 jkl
6 mno
7 pqrs
8 tuv
9 wxyz
0 -

Example 1:

Input: digits = "23"
Output: ["ad","ae","af","bd","be","bf","cd","ce","cf"]

Example 2:

Input: digits = ""
Output: []

Example 3:

Input: digits = "2"
Output: ["a","b","c"]


Constraints:

  0 <= digits.length <= 4
  digits[i] is a digit in the range ['2', '9'].

URL: https://leetcode.com/problems/letter-combinations-of-a-phone-number
******************************/

#include <algorithm>
#include <string>
#include <utility>
#include <vector>
#include <gtest/gtest.h>

using namespace std;

//==============================================================================

/* O(n * 4^n) time; O(n * 4^n) space
 * Basic straightforward iterative approach.
 * As long as SSO applies, which means less than 16 chars per string,
 * the actual performance shouldn't be too bad (beating 100% already).
 */
/* class Solution {
 *   static constexpr vector<char> letters(char c) {
 *     [[assume('1' < c && c < ':')]];
 *     switch (c) {
 *       case '2': return {'a', 'b', 'c'};
 *       case '3': return {'d', 'e', 'f'};
 *       case '4': return {'g', 'h', 'i'};
 *       case '5': return {'j', 'k', 'l'};
 *       case '6': return {'m', 'n', 'o'};
 *       case '7': return {'p', 'q', 'r', 's'};
 *       case '8': return {'t', 'u', 'v'};
 *       case '9': return {'w', 'x', 'y', 'z'};
 *       default: unreachable();
 *     }
 *   }
 *
 * public:
 *   vector<string> letterCombinations(string const& digits) {
 *     auto ans = vector<string>{};
 *     if (digits.empty()) {
 *       return ans;
 *     }
 *
 *     for (char c : digits) {
 *       [[assume('1' < c && c < ':')]];
 *
 *       if (ans.empty()) {
 *         for (char l : letters(c)) {
 *           ans.push_back(string{l});
 *         }
 *       } else {
 *         auto temp = vector<string>{};
 *         for (char l : letters(c)) {
 *           for (auto s : ans) {
 *             temp.emplace_back(s + l);
 *           }
 *         }
 *         ans = std::move(temp);
 *       }
 *     }
 *     return ans;
 *   }
 * };
 */

/* O(n * 4^n) time; O(n * 4^n) space
 * Recursive version.
 * Should have a slightly smaller constant for time complexity,
 * and slightly greater for space, due to the recursive call stack.
 */
class Solution {
public:
  vector<string> letterCombinations(string const& digits) {
    // Need to put this lambda inside the function,
    // or it causes ICE on gcc 14
    static constexpr auto letters = [](char c) -> vector<char> {
      [[assume('1' < c && c < ':')]];
      switch (c) {
        case '2': return {'a', 'b', 'c'};
        case '3': return {'d', 'e', 'f'};
        case '4': return {'g', 'h', 'i'};
        case '5': return {'j', 'k', 'l'};
        case '6': return {'m', 'n', 'o'};
        case '7': return {'p', 'q', 'r', 's'};
        case '8': return {'t', 'u', 'v'};
        case '9': return {'w', 'x', 'y', 'z'};
        default: unreachable();
      }
    };

    auto ans = vector<string>{};
    if (digits.empty()) {
      return ans;
    }

    // starting from empty string
    auto buf = string{};

    // digits.length < 5 => SSO applies, no need for reservation.
    // buf.reserve(digits.length());

    [&](this auto const& self, size_t index) {
      if (index == digits.size()) {
        ans.push_back(buf);
        return;
      }

      for (char l : letters(digits[index])) {
        //          ^
        //            "this" pointer here refers to the lambda
        //            when the letter function is a member of the class Solution,
        //            the compiler tries to find this->letters
        //            and causes an internal compiler error.
        //            (seems to be fixed in gcc-15)

        // append a letter and go 1 level deeper
        buf.push_back(l);
        self(index + 1);

        // remove the last letter and handle the next
        buf.pop_back();
      }
      // starting from level 0
    }(0);

    return ans;
  }
};

//==============================================================================

TEST(Test, s0017_letter_combinations_of_a_phone_number) {
  auto check = [](vector<string> expected, string input) {
    sort(expected.begin(), expected.end());
    auto sol = Solution{}.letterCombinations(input);
    sort(sol.begin(), sol.end());
    EXPECT_EQ(expected, sol);
  };
  check({}, "");
  check({"a", "b", "c"}, "2");
  check({"ad", "ae", "af", "bd", "be", "bf", "cd", "ce", "cf"}, "23");
}
