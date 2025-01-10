#ifndef PIECE_GENERATOR_H
#define PIECE_GENERATOR_H

#include "CommonHeaders.h"
#include "TeamColors.h"
#include "ChessPiece.h"

// Class used to generate a chess piece based on an input
class PieceGenerator {

public:

  static ChessPiece* createPieceByFEN(char c);

  static ChessPiece* createPieceByName(std::string &string, TeamColors team);
};

#endif