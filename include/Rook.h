#ifndef ROOK_H
#define ROOK_H

#include "ChessPiece.h"

class Rook : public ChessPiece {
public:
  Rook(TeamColors color);
  ~Rook();

  bool isValidMove(Board &board, int fromIndex, int toIndex);
  unordered_set<int> getAllValidMoves(Board &board, int index);
  string getIdentifier() const;
};

#endif