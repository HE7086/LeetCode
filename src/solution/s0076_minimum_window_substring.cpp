/******************************
__DEBUG_FUNCTION<minWindow>__
Question 76: Minimum Window Substring
Difficulty: Hard

Given two strings s and t of lengths m and n respectively, return the minimum window substring of s such that every character in t (including duplicates) is included in the window. If there is no such substring, return the empty string "".
The testcases will be generated such that the answer is unique.

Example 1:

Input: s = "ADOBECODEBANC", t = "ABC"
Output: "BANC"
Explanation: The minimum window substring "BANC" includes 'A', 'B', and 'C' from string t.

Example 2:

Input: s = "a", t = "a"
Output: "a"
Explanation: The entire string s is the minimum window.

Example 3:

Input: s = "a", t = "aa"
Output: ""
Explanation: Both 'a's from t must be included in the window.
Since the largest window of s only has one 'a', return empty string.


Constraints:

  m == s.length
  n == t.length
  1 <= m, n <= 10^5
  s and t consist of uppercase and lowercase English letters.


Follow up: Could you find an algorithm that runs in O(m + n) time?

URL: https://leetcode.com/problems/minimum-window-substring
******************************/

#include <array>
#include <limits>
#include <string>
#include <gtest/gtest.h>

using namespace std;

//==============================================================================

/* O(n) time; O(1) space
 * sliding window
 */
class Solution {
  static constexpr size_t to_index(char c) {
    if ('a' <= c && c <= 'z') {
      return c - 'a';
    } else {
      return 26 + c - 'A';
    }
  }

public:
  string minWindow(string const& text, string const& pattern) {
    if (pattern.empty() || text.size() < pattern.size()) {
      return "";
    }

    array<int, 52> need{};
    for (char c : pattern) {
      need[to_index(c)]++;
    }

    size_t remaining  = pattern.size();
    size_t best_begin = 0;
    size_t best_end   = numeric_limits<size_t>::max();

    for (size_t begin = 0, end = 0; end < text.size(); end++) {
      if (need[to_index(text[end])] > 0) {
        // match found
        remaining--;
      }
      need[to_index(text[end])]--;

      while (remaining == 0) {
        size_t current = end - begin + 1;
        if (current < best_end) {
          best_end   = current;
          best_begin = begin;
        }

        need[to_index(text[begin])]++;
        if (need[to_index(text[begin])] > 0) {
          // last char was needed
          remaining++;
        }
        begin++;
      }
    }

    if (best_end == numeric_limits<size_t>::max()) {
      return "";
    }
    return text.substr(best_begin, best_end);
  }
};

//==============================================================================

TEST(Test, s0076_minimum_window_substring) {
  auto s = Solution{};
  EXPECT_EQ("BANC", s.minWindow("ADOBECODEBANC", "ABC"));
  EXPECT_EQ("a", s.minWindow("a", "a"));
  EXPECT_EQ("", s.minWindow("a", "aa"));
  EXPECT_EQ("", s.minWindow("a", "b"));
}
