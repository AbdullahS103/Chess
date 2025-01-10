#include "Knight.h"

Knight::Knight(TeamColors color) : ChessPiece(color, (color == TeamColors::WHITE ? "♘" : "♞"), "Knight", true){};

bool Knight::isValidMove(Board &board, int fromRow, int fromCol, int toRow, int toCol) { 
  if (!board.isOnBoard(fromRow, fromCol) || !board.isOnBoard(toRow, toCol))
    return false;

  ChessPiece *currentPiece = board.getPiece(toRow, toCol);
  if (currentPiece && currentPiece->isSameTeam(this->color))
    return false;

  int rowDiff = abs(fromRow - toRow);
  int colDiff = abs(fromCol - toCol);
  if (rowDiff == 2 && colDiff == 1)
    return true;
  if (rowDiff == 1 && colDiff == 2)
    return true;
  return false;
};

std::unordered_set<int> Knight::getAllValidMoves(Board &board, int row, int col) {
  std::unordered_set<int> validMoves;
  int rowOffset[] = {2, 1, -1, -2, -2, -1, 1, 2};
  int colOffset[] = {1, 2, 2, 1, -1, -2, -2, -1};
  for (int i = 0; i < 8; i++) {
    int newRow = row + rowOffset[i];
    int newCol = col + colOffset[i];
    if (isValidMove(board, row, col, newRow, newCol))
      validMoves.insert(board.getIndex(newRow, newCol));
  }
  return validMoves;
}

std::unordered_set<int> Knight::getAllControlSquares(Board &board, int row, int col) {
  std::unordered_set<int> controlSquares;
  int rowOffset[] = {2, 1, -1, -2, -2, -1, 1, 2};
  int colOffset[] = {1, 2, 2, 1, -1, -2, -2, -1};
  for (int i =0; i < 8; i++) {
    int newRow = row + rowOffset[i];
    int newCol = col + colOffset[i];
    if (board.isOnBoard(newRow, newCol))
      controlSquares.insert(board.getIndex(newRow, newCol));
  }
  return controlSquares;
}

bool Knight::operator==(const ChessPiece &piece) const {
  if (this == &piece)
    return true;
  if (typeid(piece) != typeid(Knight))
    return false;

  const Knight *knight = static_cast<const Knight*>(&piece);
  return knight->color == color && knight->symbol == symbol;
};