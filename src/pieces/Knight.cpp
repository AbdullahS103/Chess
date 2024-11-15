#include "Knight.h"

Knight::Knight(TeamColors color) : ChessPiece(color){
  if (color == TeamColors::WHITE)
    this->symbol = "♘";
  else 
    this->symbol = "♞";
};
Knight::~Knight(){};

string Knight::getSymbol() const { return this->symbol; }

bool Knight::isValidMove(Board &board, int fromRow, int fromCol, int toRow, int toCol) { 
  int rowDiff = abs(fromRow - toRow);
  int colDiff = abs(fromCol - toCol);
  if (rowDiff == 2 && colDiff == 1)
    return true;
  if (rowDiff == 1 && colDiff == 2)
    return true;
  return false;
};

unordered_set<int> Knight::getAllValidMoves(Board &board, int row, int col) {
  std::unordered_set<int> validMoves;
  int rowOffset[] = {2, 1, -1, -2, -2, -1, 1, 2};
  int colOffset[] = {1, 2, 2, 1, -1, -2, -2, -1};
  for (int i = 0; i < 8; i++) 
  {
    int newRow = row + rowOffset[i];
    int newCol = col + colOffset[i];
    if (!board.isOnBoard(newRow, newCol))
      continue;

    ChessPiece *currentPiece = board.getPiece(newRow, newCol);
    if (currentPiece == nullptr || currentPiece->isSameTeam(this->color))
        validMoves.insert(board.getIndex(newRow, newCol));
  }
  return validMoves;
}
