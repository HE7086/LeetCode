/******************************
Question 3: Longest Substring Without Repeating Characters

Given a string s, find the length of the longest substring without duplicate characters.

Example 1:

Input: s = "abcabcbb"
Output: 3
Explanation: The answer is "abc", with the length of 3.

Example 2:

Input: s = "bbbbb"
Output: 1
Explanation: The answer is "b", with the length of 1.

Example 3:

Input: s = "pwwkew"
Output: 3
Explanation: The answer is "wke", with the length of 3.
Notice that the answer must be a substring, "pwke" is a subsequence and not a substring.


Constraints:

    0 <= s.length <= 5 * 10^4
    s consists of English letters, digits, symbols and spaces.

URL: https://leetcode.com/problems/longest-substring-without-repeating-characters
******************************/

#include <algorithm>
#include <unordered_set>
#include <gtest/gtest.h>

using namespace std;

//==============================================================================

class Solution {
public:
  int lengthOfLongestSubstring(string s) {
    unordered_set<char> set{};

    size_t value = 0;
    for (size_t i = 0, j = 0; j < s.size();) {
      if (set.contains(s[j])) {
        set.erase(s[i++]);
      } else {
        set.emplace(s[j++]);
        value = max(value, j - i);
      }
    }
    return value;
  }
};

//==============================================================================

TEST(Test, s0003_longest_substring_without_repeating_characters) {
  auto s = Solution{};
  EXPECT_EQ(1, s.lengthOfLongestSubstring(" "));
  EXPECT_EQ(3, s.lengthOfLongestSubstring("abcabcbb"));
  EXPECT_EQ(1, s.lengthOfLongestSubstring("bbbbb"));
  EXPECT_EQ(3, s.lengthOfLongestSubstring("pwwkew"));
}
