#ifndef CHESS_PIECE_H
#define CHESS_PIECE_H

#include "CommonHeaders.h"
#include "TeamColors.h"
#include "Board.h"

// ChessPiece is an abstract class that represents a general chess piece
class ChessPiece {
protected:
  TeamColors color;

public:
  ChessPiece(TeamColors color);
  virtual ~ChessPiece();

  // Given the board state and the move, return if the move is legal
  virtual bool isValidMove(Board &board, int fromRow, int fromCol, int toRow, int toCol);
  // Given the board state and the location, return all valid moves for a given piece
  virtual unordered_set<int> getAllValidMoves(Board &board, int row, int col);
  // Return a unique character associated with piece (mainly for debugging :0)
  virtual string getIdentifier() const;

  bool isSameTeam(TeamColors color);
  // Print the the identifier of a chess piece
  friend ostream &operator<<(ostream &strm, const ChessPiece &chessPiece);
};

#endif