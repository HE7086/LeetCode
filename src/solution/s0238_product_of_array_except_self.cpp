/******************************
__DEBUG_FUNCTION<productExceptSelf>__
Question 238: Product of Array Except Self
Difficulty: Med.

Given an integer array nums, return an array answer such that answer[i] is equal to the product of all the elements of nums except nums[i].
The product of any prefix or suffix of nums is guaranteed to fit in a 32-bit integer.
You must write an algorithm that runs in O(n) time and without using the division operation.

Example 1:
Input: nums = [1,2,3,4]
Output: [24,12,8,6]
Example 2:
Input: nums = [-1,1,0,-3,3]
Output: [0,0,9,0,0]

Constraints:

  2 <= nums.length <= 10^5
  -30 <= nums[i] <= 30
  The input is generated such that answer[i] is guaranteed to fit in a 32-bit integer.


Follow up: Can you solve the problem in O(1) extra space complexity? (The output array does not count as extra space for space complexity analysis.)

URL: https://leetcode.com/problems/product-of-array-except-self
******************************/

#include <vector>
#include <gtest/gtest.h>

using namespace std;

//==============================================================================

/* O(n) time; O(1) space
 * productExceptSelf = product of all elements to the left * ... to the right
 */
class Solution {
public:
  vector<int> productExceptSelf(vector<int> nums) {
    auto ans = vector<int>(nums.size(), 1);

    int buf = 1;
    for (size_t i = 0; i < nums.size(); i++) {
      // product of all elements to the left except for itself
      ans[i] *= buf;
      buf *= nums[i];
    }

    buf = 1;
    for (int i = nums.size() - 1; i >= 0; i--) {
      // product of all elements to the right except for itself
      ans[i] *= buf;
      buf *= nums[i];
    }

    return ans;
  }
};

//==============================================================================

TEST(Test, s0238_product_of_array_except_self) {
  auto s = Solution{};
  EXPECT_EQ((vector<int>{24, 12, 8, 6}), s.productExceptSelf({1, 2, 3, 4}));
  EXPECT_EQ((vector<int>{0, 0, 9, 0, 0}), s.productExceptSelf({-1, 1, 0, -3, 3}));
}

