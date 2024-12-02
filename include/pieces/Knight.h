#ifndef KNIGHT_H
#define KNIGHT_H

#include "ChessPiece.h"
#include "SpecialPieces.h"

class Knight : public ChessPiece, public SpecialPieces {
public:
  Knight(TeamColors color);
  ~Knight();

  bool isValidMove(Board &board, int fromRow, int fromCol, int toRow, int toCol);
  std::unordered_set<int> getAllValidMoves(Board &board, int row, int col);
  std::string getSymbol() const;
  std::unordered_set<int> getAllPossibleMoves(const Board &board, int index);
  std::unordered_set<int> getAllPossibleMoves(const Board &board, int row, int col);

  bool operator==(const ChessPiece &piece) const;
  bool operator!=(const ChessPiece &piece) const;
};

#endif