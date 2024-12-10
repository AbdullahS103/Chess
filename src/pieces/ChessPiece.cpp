#include "ChessPiece.h"
#include "Errors.h"

ChessPiece::ChessPiece(TeamColors color, std::string symbol) { 
  this->color = color; 
  this->symbol = symbol;
}

std::ostream &operator<<(std::ostream &strm, const ChessPiece &piece) { 
  return strm << piece.getSymbol(); 
};

bool ChessPiece::isSameTeam(ChessPiece *piece) { 
  return (piece == nullptr ? false : color == piece->color); 
}

std::string ChessPiece::getSymbol() const { 
  return this->symbol; 
};

TeamColors ChessPiece::getColor() const {
  return this->color;
}

bool ChessPiece::isSameTeam(TeamColors color) { 
  return this->color == color;
}

bool ChessPiece::operator!=(const ChessPiece &piece) const { 
  return !(*this == piece);
};

bool ChessPiece::operator==(std::nullptr_t) const { 
  return false; 
};

bool ChessPiece::operator!=(std::nullptr_t) const { 
  return true; 
};

bool operator==(std::nullptr_t, const ChessPiece &piece) { 
  return false; 
};

bool operator!=(std::nullptr_t, const ChessPiece &piece) { 
  return true; 
};