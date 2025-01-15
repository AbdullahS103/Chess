#ifndef FEN_MANAGER_H
#define FEN_MANAGER_H

#include "CommonHeaders.h"
#include "Board.h"

// Class used to validate FEN and deconstruct FEN string
class FENManager {

  std::string layout;

  char turn;

  std::string castlingRights;

  std::string enpassantRights;

  int halfmoves;

  int fullmoves;

  static std::vector<std::string> split(std::string str, char delimiter);

  bool isValidLayout(std::string layout);

  bool isValidTurn(std::string turn);

  bool isValidCastlingRights(std::string castling, std::string layout);

  bool isValidEnpassantRights(std::string enpassant);

  bool isValidHalfMoves(std::string halfmoves);

  bool isValidFullMoves(std::string fullmoves);

  bool isValidFEN(std::string &fenString);

public:

  FENManager(std::string fenString);

  ~FENManager();

  std::string getLayout();

  char getTurn();

  std::string getCastlingRights();

  std::string getEnpassantRights();

  int getHalfMoves();

  int getFullMoves();

  static std::string getFENString(Board &board);
};

#endif