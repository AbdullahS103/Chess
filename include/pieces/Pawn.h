#ifndef PAWN_H
#define PAWN_H

#include "ChessPiece.h"

class Pawn : public ChessPiece {
public:

  Pawn(TeamColors color);

  bool isValidMove(Board &board, int fromRow, int fromCol, int toRow, int toCol);

  std::unordered_set<int> getAllValidMoves(Board &board, int row, int col);

  std::unordered_set<int> getAllControlSquares(Board &board, int row, int col);
  
  bool operator==(const ChessPiece &piece) const;
};

#endif