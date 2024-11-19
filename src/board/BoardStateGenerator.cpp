#include "BoardStateGenerator.h"
#include "Knight.h"
#include "Bishop.h"
#include "King.h"
#include "Rook.h"
#include "Queen.h"
#include "Pawn.h"
#include "Errors.h"

void BoardStateGenerator::standardBoard(Board &board) {
  if (board.getColumns() != 8 || board.getRows() != 8) 
    throw new InvalidBoardException("ERROR: Invalid board size for standard board");

  board.setPiece(0, 0, new Rook(TeamColors::BLACK));
  board.setPiece(0, 1, new Knight(TeamColors::BLACK));
  board.setPiece(0, 2, new Bishop(TeamColors::BLACK));
  board.setPiece(0, 3, new Queen(TeamColors::BLACK));
  board.setPiece(0, 4, new King(TeamColors::BLACK));
  board.setPiece(0, 5, new Bishop(TeamColors::BLACK));
  board.setPiece(0, 6, new Knight(TeamColors::BLACK));
  board.setPiece(0, 7, new Rook(TeamColors::BLACK));
  board.setPiece(1, 0, new Pawn(TeamColors::BLACK));
  board.setPiece(1, 1, new Pawn(TeamColors::BLACK));
  board.setPiece(1, 2, new Pawn(TeamColors::BLACK));
  board.setPiece(1, 3, new Pawn(TeamColors::BLACK));
  board.setPiece(1, 4, new Pawn(TeamColors::BLACK));
  board.setPiece(1, 5, new Pawn(TeamColors::BLACK));
  board.setPiece(1, 6, new Pawn(TeamColors::BLACK));
  board.setPiece(1, 7, new Pawn(TeamColors::BLACK));

  board.setPiece(7, 0, new Rook(TeamColors::WHITE));
  board.setPiece(7, 1, new Knight(TeamColors::WHITE));
  board.setPiece(7, 2, new Bishop(TeamColors::WHITE));
  board.setPiece(7, 3, new King(TeamColors::WHITE));
  board.setPiece(7, 4, new Queen(TeamColors::WHITE));
  board.setPiece(7, 5, new Bishop(TeamColors::WHITE));
  board.setPiece(7, 6, new Knight(TeamColors::WHITE));
  board.setPiece(7, 7, new Rook(TeamColors::WHITE));
  board.setPiece(6, 0, new Pawn(TeamColors::WHITE));
  board.setPiece(6, 1, new Pawn(TeamColors::WHITE));
  board.setPiece(6, 2, new Pawn(TeamColors::WHITE));
  board.setPiece(6, 3, new Pawn(TeamColors::WHITE));
  board.setPiece(6, 4, new Pawn(TeamColors::WHITE));
  board.setPiece(6, 5, new Pawn(TeamColors::WHITE));
  board.setPiece(6, 6, new Pawn(TeamColors::WHITE));
  board.setPiece(6, 7, new Pawn(TeamColors::WHITE));
}
