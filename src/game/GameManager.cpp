#include "GameManager.h"
#include "BoardStateGenerator.h"
#include "King.h"
#include "Knight.h"


GameManager::GameManager() {
  this->board = new Board();
  BoardStateGenerator::standardBoard(*this->board);

  this->whiteKingIndex = getKingIndex(TeamColors::WHITE);
  this->blackKingIndex = getKingIndex(TeamColors::BLACK);
  this->specialPieces.push_back(std::make_unique<Knight>(TeamColors::WHITE));
}

GameManager::GameManager(int randomInteger) {
  this->board = new Board();
  BoardStateGenerator::testBoard(*this->board);

  this->whiteKingIndex = getKingIndex(TeamColors::WHITE);
  this->blackKingIndex = getKingIndex(TeamColors::BLACK);
  this->specialPieces.push_back(std::make_unique<Knight>(TeamColors::WHITE));
}

GameManager::~GameManager() {
  delete board;
}

bool GameManager::inCheck(TeamColors team) const {
  int kingIndex = (team == TeamColors::WHITE) ? whiteKingIndex : blackKingIndex;

  // Iterate through the pieces that can jump other pieces to see if King is in danger
  for (const auto &piece : specialPieces) {
    for (int index : piece.get()->getAllPossibleMoves(*board, kingIndex)){
      ChessPiece *ptr = board->getPiece(index);
      if (!ptr)
        continue;
      // Workaround: Fixes a warning
      auto &piece_ref = *piece.get();
      if (typeid(*ptr) != typeid(piece_ref))
        continue;
      if (ptr->isSameTeam(team))
        continue;
      return true;
    }
  }

  int rowOffset[] = {1, 1, -1, -1, 1, -1, 0, 0};
  int colOffset[] = {1, -1, 1, -1, 0, 0, 1, -1};
  int kingRow = board->getRow(kingIndex);
  int kingCol = board->getColumn(kingIndex);

  // Iterate through all 8 directions to see if King is in check
  for (int i = 0; i < 8; i++) {
    int newRow = kingRow + rowOffset[i];
    int newCol = kingCol + colOffset[i];

    while (board->isOnBoard(newRow, newCol)) { 
      ChessPiece *currentPiece = board->getPiece(newRow, newCol);
      // If a piece is in the way, can stop searching early
      if (!currentPiece) {
        newRow += rowOffset[i];
        newCol += colOffset[i];
        continue;
      }
        
      if (currentPiece->isSameTeam(team)) 
        break;
      // If piece can move to king, king is in check
      if (currentPiece->isValidMove(*board, newRow, newCol, kingRow, kingCol))
        return true;

      newRow += rowOffset[i];
      newCol += colOffset[i];
    }
  }
  return false;
}

bool GameManager::inCheckmate(TeamColors team) const{
  if (!inCheck(team))
    return false;

  return true;
}

int GameManager::getKingIndex(TeamColors team) {
  for (int i = 0; i < board->getBoardSize(); i++) {
    ChessPiece *ptr = board->getPiece(i);
    if (!ptr)
      continue;
    if (typeid(*ptr) != typeid(King))
      continue;
    if (!ptr->isSameTeam(team))
      continue;
    return i;
  }
  return -1;
}