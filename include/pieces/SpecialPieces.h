#ifndef SPECIAL_PIECES_H
#define SPECIAL_PIECES_H

#include "CommonHeaders.h"
#include "Board.h"

class Board;

class SpecialPieces {
public:
  virtual ~SpecialPieces() = default;
  virtual std::unordered_set<int> getAllPossibleMoves(const Board &board, int index) = 0;
  virtual std::unordered_set<int> getAllPossibleMoves(const Board &board, int row, int col) = 0;
};

#endif 