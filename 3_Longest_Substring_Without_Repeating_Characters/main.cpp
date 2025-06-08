#include <fmt/core.h>
#include <fmt/ranges.h>

#include <algorithm>
#include <unordered_set>

using namespace std;

//==============================================================================

class Solution {
public:
  int lengthOfLongestSubstring(string s) {
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
};

//==============================================================================

int main() {
  auto solution = Solution{};
  {
    string s = " ";
    int out = 1;
    fmt::println("{} -> {} ({})", s, solution.lengthOfLongestSubstring(s), out);
  }
  {
    string s = "abcabcbb";
    int out = 3;
    fmt::println("{} -> {} ({})", s, solution.lengthOfLongestSubstring(s), out);
  }
  {
    string s = "bbbbb";
    int out = 1;
    fmt::println("{} -> {} ({})", s, solution.lengthOfLongestSubstring(s), out);
  }
  {
    string s = "pwwkew";
    int out = 3;
    fmt::println("{} -> {} ({})", s, solution.lengthOfLongestSubstring(s), out);
  }
}
