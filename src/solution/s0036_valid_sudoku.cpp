/******************************
__DEBUG_FUNCTION<isValidSudoku>__
Question 36: Valid Sudoku
Difficulty: Med.

Determine if a 9 x 9 Sudoku board is valid. Only the filled cells need to be validated according to the following rules:

  Each row must contain the digits 1-9 without repetition.
  Each column must contain the digits 1-9 without repetition.
  Each of the nine 3 x 3 sub-boxes of the grid must contain the digits 1-9 without repetition.

Note:

  A Sudoku board (partially filled) could be valid but is not necessarily solvable.
  Only the filled cells need to be validated according to the mentioned rules.


Example 1:

Input: board =
[["5","3",".",".","7",".",".",".","."]
,["6",".",".","1","9","5",".",".","."]
,[".","9","8",".",".",".",".","6","."]
,["8",".",".",".","6",".",".",".","3"]
,["4",".",".","8",".","3",".",".","1"]
,["7",".",".",".","2",".",".",".","6"]
,[".","6",".",".",".",".","2","8","."]
,[".",".",".","4","1","9",".",".","5"]
,[".",".",".",".","8",".",".","7","9"]]
Output: true

Example 2:

Input: board =
[["8","3",".",".","7",".",".",".","."]
,["6",".",".","1","9","5",".",".","."]
,[".","9","8",".",".",".",".","6","."]
,["8",".",".",".","6",".",".",".","3"]
,["4",".",".","8",".","3",".",".","1"]
,["7",".",".",".","2",".",".",".","6"]
,[".","6",".",".",".",".","2","8","."]
,[".",".",".","4","1","9",".",".","5"]
,[".",".",".",".","8",".",".","7","9"]]
Output: false
Explanation: Same as Example 1, except with the 5 in the top left corner being modified to 8. Since there are two 8's in the top left 3x3 sub-box, it is invalid.


Constraints:

  board.length == 9
  board[i].length == 9
  board[i][j] is a digit 1-9 or '.'.

URL: https://leetcode.com/problems/valid-sudoku
******************************/

#include <array>
#include <vector>
#include <gtest/gtest.h>

using namespace std;

//==============================================================================

/* O(n^2) time; O(n) space
 * Technically the board is in fixed size so the runtime is also constant.
 */
class Solution {
public:
  bool isValidSudoku(vector<vector<char>> board) {
    // for up to 9 elements set is usually faster than unordered set
    // we could just use another array but it uses more memory for a sparse matrix
    auto row = array<set<char>, 9>{};
    auto col = array<set<char>, 9>{};
    auto box = array<set<char>, 9>{};

    for (size_t i = 0; i < board.size(); i++) {
      for (size_t j = 0; j < board[i].size(); j++) {
        if (board[i][j] == '.') {
          continue;
        }
        int k = i / 3 * 3 + j / 3; // identify the box based on the coordinates.

        char c = board[i][j];
        if (row[i].contains(c) || col[j].contains(c) || box[k].contains(c)) {
          return false;
        }
        row[i].insert(c);
        col[j].insert(c);
        box[k].insert(c);
      }
    }

    return true;
  }
};

//==============================================================================

TEST(Test, s0036_valid_sudoku) {
  auto s = Solution{};
  EXPECT_EQ(
      true,
      s.isValidSudoku({
          {'5', '3', '.', '.', '7', '.', '.', '.', '.'},
          {'6', '.', '.', '1', '9', '5', '.', '.', '.'},
          {'.', '9', '8', '.', '.', '.', '.', '6', '.'},
          {'8', '.', '.', '.', '6', '.', '.', '.', '3'},
          {'4', '.', '.', '8', '.', '3', '.', '.', '1'},
          {'7', '.', '.', '.', '2', '.', '.', '.', '6'},
          {'.', '6', '.', '.', '.', '.', '2', '8', '.'},
          {'.', '.', '.', '4', '1', '9', '.', '.', '5'},
          {'.', '.', '.', '.', '8', '.', '.', '7', '9'}
  })
  );
  EXPECT_EQ(
      false,
      s.isValidSudoku({
          {'8', '3', '.', '.', '7', '.', '.', '.', '.'},
          {'6', '.', '.', '1', '9', '5', '.', '.', '.'},
          {'.', '9', '8', '.', '.', '.', '.', '6', '.'},
          {'8', '.', '.', '.', '6', '.', '.', '.', '3'},
          {'4', '.', '.', '8', '.', '3', '.', '.', '1'},
          {'7', '.', '.', '.', '2', '.', '.', '.', '6'},
          {'.', '6', '.', '.', '.', '.', '2', '8', '.'},
          {'.', '.', '.', '4', '1', '9', '.', '.', '5'},
          {'.', '.', '.', '.', '8', '.', '.', '7', '9'}
  })
  );
}

