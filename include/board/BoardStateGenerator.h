#ifndef BOARD_STATE_GENERATOR_H
#define BOARD_STATE_GENERATOR_H

#include "CommonHeaders.h"

class Board;
class ChessPiece;

class BoardStateGenerator {

  static const std::string STANDARD_CHESS_BOARD_FEN;

public:

  // Sets up a standard game of chess
  static void standardBoard(Board &board);

  // Creates board based on FEN string
  static void FENBoard(Board &board, std::string fenString);
};

#endif 