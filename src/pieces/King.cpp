#include "King.h"

King::King(TeamColors color) : ChessPiece(color){
  if (color ==  TeamColors::WHITE)
    this->symbol="♔";
  else 
    this->symbol="♚";
};
King::~King(){};

std::string King::getSymbol() const { return this->symbol; }

bool King::isValidMove(Board &board, int fromRow, int fromCol, int toRow, int toCol) { 
  if (!board.isOnBoard(fromRow, fromCol) || !board.isOnBoard(toRow, toCol))
    return false;
  
  ChessPiece *currentPiece = board.getPiece(toRow, toCol);
  if (currentPiece != nullptr && currentPiece->isSameTeam(this->color))
    return false;

  int rowDiff = abs(fromRow - toRow);
  int colDiff = abs(fromCol - toCol);
  return rowDiff <= 1 && colDiff <= 1 && !(rowDiff == 0 && colDiff == 0);
};

std::unordered_set<int> King::getAllValidMoves(Board &board, int row, int col) {
  std::unordered_set<int> validMoves;
  int rowOffset[] = {1, 1, 1, 0, 0, -1, -1, -1};
  int colOffset[] = {-1, 0, 1, -1, 1, -1, 0, 1};

  for (int i = 0; i < 8; i++) {
    int newRow = row + rowOffset[i];
    int newCol = col + colOffset[i];
    if (isValidMove(board, row, col, newRow, newCol))
      validMoves.insert(board.getIndex(newRow, newCol));
  }
  
  return validMoves;
}

bool King::operator==(const ChessPiece &piece) const {
  if (this == &piece)
    return true;
  if (typeid(piece) != typeid(King))
    return false;

  const King *king = static_cast<const King*>(&piece);
  return king->color == color && king->symbol == symbol;
};
bool King::operator!=(const ChessPiece &piece) const { return !(*this == piece); };