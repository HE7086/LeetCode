/******************************
Question 14: Longest Common Prefix
Difficulty: Easy

Write a function to find the longest common prefix string amongst an array of strings.
If there is no common prefix, return an empty string "".

Example 1:

Input: strs = ["flower","flow","flight"]
Output: "fl"

Example 2:

Input: strs = ["dog","racecar","car"]
Output: ""
Explanation: There is no common prefix among the input strings.


Constraints:

  1 <= strs.length <= 200
  0 <= strs[i].length <= 200
  strs[i] consists of only lowercase English letters if it is non-empty.

URL: https://leetcode.com/problems/longest-common-prefix
******************************/

#include <algorithm>
#include <ranges>
#include <string>
#include <vector>
#include <gtest/gtest.h>

using namespace std;

//==============================================================================

/* O(number of strings * length of string) time; O(1) space
 * Basic solution: iterate over all the inputs and look for prefixes linearly.
 */
/*
 * class Solution {
 * public:
 *   string longestCommonPrefix(vector<string> const& strs) {
 *     auto prefix = string_view{*strs.begin()};
 *     for (auto it = next(strs.begin()); it != strs.end(); it++) {
 *       prefix.remove_suffix(prefix.size() - min(prefix.size(), it->size()));
 *       while (!it->starts_with(prefix)) {
 *         prefix.remove_suffix(1);
 *         if (prefix.empty()) {
 *           return "";
 *         }
 *       }
 *     }
 *     return {prefix.begin(), prefix.end()};
 *   }
 * };
 */

/* O(n * log(l)) time; O(1) space
 * The longest common prefix can at most be as long as the shortest input.
 * So we can first find out the minimum, and do a binary search with it.
 * Here we use the first string as prefix and check, whether for all of the reset stings,
 * the substring (0, index) of the first string is a common prefix.
 * The index pointer is then moved using binary search.
 */
class Solution {
public:
  string longestCommonPrefix(vector<string> const& strs) {
    if (strs.empty()) {
      return "";
    }

    size_t left  = 1;
    size_t right = ranges::min(strs | views::transform(&string::length));
    auto   begin = next(strs.begin());

    while (left <= right) {
      size_t middle = (left + right) / 2;
      auto   prefix = string_view{*strs.begin()}.substr(0, middle);
      if (all_of(begin, strs.end(), [prefix](string const& s) { return s.starts_with(prefix); })) {
        left = middle + 1;
      } else {
        right = middle - 1;
      }
    }
    return strs.begin()->substr(0, (left + right) / 2);
  }
};

//==============================================================================

TEST(Test, s0014_longest_common_prefix) {
  auto s = Solution{};
  EXPECT_EQ("", s.longestCommonPrefix({}));
  EXPECT_EQ("flow", s.longestCommonPrefix({"flower", "flow"}));
  EXPECT_EQ("fl", s.longestCommonPrefix({"flower", "flow", "flight"}));
  EXPECT_EQ("", s.longestCommonPrefix({"dog", "racecar", "car"}));
}
