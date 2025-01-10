#include "PieceGenerator.h"
#include "Pawn.h"
#include "King.h"
#include "Bishop.h"
#include "Rook.h"
#include "Knight.h"
#include "Queen.h"

ChessPiece* PieceGenerator::createPieceByFEN(char c) {
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

ChessPiece* PieceGenerator::createPieceByName(std::string &name, TeamColors team) {
  if (name == "Knight") 
    return new Knight(team);
  if (name == "Pawn")
    return new Pawn(team);
  if (name == "King")
    return new King(team);
  if (name == "Bishop")
    return new Bishop(team);
  if (name == "Rook")
    return new Rook(team);
  if (name == "Queen") 
    return new Queen(team);
  return nullptr;
}