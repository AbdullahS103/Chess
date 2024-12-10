#include "GameManager.h"
#include "BoardStateGenerator.h"
#include "King.h"
#include "Errors.h"

GameManager::GameManager() {
  this->board = new Board();
  this->emptyBoard = new Board();
  BoardStateGenerator::standardBoard(*this->board);
  intializeMemberVariables();
}

GameManager::GameManager(FENManager fen) {
  this->board = new Board();
  this->emptyBoard = new Board();

  // Initialize the board based on FEN
  BoardStateGenerator::FENBoard(*this->board, fen.getLayout());
  
  // Iterate through board to initialize variables to correctly track board state
  intializeMemberVariables();
}

GameManager::~GameManager() {
  delete board;
  delete emptyBoard;
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

  for (int i = 0; i < board->getBoardSize(); i++) {
    whiteControlledSpaces[i] = 0;
    blackControlledSpaces[i] = 0;
    pieceMap[i] = nullptr;
  }

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
    pieceControlMap[piece] = controlSquares;
      
    for (const int index : controlSquares) {
      if (color == TeamColors::WHITE)
        whiteControlledSpaces[index]++;
      else
        blackControlledSpaces[index]++;
    }

    // Track pieces that can jump seperatly
    JumpType* specialPiece = dynamic_cast<JumpType*>(piece);
    if (specialPiece) 
      jumpers[piece] = i;
  }
}

bool GameManager::inCheck(TeamColors team) {
  if (team == TeamColors::WHITE)
    return this->blackControlledSpaces[whiteKingIndex] > 0;
  else 
    return this->whiteControlledSpaces[blackKingIndex] > 0;
}

bool GameManager::inCheckmate(TeamColors team) {
  std::cout << *board << std::endl;
  if (!inCheck(team))
    return false;

  int kingIndex = (team == TeamColors::WHITE ? whiteKingIndex : blackKingIndex);
  ChessPiece *king = pieceMap[kingIndex];
  std::map<int, int> enemyControlledSpaces = (team == TeamColors::WHITE ? blackControlledSpaces : whiteControlledSpaces);

  // Check if king can move anywhere that is safe
  for (const int index : pieceControlMap[king]) {
    ChessPiece *piece = board->getPiece(index);
    if (piece && piece->isSameTeam(team))
      continue;
    if (enemyControlledSpaces[index] == 0)
      return false;
  }

  // If there are multiple threats found at this point, king is in checkmate,
  // since king has no where to move that is safe, and you cant block two threats in one move
  if (enemyControlledSpaces[kingIndex] > 1)
    return true;
  
  // Checks for threats from pieces that can jump other pieces
  ChessPiece *jumpThreat = nullptr;
  int jumpThreatIndex = -1;
  for (auto it = jumpers.begin(); it != jumpers.end(); it++) {
    if (it->first->isSameTeam(team))
      continue;
    // Logic to figure out if jump piece threatens king
    if (pieceControlMap[it->first].count(kingIndex) > 0) {
      jumpThreat = it->first;
      jumpThreatIndex=it->second;
    }
  }

  // If player can capture the jump threat with a piece not pinned to king, king is not in checkmate
  if (jumpThreat) {
    for (auto it = pieceControlMap.begin(); it != pieceControlMap.end(); it++) {
      if (it->first->getColor() != king->getColor())
        continue;
      if (it->second.count(jumpThreatIndex) > 0 && !isPiecePinnedToKing(kingIndex, jumpThreatIndex))
        return false; 
    }
    return true;
  }

  // Iterate through all 8 directions king threat can exist (line of sight threat)
  int rowOffset[] = {1, 1, -1, -1, 1, -1, 0, 0};
  int colOffset[] = {1, -1, 1, -1, 0, 0, 1, -1};
  int directionalThreatDirection = -1;
  int directionalThreatIndex = -1;
  int row = board->getRow(kingIndex);
  int col = board->getColumn(kingIndex);

  for (int i = 0; i < 8; i++) {
    int newRow = row + rowOffset[i];
    int newCol = col + colOffset[i];

    while (board->isOnBoard(newRow, newCol)) { 
      ChessPiece *currentPiece = board->getPiece(newRow, newCol);
      // If nullptr, no piece is here, continue search
      if (!currentPiece) {
        newRow += rowOffset[i];
        newCol += colOffset[i];
        continue;
      }
      // Check if found piece threatens capture of the king
      if (!currentPiece->isValidMove(*board, newRow, newCol, row, col)) 
        break;
      directionalThreatDirection = i;
      directionalThreatIndex = board->getIndex(newRow, newCol);
      break;
    }
  }

  // Check if player can either capture piece threatening capture of king or block it
  row += rowOffset[directionalThreatDirection];
  col += colOffset[directionalThreatDirection];
  int index = board->getIndex(row, col);
  while (index != directionalThreatIndex) {
    for (auto it = pieceControlMap.begin(); it != pieceControlMap.end(); it++) {
      // Skip king and pieces of opposite team, cannot be used to capture/block threat
      // (King has already been proven to be stuck and unable to move at this point)
      if (*it->first == *king || it->first->getColor() != king->getColor())
        continue;
      if (it->second.count(index) > 0 && !isPiecePinnedToKing(kingIndex, index))
        return false;
    }
    // If a piece is found, it is guarenteed to be enemy piece giving check
    // At this point, it is concluded player cannot capture said piece or block it, can 
    // break out of loop
    if (board->getPiece(board->getIndex(row, col)))
      break;
    row += rowOffset[directionalThreatDirection];
    col += colOffset[directionalThreatDirection];
    index = board->getIndex(row, col);
  }
  return true;
}

bool GameManager::isPiecePinnedToKing(int kingIndex, int pieceIndex) {
  int kingRow = board->getRow(kingIndex);
  int kingCol = board->getColumn(kingIndex);
  int pieceRow = board->getRow(pieceIndex);
  int pieceCol = board->getColumn(pieceIndex);

  int rowDiff = abs(kingRow - pieceRow);
  int colDiff = abs(kingCol - pieceCol);

  // Move is not horizontal, vertical, or diagonal
  if ((kingRow != pieceRow && kingCol != pieceCol) && (rowDiff != colDiff || rowDiff == 0))
    return false;

  int rowUnitNorm = pieceRow - kingRow; 
  int colUnitNorm = pieceCol - kingCol;
  if (rowDiff > 0)
    rowUnitNorm /= rowDiff;
  if (colDiff > 0)
    colUnitNorm /= colDiff;

  // Search past the piece, relative to its direction from the king
  int newRow = pieceRow + rowUnitNorm;
  int newCol = pieceCol + colUnitNorm;
  while (board->isOnBoard(newRow, newCol)) {
    ChessPiece *piece = board->getPiece(newRow, newCol);
    if (piece) 
      return piece->isValidMove(*emptyBoard, newRow, newCol, kingRow, kingCol);
    newRow += rowUnitNorm;
    newCol += colUnitNorm;   
  }
  return false;
}

void GameManager::printJumpers() const {
  std::ostringstream oss; // String stream to build the string
  oss << "[";
  for (auto it = jumpers.begin(); it != jumpers.end(); ++it)  {
    oss << it->first->getSymbol() << " : " << std::to_string(it->second);
    if (std::next(it) != jumpers.end()) { 
      oss << " , ";
    }
  }
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
    if (std::next(it) != pieceMap.end()) { 
      oss << " , ";
    }
  }
  oss << " }";
  std::cout << oss.str() << std::endl;
}

void GameManager::printControlSpaces(TeamColors team) const {
  std::map<int, int> map = (team == TeamColors::WHITE ? whiteControlledSpaces : blackControlledSpaces);
  std::ostringstream oss;
  oss << "{";
  for (auto it = map.begin(); it != map.end(); ++it) {
    oss << std::to_string(it->first) << ": " << std::to_string(it->second);
    if (std::next(it) != map.end()) { // Add a comma between key-value pairs, but not after the last one
      oss << ", ";
    }
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