#ifndef BOARD_H
#define BOARD_H

#include "CommonHeaders.h"

class ChessPiece;

// Board is a class that represents the state of the current chess board
class Board {
  ChessPiece **board;
  int rows;
  int cols;
  int boardSize;

  string toString() const;

public:
  Board();
  Board(int rows, int cols);
  ~Board();

  // getters
  int getRow(int index);
  int getCol(int index);
  const int getIndex(int row, int col);
  ChessPiece *getPiece(int row, int col);

  // setters
  void setPiece(int row, int col, ChessPiece *piece);

  void movePiece(int fromRow, int fromCol, int toRow, int toCol);
  bool isOnBoard(int row, int col);
  int getBlackStrength();
  int getWhiteStrength();
  friend ostream &operator<<(ostream &strm, const Board &board);
};

#endif