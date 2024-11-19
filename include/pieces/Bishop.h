#ifndef BISHOP_H
#define BISHOP_H

#include "ChessPiece.h"

class Bishop : public ChessPiece {
public:
  Bishop(TeamColors color);
  ~Bishop();

  bool isValidMove(Board &board, int fromRow, int fromCol, int toRow, int toCol);
  std::unordered_set<int> getAllValidMoves(Board &board, int row, int col);
  std::string getSymbol() const;

  bool operator==(const ChessPiece &piece) const;
  bool operator!=(const ChessPiece &piece) const;
};

#endif
