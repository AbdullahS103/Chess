#ifndef QUEEN_H
#define QUEEN_H

#include "ChessPiece.h"

class Queen : public ChessPiece {
public:
  Queen(TeamColors color);
  ~Queen();

  bool isValidMove(Board &board, int fromRow, int fromCol, int toRow, int toCol);
  unordered_set<int> getAllValidMoves(Board &board, int row, int col);
  string getIdentifier() const;
};

#endif