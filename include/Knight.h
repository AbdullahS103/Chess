#ifndef KNIGHT_H
#define KNIGHT_H

#include "ChessPiece.h"

class Knight : public ChessPiece {
public:
  Knight(TeamColors color);
  ~Knight();

  bool isValidMove(Board &board, int fromIndex, int toIndex);
  unordered_set<int> getAllValidMoves(Board &board, int index);
  string getIdentifier() const;
};

#endif