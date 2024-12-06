#include "BoardStateGenerator.h"
#include "Knight.h"
#include "Bishop.h"
#include "King.h"
#include "Rook.h"
#include "Queen.h"
#include "Pawn.h"
#include "Errors.h"

const std::string BoardStateGenerator::STANDARD_CHESS_BOARD_FEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR";

void BoardStateGenerator::standardBoard(Board &board) {
  if (board.getColumns() != 8 || board.getRows() != 8) 
    throw new InvalidBoardException("ERROR: Invalid board size for standard board");

  return FENBoard(board, STANDARD_CHESS_BOARD_FEN);
}

void BoardStateGenerator::FENBoard(Board &board, std::string fenString) {
  int boardIndex = 0;
  for (int i = 0; i < fenString.length(); i++) {
    char c = fenString.at(i);
    if (c == '/')
      continue;
    board.setPiece(board.getRow(boardIndex), board.getColumn(boardIndex), createPiece(c));
    if (isdigit(c))
      boardIndex += c - '0';
    else 
      boardIndex++;
  }
}

ChessPiece* BoardStateGenerator::createPiece(char c) {
  switch (c) {
    case 'P': return new Pawn(TeamColors::WHITE);
    case 'p': return new Pawn(TeamColors::BLACK);
    case 'R': return new Rook(TeamColors::WHITE);
    case 'r': return new Rook(TeamColors::BLACK);
    case 'B': return new Bishop(TeamColors::WHITE);
    case 'b': return new Bishop(TeamColors::BLACK);
    case 'N': return new Knight(TeamColors::WHITE);
    case 'n': return new Knight(TeamColors::BLACK);
    case 'Q': return new Queen(TeamColors::WHITE);
    case 'q': return new Queen(TeamColors::BLACK);
    case 'K': return new King(TeamColors::WHITE);
    case 'k': return new King(TeamColors::BLACK);
    default: return nullptr;
  }
}