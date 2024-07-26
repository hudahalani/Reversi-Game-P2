
int directions[8][2] = {{-1, -1}, {-1, 0}, {-1, 1}, {0, -1},
                        {0, 1},   {1, -1}, {1, 0},  {1, 1}};

  int makeMove(const char board[][26], int n, char turn, int *row, int *col) {
  char opponent = 'U';
  int highestScore = 0;
  bool found = false;
  if (turn == 'W') {
    opponent = 'B';
  } else {
    opponent = 'W';
  }
  for (int rowz = 0; rowz < n; rowz++) {
    for (int colz = 0; colz < n; colz++) {
      int score = 0;
      for (int i = 0; i < 8; i++) {
        int rowDirection = directions[i][0];
        int columnDirection = directions[i][1];
        int r = row + rowDirection;
        int c = col + columnDirection;
        bool foundOpponent = false;
        while (r >= 0 && r < n && c >= 0 && c < n &&
               board[r][c] == opponent) {
          r += rowDirection;
          c += columnDirection;
          foundOpponent = true;
        }
        if (foundOpponent && r >= 0 && r < n && c >= 0 &&
            c < n && board[r][c] == turn) {
          r -= rowDirection;
          c -= columnDirection;
          while (r != row || c != col) {
            if (board[row][col] == 'U') {
              score++;


              
              found = true;
              if (score > highestScore) {
                highestScore = score;
                (*row) = row;
                (*col) = col;
              }
            }
            r -= rowDirection;
            c -= columnDirection;
          }
        }
      }
    }
  }
  return 0;
}