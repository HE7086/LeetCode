#include <algorithm>
#include <unordered_set>

#include <utils/test.hpp>

//==============================================================================

static int lengthOfLongestSubstring(std::string s) {
  std::unordered_set<char> set{};
  int max = 0;
  for (int i = 0, j = 0; j < s.size();) {
    if (set.contains(s[j])) {
      set.erase(s[i++]);
    } else {
      set.emplace(s[j++]);
      max = std::max(max, j - i);
    }
  }
  return max;
}

//==============================================================================

int main() {
  using namespace testing;
  ASSERT_EQ(1, lengthOfLongestSubstring(" "));
  ASSERT_EQ(3, lengthOfLongestSubstring("abcabcbb"));
  ASSERT_EQ(1, lengthOfLongestSubstring("bbbbb"));
  ASSERT_EQ(3, lengthOfLongestSubstring("pwwkew"));
}
