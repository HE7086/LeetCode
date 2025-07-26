/******************************
__DEBUG_FUNCTION<solveSudoku>__
Question 37: Sudoku Solver
Difficulty: Hard

Write a program to solve a Sudoku puzzle by filling the empty cells.
A sudoku solution must satisfy all of the following rules:

  Each of the digits 1-9 must occur exactly once in each row.
  Each of the digits 1-9 must occur exactly once in each column.
  Each of the digits 1-9 must occur exactly once in each of the 9 3x3 sub-boxes of the grid.

The '.' character indicates empty cells.

Example 1:

Input: board = [["5","3",".",".","7",".",".",".","."],["6",".",".","1","9","5",".",".","."],[".","9","8",".",".",".",".","6","."],["8",".",".",".","6",".",".",".","3"],["4",".",".","8",".","3",".",".","1"],["7",".",".",".","2",".",".",".","6"],[".","6",".",".",".",".","2","8","."],[".",".",".","4","1","9",".",".","5"],[".",".",".",".","8",".",".","7","9"]]
Output: [["5","3","4","6","7","8","9","1","2"],["6","7","2","1","9","5","3","4","8"],["1","9","8","3","4","2","5","6","7"],["8","5","9","7","6","1","4","2","3"],["4","2","6","8","5","3","7","9","1"],["7","1","3","9","2","4","8","5","6"],["9","6","1","5","3","7","2","8","4"],["2","8","7","4","1","9","6","3","5"],["3","4","5","2","8","6","1","7","9"]]
Explanation: The input board is shown above and the only valid solution is shown below:



Constraints:

  board.length == 9
  board[i].length == 9
  board[i][j] is a digit or '.'.
  It is guaranteed that the input board has only one solution.

URL: https://leetcode.com/problems/sudoku-solver
******************************/

#include <vector>
#include <gtest/gtest.h>

using namespace std;

//==============================================================================

/* let n = number of empty cells:
 * O(9^n) time; O(n) space for call stack + constant sized arrays
 * Without some fancy dfs algorithm this is as far as we can get.
 */
class Solution {
  array<array<bool, 9>, 9> row = {};
  array<array<bool, 9>, 9> col = {};
  array<array<bool, 9>, 9> box = {};

public:
  int get_box_index(int i, int j) {
    return i / 3 * 3 + j / 3;
  }

  void mark(int i, int j, char c, bool b) {
    int n = c - '1';
    int k = get_box_index(i, j);
    row[i][n] = b;
    col[j][n] = b;
    box[k][n] = b;
  }

  // check current row/column/box for existing c
  bool is_valid(vector<vector<char>>& board, int i, int j, char c) {
    int n = c - '1';
    int k = get_box_index(i, j);
    return !(row[i][n] || col[j][n] || box[k][n]);
  }

  // recursive backtracking trying each possible valid solution.
  bool solve(vector<vector<char>>& board) {
    for (int i = 0; i < 9; i++) {
      for (int j = 0; j < 9; j++) {
        if (board[i][j] != '.') {
          continue;
        }
        for (char c = '1'; c <= '9'; c++) {
          if (is_valid(board, i, j, c)) {
            board[i][j] = c;
            mark(i, j, c, true);
            if (solve(board)) {
              return true;
            } else {
              board[i][j] = '.';
              mark(i, j, c, false);
            }
          }
        }
        // all numbers are invalid, backtrack
        return false;
      }
    }
    return true; // all cells filled
  }
  void solveSudoku(vector<vector<char>>& board) {
    for (int i = 0; i < 9; i++) {
      for (int j = 0; j < 9; j++) {
        if (board[i][j] != '.') {
          int c = board[i][j] - '1';
          int k = get_box_index(i, j);

          row[i][c] = true;
          col[j][c] = true;
          box[k][c] = true;
        }
      }
    }
    solve(board);
  }
};

//==============================================================================

TEST(Test, s0037_sudoku_solver) {
  auto check = [](vector<vector<char>> expected, vector<vector<char>> board) {
    Solution{}.solveSudoku(board);
    EXPECT_EQ(expected, board);
  };
  // clang-format off
  check(
  {
    {'5', '3', '4', '6', '7', '8', '9', '1', '2'},
    {'6', '7', '2', '1', '9', '5', '3', '4', '8'},
    {'1', '9', '8', '3', '4', '2', '5', '6', '7'},
    {'8', '5', '9', '7', '6', '1', '4', '2', '3'},
    {'4', '2', '6', '8', '5', '3', '7', '9', '1'},
    {'7', '1', '3', '9', '2', '4', '8', '5', '6'},
    {'9', '6', '1', '5', '3', '7', '2', '8', '4'},
    {'2', '8', '7', '4', '1', '9', '6', '3', '5'},
    {'3', '4', '5', '2', '8', '6', '1', '7', '9'}
  },
  {
    {'5', '3', '.', '.', '7', '.', '.', '.', '.'},
    {'6', '.', '.', '1', '9', '5', '.', '.', '.'},
    {'.', '9', '8', '.', '.', '.', '.', '6', '.'},
    {'8', '.', '.', '.', '6', '.', '.', '.', '3'},
    {'4', '.', '.', '8', '.', '3', '.', '.', '1'},
    {'7', '.', '.', '.', '2', '.', '.', '.', '6'},
    {'.', '6', '.', '.', '.', '.', '2', '8', '.'},
    {'.', '.', '.', '4', '1', '9', '.', '.', '5'},
    {'.', '.', '.', '.', '8', '.', '.', '7', '9'}
  }
  );
  // clang-format on
}

