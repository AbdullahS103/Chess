#include "Pawn.h"

Pawn::Pawn(TeamColors color) : ChessPiece(color){
  if (color == TeamColors::WHITE)
    this->symbol = "♙";
  else 
    this->symbol = "♟";
};
Pawn::~Pawn(){};

std::string Pawn::getSymbol() const { return this->symbol; }

bool Pawn::isValidMove(Board &board, int fromRow, int fromCol, int toRow, int toCol) { 
  if (!board.isOnBoard(fromRow, fromCol) || !board.isOnBoard(toRow, toCol))
    return false;

  ChessPiece *currentPiece = board.getPiece(toRow, toCol);
  if (currentPiece != nullptr && currentPiece->isSameTeam(this->color))
    return false;

  // TODO: En passant
  // Pawn cant move more than one column or zero/multiple rows ever
  if (abs(fromCol - toCol) > 1 || abs(fromRow - toRow) != 1)
    return false;
  if (this->color == TeamColors::WHITE && fromRow < toRow)
    return false;
  if (this->color == TeamColors::BLACK && fromRow > toRow)
    return false;

  // Pawn moving up one space
  if (fromCol == toCol)
    return currentPiece == nullptr;
  
  // Pawn capturing a piece on its diagonal
  return currentPiece != nullptr && !currentPiece->isSameTeam(this->color);
};

std::unordered_set<int> Pawn::getAllValidMoves(Board &board, int row, int col) {
  std::unordered_set<int> validMoves;
  int newRow = this->color == TeamColors::WHITE ? row - 1 : row + 1;
  int colOffset[] = {-1, 0, 1};

  for (int i = 0; i < 3; i++) {
    int newCol = col + colOffset[i];
    if (!board.isOnBoard(newRow, newCol))
      continue;

    ChessPiece *currentPiece = board.getPiece(newRow, newCol);
    // Moving forward
    if (newCol == col && currentPiece == nullptr)
      validMoves.insert(board.getIndex(newRow, newCol));
    // Capturing a piece on corner
    else if (newCol != col && currentPiece != nullptr && !currentPiece->isSameTeam(this->color))
      validMoves.insert(board.getIndex(newRow, newCol));
    // TODO: En passant
  }
  return validMoves;
}

bool Pawn::operator==(const ChessPiece &piece) const {
  if (this == &piece)
    return true;
  if (typeid(piece) != typeid(Pawn))
    return false;

  const Pawn *knight = static_cast<const Pawn*>(&piece);
  return knight->color == color && knight->symbol == symbol;
};
bool Pawn::operator!=(const ChessPiece &piece) const { return !(*this == piece); };