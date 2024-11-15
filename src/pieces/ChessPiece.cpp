#include "ChessPiece.h"
#include "Errors.h"

// Constructor for ChessPiece
ChessPiece::ChessPiece(TeamColors color) { 
  this->color = color; 
}

// Destructor for ChessPiece, its a formality but it is good practice
ChessPiece::~ChessPiece(){};

ostream &operator<<(ostream &strm, const ChessPiece &piece) { return strm << piece.getSymbol() << endl; };

bool ChessPiece::isValidMove(Board &board, int fromRow, int fromCol, int toRow, int toCol) {
  throw UnsupportedFunctionException("Class ChessPiece does not support isValidMove");
};

unordered_set<int> ChessPiece::getAllValidMoves(Board &board, int row, int col) {
  throw UnsupportedFunctionException( "Class ChessPiece does not support getAllValidMoves");
}

string ChessPiece::getSymbol() const {
  throw UnsupportedFunctionException("Class ChessPiece does not support getSymbol");
}

bool ChessPiece::isSameTeam(TeamColors color) { return this->color == color; }

bool ChessPiece::isSameTeam(ChessPiece *piece) {
  if (piece == nullptr)
    return false;
  return this->color == piece->color;
}

bool ChessPiece::operator==(const ChessPiece &piece) const {
  throw UnsupportedFunctionException("Class ChessPiece does not support == operation");
}
bool ChessPiece::operator!=(const ChessPiece &piece) const { return !(*this == piece); };
bool ChessPiece::operator==(nullptr_t) const { return false; };
bool ChessPiece::operator!=(nullptr_t) const { return true; };
bool operator==(nullptr_t, const ChessPiece &piece) { return false; };
bool operator!=(nullptr_t, const ChessPiece &piece) { return true; };