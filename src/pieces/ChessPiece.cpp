#include "ChessPiece.h"
#include "Errors.h"

ChessPiece::ChessPiece(TeamColors color, std::string symbol, std::string name, bool isPromotable, char fen) { 
  this->color = color; 
  this->symbol = symbol;
  this->name = name;
  this->isPromotable = isPromotable;
  this->fen = fen;
}

std::ostream &operator<<(std::ostream &strm, const ChessPiece &piece) { 
  return strm << piece.getSymbol(); 
};

bool ChessPiece::isSameTeam(ChessPiece *piece) { 
  return (piece == nullptr ? false : color == piece->color); 
}

std::string ChessPiece::getName() const {
  return this->name;
}

bool ChessPiece::getPromotionStatus() const {
  return this->isPromotable;
}

std::string ChessPiece::getSymbol() const { 
  return this->symbol; 
};

TeamColors ChessPiece::getColor() const {
  return this->color;
}

char ChessPiece::getFENCharacter() const {
  return this->fen;
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