#include "GameManager.h"
#include "BoardStateGenerator.h"
#include "Errors.h"

#include "King.h"
#include "Pawn.h"

GameManager::GameManager() {
  this->board = new Board();
  BoardStateGenerator::standardBoard(*this->board);
  intializeMemberVariables();
}

GameManager::GameManager(FENManager fen) {
  this->board = new Board();

  // Initialize the board based on FEN
  BoardStateGenerator::FENBoard(*this->board, fen.getLayout());
  intializeMemberVariables();
}

GameManager::~GameManager() {
  delete board;
}

int GameManager::getKingIndex(TeamColors team) {
  for (int i = 0; i < board->getBoardSize(); i++) {
    ChessPiece *piece = board->getPiece(i);
    if (!piece)
      continue;
    if (typeid(*piece) != typeid(King))
      continue;
    if (!piece->isSameTeam(team))
      continue;
    return i;
  }
  return -1;
}

void GameManager::intializeMemberVariables() {
  this->whiteKingIndex = getKingIndex(TeamColors::WHITE);
  this->blackKingIndex = getKingIndex(TeamColors::BLACK);

  for (int i = 0; i < board->getBoardSize(); i++) 
    pieceMap[i] = nullptr;

  for (int i = 0; i < board->getBoardSize(); i++) {
    ChessPiece *piece = board->getPiece(i);
    if (!piece) 
      continue;
    
    int row = board->getRow(i);
    int col = board->getColumn(i);
    TeamColors color = piece->getColor();
    std::unordered_set<int> controlSquares = piece->getAllControlSquares(*board, row, col);

    // Populate data structures with piece location and piece control squares
    pieceMap[i] = piece;
    reversePieceMap[piece] = i;
    pieceControlMap[piece] = controlSquares;
      
    for (const int index : controlSquares) {
      if (color == TeamColors::WHITE)
        whiteControlledSpaces[index].insert(piece);
      else
        blackControlledSpaces[index].insert(piece);
    }

    // Track pieces that can jump seperatly
    JumpType* specialPiece = dynamic_cast<JumpType*>(piece);
    if (specialPiece) 
      jumpers.insert(piece);
  }
}

bool GameManager::inCheck(TeamColors team) {
  if (team == TeamColors::WHITE)
    return this->blackControlledSpaces[whiteKingIndex].size() > 0;
  else 
    return this->whiteControlledSpaces[blackKingIndex].size() > 0;
}

bool GameManager::inCheckmate(TeamColors team) {
  if (!inCheck(team))
    return false;

  int kingIndex = (team == TeamColors::WHITE ? whiteKingIndex : blackKingIndex);
  ChessPiece *king = pieceMap[kingIndex];
  std::map<int, std::unordered_set<ChessPiece*>> enemyControlledSpaces = 
      (team == TeamColors::WHITE ? blackControlledSpaces : whiteControlledSpaces);

  // Check if king can move anywhere that is safe
  for (const int index : pieceControlMap[king]) {
    ChessPiece *piece = board->getPiece(index);
    if (piece && piece->isSameTeam(team))
      continue;
    if (enemyControlledSpaces[index].size() == 0)
      return false;
  }

  // If there are multiple threats found at this point, king is in checkmate,
  // since king has no where to move that is safe, and you cant block two threats in one move
  if (enemyControlledSpaces[kingIndex].size() > 1)
    return true;
  
  ChessPiece* kingThreat = *enemyControlledSpaces[kingIndex].begin();
  if (jumpers.count(kingThreat) > 0)
    return evaluateCheckmateByJump(kingThreat);
  
  return evaluateCheckmateByLineOfSight(kingThreat);
}

bool GameManager::evaluateCheckmateByJump(ChessPiece* threat) {
  int kingIndex = (threat->getColor() == TeamColors::WHITE ? blackKingIndex : whiteKingIndex);
  int threatIndex = reversePieceMap[threat];
  std::unordered_set<ChessPiece*> potentialAllies = 
      (threat->getColor() == TeamColors::WHITE ? blackControlledSpaces[threatIndex] : whiteControlledSpaces[threatIndex]);
  
  // If player can capture the jump threat with a piece not pinned to king, king is not in checkmate
  for (ChessPiece *piece : potentialAllies) 
    if (!isPiecePinnedToKing(kingIndex, threatIndex))
      return false; 
  return true;
}

bool GameManager::evaluateCheckmateByLineOfSight(ChessPiece* threat) {
  int kingIndex = (threat->getColor() == TeamColors::WHITE ? blackKingIndex : whiteKingIndex);
  int threatIndex = reversePieceMap[threat];

  int row = board->getRow(kingIndex);
  int col = board->getColumn(kingIndex);
  int pieceRow = board->getRow(threatIndex);
  int pieceCol = board->getColumn(threatIndex);

  int rowUnitNorm = pieceRow - row; 
  int colUnitNorm = pieceCol - col;
  if (rowUnitNorm != 0)
    rowUnitNorm /= abs(rowUnitNorm);
  if (colUnitNorm != 0)
    colUnitNorm /= abs(colUnitNorm);

  row += rowUnitNorm;
  col += colUnitNorm;
  int index = board->getIndex(row, col);
  std::map<int, std::unordered_set<ChessPiece*>> potentialAllies = 
      (threat->getColor() == TeamColors::WHITE ? blackControlledSpaces : whiteControlledSpaces);

  // Check if player can either capture piece threatening capture of king or block it
  while (board->isOnBoard(row, col)) {
    // See if piece can move to that position, either blocking line of sight or capturing threat
    // Exclude king, since it has already checked that king cannot move to a safe location
    for (ChessPiece* piece : potentialAllies[index]) 
      if (piece != pieceMap[kingIndex] && !isPiecePinnedToKing(kingIndex, index))
        return false;
    
    // If a piece is found, it is guarenteed to be enemy piece giving check, which was found to 
    // be uncapturable due to pin, can break loop early
    if (board->getPiece(board->getIndex(row, col)))
      break;

    row += rowUnitNorm;
    col += colUnitNorm;
    index = board->getIndex(row, col);
  }
  return true;
}

bool GameManager::isPiecePinnedToKing(int kingIndex, int pieceIndex) {
  int kingRow = board->getRow(kingIndex);
  int kingCol = board->getColumn(kingIndex);
  int pieceRow = board->getRow(pieceIndex);
  int pieceCol = board->getColumn(pieceIndex);

  ChessPiece* piece = pieceMap[pieceIndex];
  bool isPinned = false;
  std::unordered_set<ChessPiece*> attackers = 
      (piece->getColor() == TeamColors::WHITE ? blackControlledSpaces[pieceIndex] : whiteControlledSpaces[pieceIndex]);
  
  board->setPiece(pieceRow, pieceCol, nullptr);
  for (ChessPiece* piece : attackers) {
    int attackerRow = board->getRow(reversePieceMap[piece]);
    int attackerCol = board->getColumn(reversePieceMap[piece]);
    isPinned = piece->isValidMove(*board, attackerRow, attackerCol, kingRow, kingCol);
    if (isPinned)
      break;
  }

  board->setPiece(pieceRow, pieceCol, piece);
  return isPinned;
}

void GameManager::movePiece(int fromRow, int fromCol, int toRow, int toCol) {
  // Initial checks to validate move 
  ChessPiece *fromPiece = board->getPiece(fromRow, fromCol);
  if (fromPiece == nullptr)
    throw InvalidMoveException("ERROR: No piece at selected position");

  ChessPiece *toPiece = board->getPiece(toRow, toCol);
  if (fromPiece->isSameTeam(toPiece))
    throw InvalidMoveException("ERROR: Move intersects with another piece from the same team!");

  // Check if move is allowed to be made
  if (!fromPiece->isValidMove(*board, fromRow, fromCol, toRow, toCol)) {
    if (typeid(fromPiece) == typeid(Pawn) && 
      (fromPiece->getColor() == TeamColors::WHITE && toRow == 0) || 
      (fromPiece->getColor() == TeamColors::BLACK && toRow == board->getRows() - 1))
      return executePromotionRules(fromPiece, toRow, toCol);
    else if (typeid(fromPiece) == typeid(Pawn))
      return executeEnpassantRules(fromPiece, toRow, toCol);
    else if (typeid(fromPiece) == typeid(King))
      return executeCastlingRules(fromPiece, toRow, toCol);
    else 
      throw InvalidMoveException("ERROR: Invalid move");
  }
    
  // check if King would be safe after move
  TeamColors team = fromPiece->getColor();
  int kingIndex = (team == TeamColors::WHITE ? whiteKingIndex : blackKingIndex);
  int pieceIndex = board->getIndex(fromRow, fromCol);
  bool isPinned = isPiecePinnedToKing(kingIndex, pieceIndex);
  bool isCheck = inCheck(team);
  if (isPinned && isCheck)
    throw InvalidMoveException("ERROR: Piece is pinned and cannot be moved until check resolves.");
    
}

void GameManager::executePromotionRules(ChessPiece *pawn, int toRow, int toCol) {

}

void GameManager::executeEnpassantRules(ChessPiece *pawn, int toRow, int toCol) {
  
}

void GameManager::executeCastlingRules(ChessPiece *king, int toRow, int toCol) {
  
}

void GameManager::printJumpers() const {
  std::ostringstream oss; // String stream to build the string
  oss << "[";
  for (ChessPiece* piece : jumpers)  
    oss << piece->getSymbol() << " , ";
  oss << " ]";
  std::cout << oss.str() << std::endl;
}

void GameManager::printPieceMap(TeamColors team) const {
  std::ostringstream oss;
  oss << "{";
  for (auto it = pieceMap.begin(); it != pieceMap.end(); ++it) {
    if (team != it->second->getColor())
      continue;
    oss << std::to_string(it->first) << ": " << (it->second ? it->second->getSymbol() : "NULL");
    if (std::next(it) != pieceMap.end()) 
      oss << " , ";
  }
  oss << " }";
  std::cout << oss.str() << std::endl;
}

void GameManager::printControlSpaces(TeamColors team) const {
  std::map<int, std::unordered_set<ChessPiece*>> map = (team == TeamColors::WHITE ? whiteControlledSpaces : blackControlledSpaces);
  std::ostringstream oss;
  oss << "{\n";
  for (auto it = map.begin(); it != map.end(); ++it) {
    oss << std::to_string(it->first) << ":";
    for (ChessPiece* piece : it->second) {
      oss << " " << piece << " , ";
    }
    oss << std::endl;
  }
  oss << "}";
  std::cout << oss.str() << std::endl;
}

void GameManager::printTeamControlMap(TeamColors team) const {
  std::ostringstream oss;
  oss << "{\n";
  for (auto it = pieceControlMap.begin(); it != pieceControlMap.end(); ++it) {
    if (team != it->first->getColor())
      continue;
    oss << *it->first << " : ";
    for (int index : it->second) {
      oss << " " << std::to_string(index) << ", ";
    }
    oss << std::endl;
  }
  oss << "}";
  std::cout << oss.str() << std::endl;
}