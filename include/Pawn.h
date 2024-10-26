#ifndef PAWN_H
#define PAWN_H

#include "ChessPiece.h"

class Pawn : public ChessPiece {
public:
  Pawn(TeamColors color);
  ~Pawn();

  bool isValidMove(Board &board, int fromIndex, int toIndex);
  unordered_set<int> getAllValidMoves(Board &board, int index);
  string getIdentifier() const;
};

#endif