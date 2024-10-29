#include "Bishop.h"

Bishop::Bishop(TeamColors color) : ChessPiece(color){};
Bishop::~Bishop(){};

string Bishop::getIdentifier() const { return "B"; }

bool Bishop::isValidMove(Board &board, int fromRow, int fromCol, int toRow, int toCol) { 
  int rowDiff = abs(fromRow - toRow);
  int colDiff = abs(fromCol - toCol);
  return rowDiff == colDiff && rowDiff != 0;
};

unordered_set<int> Bishop::getAllValidMoves(Board &board, int row, int col) {
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
      if (currentPiece == nullptr || currentPiece->isSameTeam(this->color))
        validMoves.insert(board.getIndex(newRow, newCol));
    }
  }
  return validMoves;
}
