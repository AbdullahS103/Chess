#include "ChessPiece.h"
#include "Errors.h"

// Constructor for ChessPiece
ChessPiece::ChessPiece(TeamColors color) { this->color = color; }

// Destructor for ChessPiece, its a formality but it is good practice
ChessPiece::~ChessPiece(){};

ostream &operator<<(ostream &strm, const ChessPiece &chessPiece) {
  return strm << chessPiece.getIdentifier() << endl;
};

bool ChessPiece::isValidMove(Board &board, int fromRow, int fromCol, int toRow, int toCol) {
  throw UnsupportedFunctionException(
    "Class ChessPiece does not support isValidMove");
};

unordered_set<int> ChessPiece::getAllValidMoves(Board &board, int row, int col) {
  throw UnsupportedFunctionException(
    "Class ChessPiece does not support getAllValidMoves");
}

string ChessPiece::getIdentifier() const {
  throw UnsupportedFunctionException(
    "Class ChessPiece does not support getIdentifier");
}

bool ChessPiece::isSameTeam(TeamColors color) {
  return this->color == color;
}

bool ChessPiece::isSameTeam(ChessPiece *piece) {
  if (piece == nullptr)
    return false;
  return this->color == piece->color;
}
