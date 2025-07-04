/******************************
__DEBUG_FUNCTION<nextPermutation>__
Question 31: Next Permutation
Difficulty: Med.

A permutation of an array of integers is an arrangement of its members into a sequence or linear order.

  For example, for arr = [1,2,3], the following are all the permutations of arr: [1,2,3], [1,3,2], [2, 1, 3], [2, 3, 1], [3,1,2], [3,2,1].

The next permutation of an array of integers is the next lexicographically greater permutation of its integer. More formally, if all the permutations of the array are sorted in one container according to their lexicographical order, then the next permutation of that array is the permutation that follows it in the sorted container. If such arrangement is not possible, the array must be rearranged as the lowest possible order (i.e., sorted in ascending order).

  For example, the next permutation of arr = [1,2,3] is [1,3,2].
  Similarly, the next permutation of arr = [2,3,1] is [3,1,2].
  While the next permutation of arr = [3,2,1] is [1,2,3] because [3,2,1] does not have a lexicographical larger rearrangement.

Given an array of integers nums, find the next permutation of nums.
The replacement must be in place and use only constant extra memory.

Example 1:

Input: nums = [1,2,3]
Output: [1,3,2]

Example 2:

Input: nums = [3,2,1]
Output: [1,2,3]

Example 3:

Input: nums = [1,1,5]
Output: [1,5,1]


Constraints:

  1 <= nums.length <= 100
  0 <= nums[i] <= 100

URL: https://leetcode.com/problems/next-permutation
******************************/

#include <algorithm>
#include <utility>
#include <vector>
#include <gtest/gtest.h>

using namespace std;

//==============================================================================

/* O(n) time; O(1) space
 *
 * 1. find last decreasing element (i)
 * 2. find the smallest element (j) that is larger than (i)
 * 3. swap (i) and (j)
 * 4. reverse all the elements after (i)
 */
class Solution {
public:
  void nextPermutation(vector<int>& nums) {
    int i = nums.size() - 2;
    for (; i >= 0 && nums[i] >= nums[i + 1]; i--) {}
    if (i >= 0) {
      int j = nums.size() - 1;
      for (; nums[j] <= nums[i]; j--) {}
      swap(nums[i], nums[j]);
    }
    reverse(nums.begin() + i + 1, nums.end());

    // use the function from <algorithm> for real world purposes.
    // std::next_permutation(nums.begin(), nums.end());
  }

  void next_permutation(vector<int>& nums) {
    auto r_first = std::make_reverse_iterator(nums.end());
    auto r_last = std::make_reverse_iterator(nums.begin());
    auto left = std::is_sorted_until(r_first, r_last);
 
    if (left != r_last) {
        auto right = std::upper_bound(r_first, left, *left);
        std::iter_swap(left, right);
    }
 
    std::reverse(left.base(), nums.end());
    // if (left != r_last) -> next permutation exists
  }
};

//==============================================================================

TEST(Test, s0031_next_permutation) {
  auto check = [](vector<int> expected, vector<int> input) {
    Solution{}.nextPermutation(input);
    EXPECT_EQ(expected, input);
  };
  check({1, 3, 2}, {1, 2, 3});
  check({1, 2, 3}, {3, 2, 1});
  check({1, 5, 1}, {1, 1, 5});
}
