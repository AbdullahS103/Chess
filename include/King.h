#ifndef KING_H
#define KING_H

#include "ChessPiece.h"

class King : public ChessPiece {
public:
  King(TeamColors color);
  ~King();

  bool isValidMove(Board &board, int fromRow, int fromCol, int toRow, int toCol);
  unordered_set<int> getAllValidMoves(Board &board, int row, int col);
  string getSymbol() const;
};

#endif