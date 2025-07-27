/******************************
__DEBUG_FUNCTION<multiply>__
Question 43: Multiply Strings
Difficulty: Med.

Given two non-negative integers num1 and num2 represented as strings, return the product of num1 and num2, also represented as a string.
Note: You must not use any built-in BigInteger library or convert the inputs to integer directly.

Example 1:
Input: num1 = "2", num2 = "3"
Output: "6"
Example 2:
Input: num1 = "123", num2 = "456"
Output: "56088"

Constraints:

  1 <= num1.length, num2.length <= 200
  num1 and num2 consist of digits only.
  Both num1 and num2 do not contain any leading zero, except the number 0 itself.

URL: https://leetcode.com/problems/multiply-strings
******************************/

#include <string>
#include <gtest/gtest.h>

using namespace std;

//==============================================================================

class Solution {
public:
  string multiply(string num1, string num2) {
    string ans;

    if (num1 == "0" || num2 == "0") {
      ans = "0"; // allow rvo
      return ans;
    }

    reverse(num1.begin(), num1.end());
    reverse(num2.begin(), num2.end());
    ans = string(num1.size() + num2.size(), '0');

    for (size_t i = 0; i < num2.size(); i++) {
      int digit1 = num2[i] - '0';

      for (size_t j = 0; j < num1.size(); j++) {
        int digit2  = num1[j] - '0';
        int carry   = ans[i + j] - '0';
        int product = digit1 * digit2 + carry;

        ans[i + j] = (product % 10) + '0';
        ans[i + j + 1] += product / 10;
      }
    }

    if (ans.back() == '0') {
      ans.pop_back();
    }
    reverse(ans.begin(), ans.end());
    return ans;
  }
};

//==============================================================================

TEST(Test, s0043_multiply_strings) {
  auto s = Solution{};
  EXPECT_EQ("6", s.multiply("2", "3"));
  EXPECT_EQ("56088", s.multiply("123", "456"));
}

