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

    // Tracks the squares that are controlled by a team
  //  key   -> Square Index
  //  value -> Number of pieces that control that square (frequency)
  std::map<int, int> whiteControlledSpaces;
  std::map<int, int> blackControlledSpaces;
  
  // Quick lookups for pieces and the spaces each piece controls
  //  key   -> Chess piece
  //  value -> Squares controlled by specific piece
  std::map<ChessPiece*, std::unordered_set<int>> whitePieceControlMap;
  std::map<ChessPiece*, std::unordered_set<int>> blackPieceControlMap;

  // Map to store piece locations
  //  key   -> Chess piece
  //  value -> index
  std::map<ChessPiece*, int> whitePieceMap;
  std::map<ChessPiece*, int> blackPieceMap;
  
  std::vector<std::unique_ptr<SpecialPieces>> specialPieces;

public:
  GameManager();
  GameManager(int randomInteger);
  ~GameManager();

  // void movePiece(int fromRow, int fromCol, int toRow, int toCol);
  // int getBlackStrength() const;
  // int getWhiteStrength() const;
  bool inCheck(TeamColors team) const;
  bool inCheckmate(TeamColors team) const;
  int getKingIndex(TeamColors team);
};

#endif