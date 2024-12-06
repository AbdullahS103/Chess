#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include "CommonHeaders.h"
#include "Board.h"
#include "JumpType.h"

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
  
  // Quick lookups for pieces and the spaces each piece controls
  //   key   -> Chess piece
  //   value -> Squares controlled by specific piece
  std::map<ChessPiece*, std::unordered_set<int>> whitePieceControlMap;
  std::map<ChessPiece*, std::unordered_set<int>> blackPieceControlMap;

  // Map to store piece locations
  //   key   -> index
  //   value -> Chess piece
  std::map<int, ChessPiece*> whitePieceMap;
  std::map<int, ChessPiece*> blackPieceMap;
  
  // Individually track pieces that can jump other pieces (ie. Knight)
  //   key   -> Chess piece
  //   value -> index
  std::map<ChessPiece*, int> jumpers;

  int getKingIndex(TeamColors team);

  void intializeMemberVariables();

  bool isPiecePinnedToKing(int kingIndex, int pieceIndex);

  bool isValidFEN(std::string &fenString);
  
  // Meant for debugging data structures, will get deleted on final build
  void printPieceMap(TeamColors team) const;

  void printControlMap(TeamColors team) const;

  void printSpecialPieces() const;

public:
  GameManager();

  GameManager(std::string fenString);

  ~GameManager();

  // void movePiece(int fromRow, int fromCol, int toRow, int toCol);
  // int getBlackStrength() const;
  // int getWhiteStrength() const;
  bool inCheck(TeamColors team);

  bool inCheckmate(TeamColors team);
};

#endif