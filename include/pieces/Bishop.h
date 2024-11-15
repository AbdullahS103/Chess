#ifndef BISHOP_H
#define BISHOP_H

#include "ChessPiece.h"

class Bishop : public ChessPiece {
public:
  Bishop(TeamColors color);
  ~Bishop();

  bool isValidMove(Board &board, int fromRow, int fromCol, int toRow, int toCol);
  unordered_set<int> getAllValidMoves(Board &board, int row, int col);
  string getSymbol() const;
};

#endif
