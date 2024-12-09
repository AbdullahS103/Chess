#include "GameManager.h"
#include "BoardStateGenerator.h"
#include "FENManager.h"
#include "King.h"
#include "Errors.h"

GameManager::GameManager() {
  this->board = new Board();
  this->emptyBoard = new Board();
  BoardStateGenerator::standardBoard(*this->board);
  intializeMemberVariables();
}

GameManager::GameManager(std::string fenString) {
  this->board = new Board();
  this->emptyBoard = new Board();

  // Validate the FEN string and initialize the board based on FEN
  if (!FENManager::isValidFEN(fenString)) 
    throw new InvalidInputException("ERROR: Invalid FEN");
  BoardStateGenerator::FENBoard(*this->board, FENManager::getBoardLayout(fenString));
  std::cout << *board << std::endl;
  
  // Iterate through board to initialize variables to correctly track board state
  intializeMemberVariables();
}

GameManager::~GameManager() {
  delete board;
  delete emptyBoard;
}

bool GameManager::isValidFEN(std::string &fenString) {
  return true;
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
    whitePieceMap[i] = nullptr;
    blackPieceMap[i] = nullptr;
  }

  for (int i = 0; i < board->getBoardSize(); i++) {
    ChessPiece *piece = board->getPiece(i);
    if (!piece) 
      continue;
    
    int row = board->getRow(i);
    int col = board->getColumn(i);
    TeamColors color = piece->getColor();
    std::unordered_set<int> controlSquares = piece->getAllControlSquares(*board, row, col);

    if (color == TeamColors::WHITE) {
      whitePieceControlMap[piece] = controlSquares;
      whitePieceMap[i] = piece;
    } else {
      blackPieceControlMap[piece] = controlSquares;
      blackPieceMap[i] = piece;
    }
      
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
    return this->blackControlledSpaces[blackKingIndex] > 0;
  else 
    return this->whiteControlledSpaces[whiteKingIndex] > 0;
}

bool GameManager::inCheckmate(TeamColors team) {
  std::cout << *board << std::endl;
  if (!inCheck(team))
    return false;

  ChessPiece *king;
  int kingIndex;
  std::map<int, int> enemyControlledSpaces;
  std::map<ChessPiece*, std::unordered_set<int>> enemyPieceControlMap;
  std::map<ChessPiece*, std::unordered_set<int>> allyPieceControlMap;

  if (team == TeamColors::WHITE) {
    king = whitePieceMap[whiteKingIndex];
    kingIndex = whiteKingIndex;
    enemyControlledSpaces = blackControlledSpaces;
    enemyPieceControlMap = blackPieceControlMap;
    allyPieceControlMap = whitePieceControlMap;
  } else {
    king = blackPieceMap[blackKingIndex];
    kingIndex = blackKingIndex;
    enemyControlledSpaces = whiteControlledSpaces;
    enemyPieceControlMap = whitePieceControlMap;
    allyPieceControlMap = blackPieceControlMap;
  }

  // Check if king can move anywhere that is safe
  for (const int index : allyPieceControlMap[king]) {
    ChessPiece *piece = board->getPiece(index);
    if (piece && piece->isSameTeam(team))
      continue;
    if (enemyControlledSpaces[index] == 0)
      return false;
  }

  // Begin looking for number of immediate threats to the king
  // If there are multiple threats found at this point, king is in checkmate
  int numberOfThreats = 0;
  
  // Checks for threats from pieces that can jump other pieces
  ChessPiece *jumpThreat = nullptr;
  int jumpThreatIndex = -1;
  for (auto it = jumpers.begin(); it != jumpers.end(); it++) {
    if (it->first->isSameTeam(team))
      continue;
    // Logic to figure out if jump piece threatens king
    if (enemyPieceControlMap[it->first].count(kingIndex) > 0) {
      jumpThreat = it->first;
      jumpThreatIndex=it->second;
      if (++numberOfThreats > 1)
        return true;
    }
  }

  // Iterate through all 8 directions king threat can exist
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
      if (++numberOfThreats > 1)
        return true;
      break;
    }
  }

  // If player can capture the jump threat with a piece not pinned to king, king is not in checkmate
  if (jumpThreat) {
    for (auto it = allyPieceControlMap.begin(); it != allyPieceControlMap.end(); it++) 
      if (it->second.count(jumpThreatIndex) > 0 && !isPiecePinnedToKing(kingIndex, jumpThreatIndex))
        return false; 
    return true;
  }

  // Check if player can either capture piece threatening capture of king or block it
  row += rowOffset[directionalThreatDirection];
  col += colOffset[directionalThreatDirection];
  int index = board->getIndex(row, col);
  while (index != directionalThreatIndex) {
    for (auto it = allyPieceControlMap.begin(); it != allyPieceControlMap.end(); it++) {
      if (*it->first == *king)
        continue;
      if (it->second.count(index) > 0 && !isPiecePinnedToKing(kingIndex, index))
        return false;
    }
    // If a piece is found, it is guarenteed to be enemy piece giving check
    // At this point, it is concluded player cannot capture said piece or block it, can 
    // break out of loop early
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

void GameManager::printSpecialPieces() const {
  std::ostringstream oss; // String stream to build the string
  oss << "[";
  for (auto it = jumpers.begin(); it != jumpers.end(); ++it)  {
    oss << it->first->getSymbol() << " ";
    if (std::next(it) != jumpers.end()) { // Add a comma between elements, but not after the last one
      oss << ", ";
    }
  }
  oss << "]";
  std::cout << oss.str() << std::endl;
}

void GameManager::printPieceMap(TeamColors team) const {
  std::map<int, ChessPiece*> map = (team == TeamColors::WHITE ? whitePieceMap : blackPieceMap);
  std::ostringstream oss;
  oss << "{";
  for (auto it = map.begin(); it != map.end(); ++it) {
    oss << std::to_string(it->first) << ": " << it->second->getSymbol();
    if (std::next(it) != map.end()) { // Add a comma between key-value pairs, but not after the last one
      oss << ", ";
    }
  }
  oss << "}";
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
  std::map<ChessPiece*, std::unordered_set<int>> map = (team == TeamColors::WHITE ? whitePieceControlMap : blackPieceControlMap);
  std::ostringstream oss;
  oss << "{";
  for (auto it = map.begin(); it != map.end(); ++it) {
    oss << *it->first;
    for (int index : it->second) {
      oss << " " << std::to_string(index) << " , ";
    }
    oss << std::endl;
  }
  oss << "}";
  std::cout << oss.str() << std::endl;
}