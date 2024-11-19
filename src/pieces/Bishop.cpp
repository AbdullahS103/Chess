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
  if (!board.isOnBoard(fromRow, fromCol) || !board.isOnBoard(toRow, toCol))
    return false;
  // Move should not collide with piece of same team
  ChessPiece *currentPiece = board.getPiece(toRow, toCol);
  if (currentPiece != nullptr && currentPiece->isSameTeam(this->color))
    return false;

  int rowDiff = abs(fromRow - toRow);
  int colDiff = abs(fromCol - toCol);
  // Move is not diagonal
  if (rowDiff != colDiff || rowDiff == 0)
    return false;

  int rowUnitNorm = (toRow - fromRow) / rowDiff; 
  int colUnitNorm = (toCol - fromCol) / colDiff;

  // Search in the direction of the movement
  int newRow = fromRow + rowUnitNorm;
  int newCol = fromCol + colUnitNorm;
  while (newRow != toRow || newCol != toCol) {
    // If you see a piece before destination, move is not valid (Bishop cannot jump a piece)
    if (board.getPiece(newRow, newCol) != nullptr)
      return false;
    newRow += rowUnitNorm;
    newCol += colUnitNorm;
  }

  return true;
};

std::unordered_set<int> Bishop::getAllValidMoves(Board &board, int row, int col) {
  std::unordered_set<int> validMoves;
  int rowOffset[] = {1, 1, -1, -1};
  int colOffset[] = {1, -1, 1, -1};

  // Iterate through all 4 diagonal directions
  for (int i = 0; i < 4; i++) {
    int newRow = row + rowOffset[i];
    int newCol = col + colOffset[i];

    while (board.isOnBoard(newRow, newCol)) { 
      // If a piece is in the way, can stop searching early
      ChessPiece *currentPiece = board.getPiece(newRow, newCol);
      if (currentPiece != nullptr) {
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

bool Bishop::operator==(const ChessPiece &piece) const {
  if (this == &piece)
    return true;
  if (typeid(piece) != typeid(Bishop))
    return false;

  const Bishop *rook = static_cast<const Bishop*>(&piece);
  return rook->color == color && rook->symbol == symbol;
};
bool Bishop::operator!=(const ChessPiece &piece) const { return !(*this == piece); };