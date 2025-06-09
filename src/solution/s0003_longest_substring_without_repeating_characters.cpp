#include <algorithm>
#include <unordered_set>

#include <utils/test.hpp>

using namespace std;

//==============================================================================

static int lengthOfLongestSubstring(string s) {
  unordered_set<char> set{};
  int value = 0;
  for (int i = 0, j = 0; j < s.size();) {
    if (set.contains(s[j])) {
      set.erase(s[i++]);
    } else {
      set.emplace(s[j++]);
      value = max(value, j - i);
    }
  }
  return value;
}

//==============================================================================

int main() {
  ASSERT_EQ(1, lengthOfLongestSubstring(" "));
  ASSERT_EQ(3, lengthOfLongestSubstring("abcabcbb"));
  ASSERT_EQ(1, lengthOfLongestSubstring("bbbbb"));
  ASSERT_EQ(3, lengthOfLongestSubstring("pwwkew"));
}
