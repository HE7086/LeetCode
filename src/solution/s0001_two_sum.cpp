#include <unordered_map>
#include <vector>

#include <utils/test.hpp>

using namespace std;

//==============================================================================

static vector<int> twoSum(vector<int> const &nums, int target) {
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

//==============================================================================

int main() {
  ASSERT_EQ(std::vector{0, 1}, twoSum({2, 7, 11, 15}, 9));
  ASSERT_EQ(std::vector{1, 2}, twoSum({3, 2, 4}, 6));
  ASSERT_EQ(std::vector{0, 1}, twoSum({3, 3}, 6));
}
