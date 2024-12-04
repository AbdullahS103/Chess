#include "Rook.h"

Rook::Rook(TeamColors color) : ChessPiece(color, (color == TeamColors::WHITE ? "♖" : "♜")){};

bool Rook::isValidMove(Board &board, int fromRow, int fromCol, int toRow, int toCol) { 
  if (!board.isOnBoard(fromRow, fromCol) || !board.isOnBoard(toRow, toCol))
    return false;
  // Move is not horizontal nor vertical
  if (fromRow != toRow && fromCol != toCol)
    return false;
  // Move should not collide with piece of same team
  ChessPiece *currentPiece = board.getPiece(toRow, toCol);
  if (currentPiece && currentPiece->isSameTeam(this->color))
    return false;

  int rowDiff = abs(fromRow - toRow);
  int colDiff = abs(fromCol - toCol);
  int rowUnitNorm = toRow - fromRow; 
  int colUnitNorm = toCol - fromCol;
  if (rowDiff > 0)
    rowUnitNorm /= rowDiff;
  if (colDiff > 0)
    colUnitNorm /= colDiff;

  // Search in the direction of the movement
  int newRow = fromRow + rowUnitNorm;
  int newCol = fromCol + colUnitNorm;
  while (newRow != toRow || newCol != toCol) {
    // If you see a piece before destination, move is not valid (Rook cannot jump a piece)
    if (board.getPiece(newRow, newCol) != nullptr)
      return false;
    newRow += rowUnitNorm;
    newCol += colUnitNorm;
  }

  return true;
};

std::unordered_set<int> Rook::getAllValidMoves(Board &board, int row, int col) {
  std::unordered_set<int> validMoves;
  int rowOffset[] = {1, -1, 0, 0};
  int colOffset[] = {0, 0, 1, -1};

  // Iterate through all 4 cardinal directions
  for (int i = 0; i < 4; i++) {
    int newRow = row + rowOffset[i];
    int newCol = col + colOffset[i];

    while (board.isOnBoard(newRow, newCol)) { 
      // If a piece is in the way, can stop searching early
      ChessPiece *currentPiece = board.getPiece(newRow, newCol);
      if (currentPiece) {
        if (!currentPiece->isSameTeam(this->color))
          validMoves.insert(board.getIndex(newRow, newCol));
        break;
      }
        
      validMoves.insert(board.getIndex(newRow, newCol));
      newRow += rowOffset[i];
      newCol += colOffset[i];
    }
  }
  return validMoves;
}

std::unordered_set<int> Rook::getAllControlSquares(Board &board, int row, int col) {
  std::unordered_set<int> controlSquares;
  int rowOffset[] = {1, -1, 0, 0};
  int colOffset[] = {0, 0, 1, -1};

  // Iterate through all 4 diagonal directions
  for (int i = 0; i < 4; i++) {
    int newRow = row + rowOffset[i];
    int newCol = col + colOffset[i];

    while (board.isOnBoard(newRow, newCol)) { 
      controlSquares.insert(board.getIndex(newRow, newCol));
      // If a piece is in the way, break early
      if (board.getPiece(newRow, newCol)) 
        break;
      newRow += rowOffset[i];
      newCol += colOffset[i];
    }
  }
  return controlSquares;
}

bool Rook::operator==(const ChessPiece &piece) const {
  if (this == &piece)
    return true;
  if (typeid(piece) != typeid(Rook))
    return false;

  const Rook *rook = static_cast<const Rook*>(&piece);
  return rook->color == color && rook->symbol == symbol;
};