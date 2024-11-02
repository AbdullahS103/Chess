#include "Queen.h"

Queen::Queen(TeamColors color) : ChessPiece(color){
  if (color == TeamColors::WHITE)
    this->symbol = "♕";
  else 
    this->symbol = "♛";
};
Queen::~Queen(){};

string Queen::getSymbol() const { return this->symbol; }

bool Queen::isValidMove(Board &board, int fromRow, int fromCol, int toRow, int toCol) { 
  int rowDiff = abs(fromRow - toRow);
  int colDiff = abs(fromCol - toCol);
  return (rowDiff == 0 || colDiff == 0) || (rowDiff == colDiff && rowDiff != 0);
};

unordered_set<int> Queen::getAllValidMoves(Board &board, int row, int col) {
  std::unordered_set<int> validMoves;
  int rowOffset[] = {1, -1, 0, 0, 1, 1, -1, -1};
  int colOffset[] = {0, 0, 1, -1, 1, -1, 1, -1};
  for (int i = 0; i < 8; i++) 
  {
    while (true)
    {
      int newRow = row + rowOffset[i];
      int newCol = col + colOffset[i];
      if (!board.isOnBoard(newRow, newCol))
        break;
      
      ChessPiece *currentPiece = board.getPiece(newRow, newCol);
      if (currentPiece == nullptr) {
        validMoves.insert(board.getIndex(newRow, newCol));
      } else if (!currentPiece->isSameTeam(this->color)) {
        validMoves.insert(board.getIndex(newRow, newCol));
        break;
      } else {
        break;
      }
    }
  }
  return validMoves;
}