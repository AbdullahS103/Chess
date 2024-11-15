#ifndef BOARD_H
#define BOARD_H

#include "CommonHeaders.h"
#include "ChessPiece.h"

class ChessPiece;

// Board is a class that represents the state of the current chess board
class Board {
  ChessPiece **board;
  int rows;
  int columns;
  int boardSize;

  std::string toString() const;

public:
  Board();
  Board(int rows, int cols);
  ~Board();

  // getters
  int getRows() const;
  int getColumns() const;
  int getRow(int index) const;
  int getColumn(int index) const;
  const int getIndex(int row, int col) const;
  ChessPiece *getPiece(int row, int col) const;
  ChessPiece *getPiece(int index) const;

  // setters
  void setPiece(int row, int col, ChessPiece *piece);

  void movePiece(int fromRow, int fromCol, int toRow, int toCol);
  bool isOnBoard(int row, int col) const;
  int getBlackStrength() const;
  int getWhiteStrength() const;

  bool operator==(const Board &board) const;
  bool operator!=(const Board &board) const;
  bool operator==(std::nullptr_t) const;
  bool operator!=(std::nullptr_t) const;
  friend bool operator==(std::nullptr_t, const Board &board);
  friend bool operator!=(std::nullptr_t, const Board &board);
  friend std::ostream &operator<<(std::ostream &strm, const Board &board);
};

#endif