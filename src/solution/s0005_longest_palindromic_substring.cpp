/******************************
Question 5: Longest Palindromic Substring

Given a string s, return the longest palindromic substring in s.

Example 1:

Input: s = "babad"
Output: "bab"
Explanation: "aba" is also a valid answer.

Example 2:

Input: s = "cbbd"
Output: "bb"


Constraints:

    1 <= s.length <= 1000
    s consist of only digits and English letters.

URL: https://leetcode.com/problems/longest-palindromic-substring
******************************/

#include <gtest/gtest.h>

using namespace std;

//==============================================================================

/* O(n^3) time; O(1) space
 * Brute force solution: check every possible substrings for palindrome.
 * Minor optimization: since we are looking for longest possible answer,
 * just starting from checking the longest substrings.
 */
/* class Solution {
 * public:
 *   bool is_palindromic(string_view s) {
 *     for (int i = 0; i < s.size() / 2; i++) {
 *       if (s[i] != s[s.size() - 1 - i]) {
 *         return false;
 *       }
 *     }
 *     return true;
 *   }
 *   string longestPalindrome(string s) {
 *     auto sv = std::string_view{s};
 *     for (int i = s.size(); i > 1; i--) {
 *       for (int j = 0; j < sv.size() + 1 - i; j++) {
 *         auto temp = sv.substr(j, i);
 *         if (is_palindromic(temp)) {
 *           return {temp.begin(), temp.end()};
 *         }
 *       }
 *     }
 *     return {s.front()};
 *   }
 * };
 */

/* O(n^2) time; O(1) space
 * Palindromes are symmetrical to its center. We only need to check each position if it forms a palindrome.
 * For each center position, expand to both sides and check for palindrome.
 * On each position, check for palidrome with odd and even length.
 *
 * Note: Manacher's algorithm can do O(n) time in O(n) space using a bogus character
 */
class Solution {
public:
  string_view expand(int i, int j, string_view s) {
    int left  = i;
    int right = j;

    while (left >= 0 && right < s.size() && s[left] == s[right]) {
      left--;
      right++;
    }

    return s.substr(left + 1, right - left - 1);
  }

  string longestPalindrome(string const& s) {
    auto ans = string_view{};

    for (int i = 0; i < s.size(); i++) {
      auto odd = expand(i, i, s);
      if (odd.size() > ans.size()) {
        ans = odd;
      }

      auto even = expand(i, i + 1, s);
      if (even.size() > ans.size()) {
        ans = even;
      }
    }

    return {ans.begin(), ans.end()};
  }
};

//==============================================================================

TEST(Test, s0005_longest_palindromic_substring) {
  auto s = Solution{};
  EXPECT_EQ("bab", s.longestPalindrome("babad"));
  EXPECT_EQ("bb", s.longestPalindrome("cbbd"));
  EXPECT_EQ("a", s.longestPalindrome("a"));
  EXPECT_EQ("bb", s.longestPalindrome("abcdbbfcba"));
}
