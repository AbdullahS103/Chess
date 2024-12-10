#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include "CommonHeaders.h"
#include "Board.h"
#include "JumpType.h"
#include "FENManager.h"

class Board;

// GameManager class handles getting information on Board state, chess moves, player strength calculations, etc.
class GameManager {
  Board *board;
  Board *emptyBoard;
  int whiteKingIndex;
  int blackKingIndex;

  // Tracks the squares that are controlled by a team
  //   key   -> Square Index
  //   value -> Number of pieces that control that square (frequency)
  std::map<int, int> whiteControlledSpaces;
  std::map<int, int> blackControlledSpaces;
  
  // Quick lookups for pieces and the spaces each piece controls (can also merge maybe)
  //   key   -> Chess piece
  //   value -> Squares controlled by specific piece
  std::map<ChessPiece*, std::unordered_set<int>> pieceControlMap;

  // Map to store piece locations (May want to merge)
  //   key   -> index
  //   value -> Chess piece
  std::map<int, ChessPiece*> pieceMap;
  
  // Individually track pieces that can jump other pieces (ie. Knight)
  //   key   -> Chess piece
  //   value -> index
  std::map<ChessPiece*, int> jumpers;

  int getKingIndex(TeamColors team);

  void intializeMemberVariables();

  bool isPiecePinnedToKing(int kingIndex, int pieceIndex);
  
  // Meant for debugging data structures, will get deleted on final build
  void printPieceMap(TeamColors team) const;

  void printControlSpaces(TeamColors team) const;

  void printTeamControlMap(TeamColors team) const;

  void printJumpers() const;

public:
  GameManager();

  GameManager(FENManager fen);

  ~GameManager();

  // void movePiece(int fromRow, int fromCol, int toRow, int toCol);
  // int getBlackStrength() const;
  // int getWhiteStrength() const;
  bool inCheck(TeamColors team);

  bool inCheckmate(TeamColors team);
};

#endif