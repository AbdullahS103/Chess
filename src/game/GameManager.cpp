#include "GameManager.h"
#include "BoardStateGenerator.h"
#include "King.h"

GameManager::GameManager() {
  this->board = new Board();
  this->emptyBoard = new Board();
  BoardStateGenerator::standardBoard(*this->board);
  intializeMemberVariables();
}

GameManager::GameManager(int randomInteger) {
  this->board = new Board();
  this->emptyBoard = new Board();
  BoardStateGenerator::testBoard(*this->board);
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
        jumpers.push_back(piece);
  }
}

bool GameManager::inCheck(TeamColors team) {
  if (team == TeamColors::WHITE)
    return this->blackControlledSpaces[blackKingIndex] > 0;
  else 
    return this->whiteControlledSpaces[whiteKingIndex] > 0;
}

bool GameManager::inCheckmate(TeamColors team) {
  if (!inCheck(team))
    return false;

  ChessPiece *king;
  int kingIndex;
  std::unordered_set<int> kingControlSquares;
  std::map<int, int> enemyControlledSpaces;
  std::map<ChessPiece*, std::unordered_set<int>> enemyPieceControlMap;

  if (team == TeamColors::WHITE) {
    king = whitePieceMap[whiteKingIndex];
    kingIndex = whiteKingIndex;
    kingControlSquares = whitePieceControlMap[king];
    enemyControlledSpaces = blackControlledSpaces;
    enemyPieceControlMap = blackPieceControlMap;
  } else {
    king = blackPieceMap[blackKingIndex];
    kingIndex = blackKingIndex;
    kingControlSquares = blackPieceControlMap[king];
    enemyControlledSpaces = whiteControlledSpaces;
    enemyPieceControlMap = whitePieceControlMap;
  }

  // Check if king can move anywhere that is safe
  for (const int index : kingControlSquares) {
    ChessPiece *piece = board->getPiece(index);
    if (piece && piece->isSameTeam(team))
      continue;
    if (enemyControlledSpaces[index] == 0)
      return false;
  }

  // Begin looking for number of immediate threats to the king
  // If there are multiple threats found at this point, king is in checkmate
  int numberOfThreats = 0;
  ChessPiece *jumpThreat = nullptr;
  int directionalThreat = -1;

  // Checks for threats from pieces that can jump other pieces
  for (ChessPiece* piece : jumpers) {
    if (piece->isSameTeam(team))
      continue;
    // Logic to figure out if jump piece threatens king
    if (enemyPieceControlMap[piece].count(kingIndex) > 0) {
      jumpThreat = piece;
      if (++numberOfThreats > 1)
        return true;
    }
  }

  // Iterate through all 8 directions king threat can exist
  int rowOffset[] = {1, 1, -1, -1, 1, -1, 0, 0};
  int colOffset[] = {1, -1, 1, -1, 0, 0, 1, -1};
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
      directionalThreat = i;
      if (++numberOfThreats > 1)
        return true;
      break;
    }
  }

  // TODO: Complete checkmate function 

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
  for (size_t i = 0; i < jumpers.size(); ++i) {
      oss << jumpers[i]->getSymbol() << " ";
      if (i != jumpers.size() - 1) { // Add a comma between elements, but not after the last one
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

void GameManager::printControlMap(TeamColors team) const {
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