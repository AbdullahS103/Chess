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

  ChessPiece(TeamColors color, std::string symbol);

  virtual ~ChessPiece() = default;

  // Mandatory methods that need to be defined to fully define a chess piece and its ruleset

  // Given the board state and the move, return if the move is legal
  virtual bool isValidMove(Board &board, int fromRow, int fromCol, int toRow, int toCol) = 0;

  // Given the board state and the location, return all valid moves for a given piece
  virtual std::unordered_set<int> getAllValidMoves(Board &board, int row, int col) = 0;

  // Returns a set of indexes that correspond to the squares the piece currently controls
  virtual std::unordered_set<int> getAllControlSquares(Board &board, int row, int col) = 0;

  // Equality operator for given piece
  virtual bool operator==(const ChessPiece &piece) const = 0;

  // Provided methods are supplemental methods that derived classes inherit

  std::string getSymbol() const;

  TeamColors getColor() const;

  bool isSameTeam(TeamColors color);

  bool isSameTeam(ChessPiece *piece);

  bool operator!=(const ChessPiece &piece) const;

  bool operator==(std::nullptr_t) const;

  bool operator!=(std::nullptr_t) const;

  friend bool operator==(std::nullptr_t, const ChessPiece &piece);

  friend bool operator!=(std::nullptr_t, const ChessPiece &piece);

  friend std::ostream &operator<<(std::ostream &strm, const ChessPiece &piece);
};

#endif