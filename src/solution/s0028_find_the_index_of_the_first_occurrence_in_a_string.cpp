/******************************
__DEBUG_FUNCTION<strStr>__
Question 28: Find the Index of the First Occurrence in a String
Difficulty: Easy

Given two strings needle and haystack, return the index of the first occurrence of needle in haystack, or -1 if needle is not part of haystack.

Example 1:

Input: haystack = "sadbutsad", needle = "sad"
Output: 0
Explanation: "sad" occurs at index 0 and 6.
The first occurrence is at index 0, so we return 0.

Example 2:

Input: haystack = "leetcode", needle = "leeto"
Output: -1
Explanation: "leeto" did not occur in "leetcode", so we return -1.


Constraints:

  1 <= haystack.length, needle.length <= 10^4
  haystack and needle consist of only lowercase English characters.

URL: https://leetcode.com/problems/find-the-index-of-the-first-occurrence-in-a-string
******************************/

#include <algorithm>
#include <functional>
#include <iterator>
#include <string>
#include <string_view>
#include <gtest/gtest.h>

using namespace std;

//==============================================================================

/* O(m + n) time; O(m) space
 * KMP algorithm.
 *
 * Ideally, use trivial solution when pattern is short, and KMP when long.
 * Threshhold should be determined by a benchmark.
 * A rough estimate would be at around 10~20 chars long pattern.
 */
class Solution {
  vector<int> computeLPSArray(string const& pattern) {
    auto lps = vector<int>(pattern.length(), 0);

    size_t length = 0;
    size_t i      = 1;

    while (i < pattern.length()) {
      if (pattern[i] == pattern[length]) {
        length++;
        lps[i] = length;
        i++;
      } else {
        if (length != 0) {
          length = lps[length - 1];
        } else {
          lps[i] = 0;
          i++;
        }
      }
    }
    return lps;
  }

public:
  int strStr(string const& haystack, string const& needle) {
    if (haystack.length() == 0) return 0;
    if (haystack.length() < needle.length()) return -1;

    auto lps = computeLPSArray(needle);

    size_t i = 0;
    size_t j = 0;

    while (i < haystack.length()) {
      if (needle[j] == haystack[i]) {
        i++;
        j++;
      }

      if (j == needle.length()) {
        return i - j;

        // if multiple matches needed, continue here.
        // j = lps[j - 1];
      } else if (i < haystack.length() && needle[j] != haystack[i]) {
        if (j != 0) {
          j = lps[j - 1];
        } else {
          i++;
        }
      }
    }
    return -1;
  }

  /* O(m * n) time; O(m) space
   * trivial solution.
   */
  int strStr_trivial(string const& haystack, string const& needle) {
    auto sv = string_view{haystack};
    for (size_t i = 0; i < haystack.length() - needle.length(); i++) {
      if (sv.substr(i, needle.length()) == needle) {
        return i;
      }
    }
    return -1;
  }

  int strStr_stl(string const& haystack, string const& needle) {
#if __cpp_lib_boyer_moore_searcher >= 201603L
    auto it = std::search(haystack.begin(), haystack.end(), std::boyer_moore_horspool_searcher{needle.begin(), needle.end()});
    if (it != haystack.end()) {
      return distance(haystack.begin(), it);
    }
    return -1;
#else
    return haystack.find(needle);
#endif
  }
};

//==============================================================================

TEST(Test, s0028_find_the_index_of_the_first_occurrence_in_a_string) {
  auto s = Solution{};
  EXPECT_EQ(0, s.strStr_trivial("sadbutsad", "sad"));
  EXPECT_EQ(-1, s.strStr_trivial("leetcode", "leeto"));
  EXPECT_EQ(0, s.strStr("sadbutsad", "sad"));
  EXPECT_EQ(-1, s.strStr("leetcode", "leeto"));
  EXPECT_EQ(0, s.strStr_stl("sadbutsad", "sad"));
  EXPECT_EQ(-1, s.strStr_stl("leetcode", "leeto"));
}
