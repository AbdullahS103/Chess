#include "Bishop.h"

Bishop::Bishop(TeamColors color) : ChessPiece(color){
  if (color == TeamColors::WHITE)
    this->symbol = "♗";
  else 
    this->symbol = "♝";
};
Bishop::~Bishop(){};

std::string Bishop::getSymbol() const { return this->symbol; }

bool Bishop::isValidMove(Board &board, int fromRow, int fromCol, int toRow, int toCol) { 
  int rowDiff = abs(fromRow - toRow);
  int colDiff = abs(fromCol - toCol);
  return rowDiff == colDiff && rowDiff != 0;
};

std::unordered_set<int> Bishop::getAllValidMoves(Board &board, int row, int col) {
  std::unordered_set<int> validMoves;
  int rowOffset[] = {1, 1, -1, -1};
  int colOffset[] = {1, -1, 1, -1};
  for (int i = 0; i < 4; i++) 
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
