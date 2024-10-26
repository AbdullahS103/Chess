#ifndef KING_H
#define KING_H

#include "ChessPiece.h"

class King : public ChessPiece {
public:
  King(TeamColors color);
  ~King();

  bool isValidMove(Board &board, int fromIndex, int toIndex);
  unordered_set<int> getAllValidMoves(Board &board, int index);
  string getIdentifier() const;
};

#endif