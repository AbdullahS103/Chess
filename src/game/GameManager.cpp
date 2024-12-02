#include "GameManager.h"
#include "BoardStateGenerator.h"
#include "King.h"
#include "Knight.h"


GameManager::GameManager() {
  this->board = Board();
  BoardStateGenerator::standardBoard(this->board);

  this->whiteKingIndex = getKingIndex(TeamColors::WHITE);
  this->blackKingIndex = getKingIndex(TeamColors::BLACK);
  this->specialPieces.push_back(std::make_unique<Knight>(TeamColors::WHITE));
}

GameManager::~GameManager() {}

bool GameManager::inCheck(TeamColors team) const {
  int kingIndex = (team == TeamColors::WHITE) ? whiteKingIndex : blackKingIndex;

  // Iterate through the pieces that can jump other pieces to see if King is in danger
  for (const auto &piece : specialPieces) {
    for (int index : piece.get()->getAllPossibleMoves(board, kingIndex)){
      if (board.board[index] == nullptr)
        continue;
      if (typeid(board.board[index]) != typeid(*piece.get()))
        continue;
      if (board.board[index]->isSameTeam(team))
        continue;
      return true;
    }
  }

  // int rowOffset[] = {1, 1, -1, -1, 1, -1, 0, 0};
  // int colOffset[] = {1, -1, 1, -1, 0, 0, 1, -1};

  // // Iterate through all 8 directions queen can go
  // for (int i = 0; i < 8; i++) {
  //   int newRow = row + rowOffset[i];
  //   int newCol = col + colOffset[i];

  //   while (board.isOnBoard(newRow, newCol)) { 
  //     // If a piece is in the way, can stop searching early
  //     ChessPiece *currentPiece = board.getPiece(newRow, newCol);
  //     if (currentPiece != nullptr) {
  //       if (!currentPiece->isSameTeam(this->color))
  //         validMoves.insert(board.getIndex(newRow, newCol));
  //       break;
  //     }
        
  //     validMoves.insert(board.getIndex(newRow, newCol));
  //     newRow += rowOffset[i];
  //     newCol += colOffset[i];
  //   }
  // }
  return false;
}

int GameManager::getKingIndex(TeamColors team) {
  for (int i = 0; i < board.boardSize; i++) {
    if (this->board.board[i] == nullptr)
      continue;
    if (typeid(*this->board.board[i]) != typeid(King))
      continue;
    if (!this->board.board[i]->isSameTeam(team))
      continue;
    return i;
  }
  return -1;
}