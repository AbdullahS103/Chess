#ifndef KNIGHT_H
#define KNIGHT_H

#include "ChessPiece.h"

class Knight : public ChessPiece {
public:
  Knight(TeamColors color);
  ~Knight();

  bool isValidMove(Board &board, int fromRow, int fromCol, int toRow, int toCol);
  unordered_set<int> getAllValidMoves(Board &board, int row, int col);
  string getIdentifier() const;
};

#endif