//
// Author: Huda Halani
//
#include "lab8part1.h"

#include <stdio.h>
#include <stdlib.h>

int numberOfDirection = 8;
int directions[8][2] = {{-1, -1}, {-1, 0}, {-1, 1}, {0, -1},
                        {0, 1},   {1, -1}, {1, 0},  {1, 1}};

void initializeBoard(char board[][26], int dimension);
bool checkAvailableMoves(char board[][26], int dimension, char player);
bool isValidMove(char board[][26], int dimension, int row, int col,
                 char player);
void getUserInputAndMove(char board[][26], int dimension, char player);
void makeMove(char board[][26], int dimension, int row, int col, char player);
bool bestMove(char board[][26], int dimension, int* bestRow, int* bestCol,
              char colour);
void declareWinner(char board[][26], int dimension);

int main(void) {
  int dimension = 0;
  int bestRow = 0;
  int bestCol = 0;
  char turn = 0;
  char board[26][26];
  printf("Enter the board dimension: ");
  scanf(" %d", &dimension);
  initializeBoard(board, dimension);
  printf("Computer plays (B/W): ");
  scanf(" %c", &turn);
  printBoard(board, dimension);
  while (checkAvailableMoves(board, dimension, 'B') ||
         checkAvailableMoves(board, dimension, 'W')) {
    if (turn == 'B') {
      int opponent = 'W';
      if (bestMove(board, dimension, &bestRow, &bestCol, turn)) {
        printf("Computer places %c at %c%c.\n", turn, (char)(97 + bestRow),
               (char)(97 + bestCol));
        makeMove(board, dimension, bestRow, bestCol, turn);
        printBoard(board, dimension);
      } else {
        printf("%c player has no valid move.\n", turn);
      }
      if (bestMove(board, dimension, &bestRow, &bestCol, opponent)) {
        getUserInputAndMove(board, dimension, turn);
        printBoard(board, dimension);
      } else {
        printf("%c player has no valid move.\n", opponent);
      }

    } else if (turn == 'W') {
      getUserInputAndMove(board, dimension, turn);
      printBoard(board, dimension);
      if (bestMove(board, dimension, &bestRow, &bestCol, turn)) {
        printf("Computer places %c at %c%c.\n", turn, (char)(97 + bestRow),
               (char)(97 + bestCol));
        makeMove(board, dimension, bestRow, bestCol, turn);
        printBoard(board, dimension);
      } else if (checkAvailableMoves(board, dimension, 'B') ||
                 checkAvailableMoves(board, dimension, 'W')) {
        printf("%c player has no valid move.\n", turn);
      }
    }
  }
  declareWinner(board, dimension);

  return 0;
}

bool bestMove(char board[][26], int dimension, int* pointbestRow,
              int* pointbestCol, char colour)
               {
  char opponent = 'U';
  int highestScore = 0;
  bool found = false;
  if (colour == 'W') {
    opponent = 'B';
  } else {
    opponent = 'W';
  }
  for (int row = 0; row < dimension; row++) {
    for (int col = 0; col < dimension; col++) {
      int score = 0;
      for (int i = 0; i < 8; i++) {
        int rowDirection = directions[i][0];
        int columnDirection = directions[i][1];
        int r = row + rowDirection;
        int c = col + columnDirection;
        bool foundOpponent = false;
        while (r >= 0 && r < dimension && c >= 0 && c < dimension &&
               board[r][c] == opponent) {
          r += rowDirection;
          c += columnDirection;
          foundOpponent = true;
        }
        if (foundOpponent && r >= 0 && r < dimension && c >= 0 &&
            c < dimension && board[r][c] == colour) {
          r -= rowDirection;
          c -= columnDirection;
          while (r != row || c != col) {
            if (board[row][col] == 'U') {
              score++;
              found = true;
              if (score > highestScore) {
                highestScore = score;
                (*pointbestRow) = row;
                (*pointbestCol) = col;
              }
            }
            r -= rowDirection;
            c -= columnDirection;
          }
        }
      }
    }
  }
  return found;
}

void initializeBoard(char board[][26], int dimension) {
  for (int row = 0, i = 0; row < 26; row++, i++) {
    for (int col = 0; col < 26; col++) {
      if (row + 1 == dimension / 2 && col + 1 == dimension / 2)
        board[row][col] = 'W';
      else if (row == dimension / 2 && col == dimension / 2)
        board[row][col] = 'W';
      else if (row == dimension / 2 - 1 && col == dimension / 2)
        board[row][col] = 'B';
      else if (row == dimension / 2 && col == dimension / 2 - 1)
        board[row][col] = 'B';
      else {
        board[row][col] = 'U';
      }
    }
  }
}

void printBoard(char board[][26], int n) {
  printf("  ");
  char letters[26];
  for (int index = 0; index < 26; index++) {
    letters[index] = (char)('a' + index);
  }

  for (int i = 0; i < n; i++) {
    printf("%c", letters[i]);
  }
  printf("\n");

  for (int row = 0, i = 0; row < n; row++, i++) {
    printf("%c ", letters[i]);
    for (int col = 0; col < n; col++) {
      printf("%c", board[row][col]);
    }
    printf(" \n");
  }
}

bool checkAvailableMoves(char board[][26], int dimension, char colour) {
  bool moveHere = false;
  for (int row = 0; row < dimension; row++) {
    for (int col = 0; col < dimension; col++) {
      if (isValidMove(board, dimension, row, col, colour)) {
        moveHere = true;
      }
    }
  }
  return moveHere;
}

void getUserInputAndMove(char board[][26], int dimension, char player) {
  char rowChar;
  char colChar;
  char opponent;
  if (player == 'W') {
    opponent = 'B';
  } else {
    opponent = 'W';
  }
  printf("Enter move for colour %c (RowCol): ", opponent);
  scanf(" %c%c", &rowChar, &colChar);
  int row = rowChar - 'a';
  int col = colChar - 'a';
  if (isValidMove(board, dimension, row, col, opponent) &&
      positionInBounds(dimension, row, col)) {
    makeMove(board, dimension, row, col, opponent);
  } else {
    printf("Invalid move.\n");
    declareWinner(board, dimension);
    exit(0);
  }
}

void declareWinner(char board[][26], int dimension) {
  int blackScore = 0;
  int whiteScore = 0;
  for (int row = 0; row < dimension; row++) {
    for (int col = 0; col < dimension; col++) {
      if (board[row][col] == 'B') {
        blackScore++;
      } else if (board[row][col] == 'W') {
        whiteScore++;
      }
    }
  }
  if (blackScore > whiteScore) {
    printf("B player wins.\n");
  } else if (blackScore < whiteScore) {
    printf("W player wins.\n");
  }

  else if (blackScore < whiteScore) {
    printf("Draw!\n");
  }
}

bool isValidMove(char board[][26], int dimension, int row, int col,
                 char colour) {
  bool isValidMove = false;
  for (int i = 0; i < numberOfDirection; i++) {
    if (checkLegalInDirection(board, dimension, row, col, colour,
                              directions[i][0], directions[i][1])) {
      isValidMove = true;
    }
  }
  return isValidMove;
}

bool checkLegalInDirection(char board[][26], int dimension, int row, int col,
                           char colour, int deltaRow, int deltaCol) {
  char opponent = 'U';
  if (board[row][col] != 'U') return 0;

  if (colour == 'W') {  // changed
    opponent = 'B';
  } else {
    opponent = 'W';
  }
  int newRow = row + deltaRow;
  int newCol = col + deltaCol;
  bool foundOpponent = false;
  while (positionInBounds(dimension, newRow, newCol) &&
         board[newRow][newCol] == opponent) {
    newRow += deltaRow;
    newCol += deltaCol;
    foundOpponent = true;
  }
  if (foundOpponent && positionInBounds(dimension, newRow, newCol) &&
      board[newRow][newCol] == colour) {
    return true;
  }
  return false;
}

bool positionInBounds(int size, int row, int col) {
  return (row >= 0 && row < size && col >= 0 && col < size);
}

void makeMove(char board[][26], int dimension, int row, int col, char colour) {
  board[row][col] = colour;
  char opponent = 'U';
  if (colour == 'W') {
    opponent = 'B';
  } else {
    opponent = 'W';
  }

  for (int i = 0; i < 8; i++) {
    int rowDirection = directions[i][0];
    int columnDirection = directions[i][1];
    int r = row + rowDirection;
    int c = col + columnDirection;
    bool foundOpponent = false;
    while (r >= 0 && r < dimension && c >= 0 && c < dimension &&
           board[r][c] == opponent) {
      r += rowDirection;
      c += columnDirection;
      foundOpponent = true;
    }
    if (foundOpponent && r >= 0 && r < dimension && c >= 0 && c < dimension &&
        board[r][c] == colour) {
      r -= rowDirection;
      c -= columnDirection;
      while (r != row || c != col) {
        board[r][c] = colour;
        r -= rowDirection;
        c -= columnDirection;
      }
    }
  }
}

// gcc lab8part1.c -o lab8part1
//~aps105i/public/exercise 8 lab8part1
//~aps105i/public/submit 8
