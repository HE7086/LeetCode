/******************************
__DEBUG_FUNCTION<groupAnagrams>__
Question 49: Group Anagrams
Difficulty: Med.

Given an array of strings strs, group the anagrams together. You can return the answer in any order.

Example 1:

Input: strs = ["eat","tea","tan","ate","nat","bat"]
Output: [["bat"],["nat","tan"],["ate","eat","tea"]]
Explanation:

  There is no string in strs that can be rearranged to form "bat".
  The strings "nat" and "tan" are anagrams as they can be rearranged to form each other.
  The strings "ate", "eat", and "tea" are anagrams as they can be rearranged to form each other.

Example 2:

Input: strs = [""]
Output: [[""]]

Example 3:

Input: strs = ["a"]
Output: [["a"]]


Constraints:

  1 <= strs.length <= 10^4
  0 <= strs[i].length <= 100
  strs[i] consists of lowercase English letters.

URL: https://leetcode.com/problems/group-anagrams
******************************/

#include <algorithm>
#include <array>
#include <string>
#include <ranges>
#include <unordered_map>
#include <vector>
#include <gtest/gtest.h>

using namespace std;

//==============================================================================

/* O(m * n) time; O(m * n) space
 *
 * Design a function that has the same output for every group of anagrams.
 * A solution like Gödel numbering would work, but the result is too large to fit.
 * So instead a frequency map is used. We first count the frequency of each letters in the input,
 * Then calculate hash value of the frequency map and put the string into corresponding bucket.
 *
 * Alternatively one could just sort the string.
 */
class Solution {
  struct Hasher {
    size_t operator()(array<int, 26> const& arr) const {
      size_t h = 0;
      for (int i : arr) {
        h ^= std::hash<int>{}(i) + 0x9e3779b9 + (h << 6) + (h >> 2);
      }
      return h;
    }
  };

  array<int, 26> hash(string str) {
    auto ans = array<int, 26>{};
    for (char c : str) {
      ans[c - 'a']++;
    }
    return ans;
  }

public:
  vector<vector<string>> groupAnagrams(vector<string>& strs) {
    auto m = unordered_map<array<int, 26>, vector<string>, Hasher>{};
    for (auto& str : strs) {
      m[hash(str)].emplace_back(std::move(str));
    }
    return std::move(m) | views::values | ranges::to<vector<vector<string>>>();
  }
};

//==============================================================================

TEST(Test, s0049_group_anagrams) {
  auto check = [](vector<vector<string>> expected, vector<string> input) {
    for (auto& v : expected) {
      sort(v.begin(), v.end());
    }
    sort(expected.begin(), expected.end());
    auto ans = Solution{}.groupAnagrams(input);
    for (auto& v : ans) {
      sort(v.begin(), v.end());
    }
    sort(ans.begin(), ans.end());
    EXPECT_EQ(expected, ans);
  };
  check({{""}}, {""});
  check({{"a"}}, {"a"});
  check(
      {
          {"bat"},
          {"nat", "tan"},
          {"ate", "eat", "tea"}
  },
      {"eat", "tea", "tan", "ate", "nat", "bat"}
  );
  check(
      {{"max"}, {"bar"}, {"doc"}, {"buy"}, {"may"}, {"duh"}, {"ill"}, {"pew"}, {"tin"}, {"cab"}},
      {"cab", "tin", "pew", "duh", "may", "ill", "buy", "bar", "max", "doc"}
  );
}

