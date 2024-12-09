#ifndef FEN_MANAGER_H
#define FEN_MANAGER_H

#include "CommonHeaders.h"

// Class used to validate FEN and deconstruct FEN string
class FENManager {

  static std::vector<std::string> split(std::string str, char delimiter);

  static bool isValidLayout(std::string layout);

  static bool isValidTurn(std::string turn);

  static bool isValidCastlingState(std::string castling);

  static bool isValidEnpassantState(std::string enpassant);

  static bool isValidHalfMoves(std::string halfmoves);

  static bool isValidFullMoves(std::string fullmoves);

public:

  static bool isValidFEN(std::string &fenString);

  static std::string getBoardLayout(std::string &fenString);
};

#endif