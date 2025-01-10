#include "BoardStateGenerator.h"
#include "PieceGenerator.h"
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
    board.setPiece(board.getRow(boardIndex), board.getColumn(boardIndex), PieceGenerator::createPieceByFEN(c));
    if (isdigit(c))
      boardIndex += c - '0';
    else 
      boardIndex++;
  }
}