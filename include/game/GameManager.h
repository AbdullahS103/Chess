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
  int whiteKingIndex;
  int blackKingIndex;

  // Enpassant specific variables
  ChessPiece* enpassantPiece;

  // Castling specific variables
  bool whiteQueenSideCastling;
  bool whiteKingSideCastling;
  bool blackQueenSideCastling;
  bool blackKingSideCastling;
  ChessPiece* blackQueenSideRook;
  ChessPiece* blackKingSideRook;
  ChessPiece* whiteQueenSideRook;
  ChessPiece* whiteKingSideRook;

  std::unordered_set<std::string> promotionTargets;

  // Tracks the squares that are controlled by a team
  //   key   -> Square Index
  //   value -> Pieces that control that square (white)
  std::map<int, std::unordered_set<ChessPiece*>> whiteControlledSpaces;

  // Tracks the squares that are controlled by a team
  //   key   -> Square Index
  //   value -> Pieces that control that square (black)
  std::map<int, std::unordered_set<ChessPiece*>> blackControlledSpaces;
  
  // Quick lookups for pieces and the spaces each piece controls
  //   key   -> Chess piece
  //   value -> Squares controlled by specific piece
  std::map<ChessPiece*, std::unordered_set<int>> pieceControlMap;

  // Map to store piece locations
  //   key   -> index
  //   value -> Chess piece
  std::map<int, ChessPiece*> pieceMap;

  // Reverse of piece map
  //   key   -> Chess piece
  //   value -> index
  std::map<ChessPiece*, int> reversePieceMap;
  
  // Individually track pieces that can jump other pieces (ie. Knight)
  //   key -> Chess piece
  std::unordered_set<ChessPiece*> jumpers;

  int getKingIndex(TeamColors team);

  void intializeMemberVariables();

  void updateMemberVariables(ChessPiece *piece, int toRow, int toCol);

  void updateControlSquares(ChessPiece *piece);

  bool isPiecePinnedToKing(int kingIndex, int pieceIndex);
  
  bool evaluateCheckmateByJump(ChessPiece* threat);

  bool evaluateCheckmateByLineOfSight(ChessPiece* threat);

  void executePromotionRules(ChessPiece *pawn, int toRow, int toCol);

  void executeEnpassantRules(ChessPiece *pawn, int toRow, int toCol);

  void executeEnpassantMovement(ChessPiece *pawn, int toRow, int toCol);

  void executeCastlingRules(ChessPiece *king, int toRow, int toCol);

  void disableKingCastling(TeamColors color);

  // Meant for debugging data structures, will get deleted on final build
  void printPieceMap(TeamColors team) const;

  void printControlSpaces(TeamColors team) const;

  void printTeamControlMap(TeamColors team) const;

  void printJumpers() const;

public:
  GameManager();

  GameManager(std::string fen);

  ~GameManager();

  void movePiece(int fromRow, int fromCol, int toRow, int toCol);

  // int getBlackStrength() const;
  // int getWhiteStrength() const;
  bool inCheck(TeamColors team);

  bool inCheckmate(TeamColors team);

  std::string getFENString() const;

  friend std::ostream &operator<<(std::ostream &strm, const GameManager &game);
};

#endif