#include <fmt/core.h>
#include <fmt/ranges.h>
#include <unordered_map>
#include <vector>

using namespace std;

//==============================================================================

class Solution {
public:
  vector<int> twoSum(vector<int> &nums, int target) {
    auto map = unordered_map<int, int>{};
    for (int i = 0; i < nums.size(); ++i) {
      int n = target - nums[i];
      if (auto it = map.find(n); it != map.end()) {
        return {it->second, i};
      } else {
        map.insert({nums[i], i});
      }
    }
    return {};
  }
};

//==============================================================================

int main() {
  auto solution = Solution{};
  {
    auto nums = std::vector{2, 7, 11, 15};
    auto target = 9;
    fmt::println("{} == [0, 1]", solution.twoSum(nums, target));
  }
  {
    auto nums = std::vector{3, 2, 4};
    auto target = 6;
    fmt::println("{} == [1, 2]", solution.twoSum(nums, target));
  }
  {
    auto nums = std::vector{3, 3};
    auto target = 6;
    fmt::println("{} == [0, 1]", solution.twoSum(nums, target));
  }
}
