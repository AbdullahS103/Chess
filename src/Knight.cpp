#include "ClassDeclarations.h"

Knight::Knight(TeamColors color) : ChessPiece(color){};
Knight::~Knight(){};

string Knight::getIdentifier() const { return "K"; }

bool Knight::isValidMove(Board &board, int fromIndex, int toIndex) { 
  int rowDiff = abs(board.getRow(fromIndex) - board.getRow(toIndex));
  int colDdiff = abs(board.getCol(fromIndex) - board.getCol(toIndex));
  if (rowDiff == 2 && colDdiff == 1)
    return true;
  if (rowDiff == 1 && colDdiff == 2)
    return true;
  return false;
};

unordered_set<int> Knight::getAllValidMoves(Board &board, int index) {
  std::unordered_set<int> validMoves;
  int row = board.getRow(index);
  int col = board.getCol(index);
  int rowOffset[] = {2, 1, -1, -2, -2, -1, 1, 2};
  int colOffset[] = {1, 2, 2, 1, -1, -2, -2, -1};
  for (int i = 0; i < 8; i++) 
    if (board.isOnBoard(row + rowOffset[i], col + colOffset[i]))
        validMoves.insert(board.getIndex(row + rowOffset[i], col + colOffset[i]));
  
  return validMoves;
}
