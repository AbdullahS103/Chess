#ifndef ROOK_H
#define ROOK_H

#include "ChessPiece.h"

class Rook : public ChessPiece {
public:
  Rook(TeamColors color);
  ~Rook();

  bool isValidMove(Board &board, int fromRow, int fromCol, int toRow, int toCol);
  unordered_set<int> getAllValidMoves(Board &board, int row, int col);
  string getSymbol() const;
};

#endif