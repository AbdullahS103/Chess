#ifndef KING_H
#define KING_H

#include "ChessPiece.h"

class King : public ChessPiece {
public:
  King(TeamColors color);
  ~King();

  bool isValidMove(Board &board, int fromRow, int fromCol, int toRow, int toCol);
  std::unordered_set<int> getAllValidMoves(Board &board, int row, int col);
  std::string getSymbol() const;

  bool operator==(const ChessPiece &piece) const;
  bool operator!=(const ChessPiece &piece) const;
};

#endif