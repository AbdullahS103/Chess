#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include "CommonHeaders.h"
#include "Board.h"
#include "SpecialPieces.h"

class Board;

// GameManager class handles getting information on Board state, chess moves, player strength calculations, etc.
class GameManager {
  Board *board;
  int whiteKingIndex;
  int blackKingIndex;
  std::vector<std::unique_ptr<SpecialPieces>> specialPieces;

public:
  GameManager();
  ~GameManager();

  // void movePiece(int fromRow, int fromCol, int toRow, int toCol);
  // int getBlackStrength() const;
  // int getWhiteStrength() const;
  bool inCheck(TeamColors team) const;
  int getKingIndex(TeamColors team);
};

#endif