#ifndef CHESS_PIECE_H
#define CHESS_PIECE_H

#include "CommonHeaders.h"
#include "TeamColors.h"
#include "Board.h"

class Board;

// ChessPiece is an abstract class that represents a general chess piece
class ChessPiece {
protected:
  TeamColors color;
  std::string symbol;

public:
  ChessPiece(TeamColors color);
  virtual ~ChessPiece();

  // Given the board state and the move, return if the move is legal
  virtual bool isValidMove(Board &board, int fromRow, int fromCol, int toRow, int toCol);
  // Given the board state and the location, return all valid moves for a given piece
  virtual std::unordered_set<int> getAllValidMoves(Board &board, int row, int col);
  // Return a unique character associated with piece (mainly for debugging :0)
  virtual std::string getSymbol() const;

  bool isSameTeam(TeamColors color);
  bool isSameTeam(ChessPiece *piece);
  
  virtual bool operator==(const ChessPiece &piece) const;
  virtual bool operator!=(const ChessPiece &piece) const;
  bool operator==(std::nullptr_t) const;
  bool operator!=(std::nullptr_t) const;
  friend bool operator==(std::nullptr_t, const ChessPiece &piece);
  friend bool operator!=(std::nullptr_t, const ChessPiece &piece);
  friend std::ostream &operator<<(std::ostream &strm, const ChessPiece &piece);
};

#endif