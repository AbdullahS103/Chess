#ifndef QUEEN_H
#define QUEEN_H

#include "ChessPiece.h"

class Queen : public ChessPiece {
public:
  Queen(TeamColors color);
  ~Queen();

  bool isValidMove(Board &board, int fromIndex, int toIndex);
  unordered_set<int> getAllValidMoves(Board &board, int index);
  string getIdentifier() const;
};

#endif