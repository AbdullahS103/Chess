#include "ChessPiece.h"
#include "Errors.h"

// Constructor for ChessPiece
ChessPiece::ChessPiece(TeamColors color) { 
  this->color = color; 
}

// Destructor for ChessPiece, its a formality but it is good practice
ChessPiece::~ChessPiece(){};

std::ostream &operator<<(std::ostream &strm, const ChessPiece &piece) { return strm << piece.getSymbol() << std::endl; };

bool ChessPiece::isValidMove(Board &board, int fromRow, int fromCol, int toRow, int toCol) {
  throw UnsupportedFunctionException("Class ChessPiece does not support isValidMove");
};

std::unordered_set<int> ChessPiece::getAllValidMoves(Board &board, int row, int col) {
  throw UnsupportedFunctionException( "Class ChessPiece does not support getAllValidMoves");
}

std::string ChessPiece::getSymbol() const {
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
bool ChessPiece::operator==(std::nullptr_t) const { return false; };
bool ChessPiece::operator!=(std::nullptr_t) const { return true; };
bool operator==(std::nullptr_t, const ChessPiece &piece) { return false; };
bool operator!=(std::nullptr_t, const ChessPiece &piece) { return true; };