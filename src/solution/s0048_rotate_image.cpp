/******************************
__DEBUG_FUNCTION<rotate>__
Question 48: Rotate Image
Difficulty: Med.

You are given an n x n 2D matrix representing an image, rotate the image by 90 degrees (clockwise).
You have to rotate the image in-place, which means you have to modify the input 2D matrix directly. DO NOT allocate another 2D matrix and do the rotation.

Example 1:

Input: matrix = [[1,2,3],[4,5,6],[7,8,9]]
Output: [[7,4,1],[8,5,2],[9,6,3]]

Example 2:

Input: matrix = [[5,1,9,11],[2,4,8,10],[13,3,6,7],[15,14,12,16]]
Output: [[15,13,2,5],[14,3,4,1],[12,6,8,9],[16,7,10,11]]


Constraints:

  n == matrix.length == matrix[i].length
  1 <= n <= 20
  -1000 <= matrix[i][j] <= 1000

URL: https://leetcode.com/problems/rotate-image
******************************/

#include <algorithm>
#include <utility>
#include <vector>
#include <gtest/gtest.h>

using namespace std;

//==============================================================================

/* O(n^2) time; O(1) space
 *
 * rotate clockwise: transpose -> reverse row
 * conter-clockwise: reverse row -> transpose
 */
class Solution {
public:
  void rotate(vector<vector<int>>& matrix) {
    // transpose
    for (size_t i = 0; i < matrix.size(); i++) {
      for (size_t j = i + 1; j < matrix.front().size(); j++) {
        swap(matrix[i][j], matrix[j][i]);
      }
    }

    for (auto& row : matrix) {
      reverse(row.begin(), row.end());
    }
  }
};

//==============================================================================

TEST(Test, s0048_rotate_image) {
  auto check = [](vector<vector<int>> expected, vector<vector<int>> input) {
    Solution{}.rotate(input);
    EXPECT_EQ(expected, input);
  };
  // clang-format off
  check({{1}}, {{1}});
  check({{3, 1}, {4, 2}}, {{1, 2}, {3, 4}});
  check(
    {
        {7, 4, 1},
        {8, 5, 2},
        {9, 6, 3}
    },
    {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    }
  );
  check(
    {
        {15, 13, 2,  5 },
        {14, 3,  4,  1 },
        {12, 6,  8,  9 },
        {16, 7,  10, 11}
    },
    {
        {5,  1,  9,  11},
        {2,  4,  8,  10},
        {13, 3,  6,  7 },
        {15, 14, 12, 16}
    }
  );
  //clang-format on
}
