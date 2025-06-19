/******************************
Question 3: Longest Substring Without Repeating Characters
Difficulty: Med.

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

/* O(n) time; O(n) space
 * We are looking for substring *without* duplicates, so naturally we just store every characters in a set.
 * Use 2 pointers here for the beginning and end of the substring.
 * Each time we move the end pointer forward, the char is stored into the set.
 * And if it is already in, i.e. we find a duplicate, the start pointer is moved forward.
 * The maximum value of the difference between the 2 pointers is then the result.
 */
class Solution {
public:
  int lengthOfLongestSubstring(string s) {
    unordered_set<char> set{};

    size_t value = 0;
    for (size_t i = 0, j = 0; j < s.size();) {
      if (set.contains(s[j])) {
        // duplicate found
        // the erased char might not be the duplicate
        // so check it in the next loop again
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
