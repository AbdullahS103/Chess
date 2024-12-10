#include "FENManager.h"
#include "Errors.h"

FENManager::FENManager(std::string fenString) {
  if (!isValidFEN(fenString))
    throw new std::invalid_argument("ERROR: Invalid FEN");
  
  std::vector<std::string> fenSegments = split(fenString, ' ');
  layout = fenSegments[0];
  turn = fenSegments[1].at(0);
  castlingRights = fenSegments[2];
  enpassantRights = fenSegments[3];
  halfmoves = std::stoi(fenSegments[4]);
  fullmoves = std::stoi(fenSegments[5]);
};

FENManager::~FENManager() {};

bool FENManager::isValidFEN(std::string &fenString) {
  std::vector<std::string> fenSegments = split(fenString, ' ');
  if (fenSegments.size() != 6)
    return false;

  if (!isValidLayout(fenSegments[0]))
    return false;
  if (!isValidTurn(fenSegments[1]))
    return false;
  if (!isValidCastlingRights(fenSegments[2], fenSegments[0]))
    return false;
  if (!isValidEnpassantRights(fenSegments[3]))
    return false;
  if (!isValidHalfMoves(fenSegments[4]))
    return false; 
  if (!isValidFullMoves(fenSegments[5]))
    return false;

  return true;
}

bool FENManager::isValidLayout(std::string layout) {
  std::vector<std::string> rowSegments = split(layout, '/');
  if (rowSegments.size() == 0)
    return false;

  int colSize = -1;
  bool whiteKingFound = false;
  bool blackKingFound = false;

  for (std::string row : rowSegments) {
    int columns = 0;
    for (char c : row) {
      if (isdigit(c))
        columns += c - '0';
      else
        columns++;
      
      if (c == 'K' && !whiteKingFound)
        whiteKingFound = true;
      else if (c == 'K')
        return false;

      if (c == 'k' && !blackKingFound)
        blackKingFound = true;
      else if (c == 'k')
        return false;
    }
    if (colSize == -1)
      colSize = columns;
    if (colSize != columns)
      return false;
  }

  return true;
}

bool FENManager::isValidTurn(std::string turn) {
  if (turn.size() != 1)
    return false;
  return turn == "w" || turn == "b";
}

bool FENManager::isValidCastlingRights(std::string castling, std::string layout) {
  if (castling == "-")
    return true;
  if (castling.size() == 0 || castling.size() > 4)
    return false;

  // Format requires only characters K, Q, k, q
  // Relative order of these characters also must be preserved
  std::string validCharacters = "KQkq";
  int validCharactersIndex = 0;
  for (int i = 0; i < castling.length(); i++) {
    char c = castling.at(i);
    while (validCharactersIndex < validCharacters.length()) {
      if (validCharacters.at(validCharactersIndex) == c)
        break;
      validCharactersIndex++;
    }
  }

  if (validCharactersIndex >= 4)
    return false;
  
  // Edge cases for castling ruleset
  // For castling to be applicable these rules must be followed:
  // King must be located, for both teams, within at least two squares from the outside perimeter of board
  // White King must be on last row
  // Black King must be on first row
  // Rooks need to be on king/queen side corners for the castling options provided
  //   ie. If user says k for castling, then black king must be on first row and rook must be in corner on 
  //       kings side on first row
  std::vector<std::string> rowSegments = split(layout, '/');
  for (int i = 2; i < rowSegments[0].length() - 2; i++) {
    char c = rowSegments[0].at(i);
    if (c == 'k') {
      if (castling.find('k') != std::string::npos && rowSegments[0].back() != 'r')
        return false;
      if (castling.find('q') != std::string::npos && rowSegments[0].at(0) != 'r')
        return false;
    }
  }
  for (int i = 2; i < rowSegments.back().length() - 2; i++) {
    char c = rowSegments.back().at(i);
    if (c == 'K') {
      if (castling.find('K') != std::string::npos && rowSegments.back().back() != 'R')
        return false;
      if (castling.find('Q') != std::string::npos && rowSegments.back().at(0) != 'R')
        return false;
    }
  }
  return true;
}

bool FENManager::isValidEnpassantRights(std::string enpassant) {
  if (enpassant == "-")
    return true;
  if (enpassant.length() != 2)
    return false;
  return isalpha(enpassant.at(0)) && islower(enpassant.at(0)) && isdigit(enpassant.at(1));
}

bool FENManager::isValidHalfMoves(std::string halfmoves) {
  int moves;
  try {
    moves = std::stoi(halfmoves);
  } catch (const std::invalid_argument &e) {
    return false;
  } 
  return moves >= 0 && moves <= 50;
}

bool FENManager::isValidFullMoves(std::string fullmoves) {
  int moves;
  try {
    moves = std::stoi(fullmoves);
  } catch (const std::invalid_argument &e) {
    return false;
  } 
  return moves >= 0;
}

std::vector<std::string> FENManager::split(std::string str, char delimiter){
    std::vector<std::string> result;
    std::string current = ""; 
    for(int i = 0; i < str.size(); i++){
        if(str[i] == delimiter){
            if(current != ""){
                result.push_back(current);
                current = "";
            } 
            continue;
        }
        current += str[i];
    }
    if(current.size() != 0)
        result.push_back(current);
    return result;
}

std::string FENManager::getLayout() {
  return layout;
}

char FENManager::getTurn() { 
  return turn; 
}

std::string FENManager::getCastlingRights() { 
  return castlingRights; 
};

std::string FENManager::getEnpassantRights() { 
  return enpassantRights; 
};

int FENManager::getHalfMoves() {
  return halfmoves;
};

int FENManager::getFullMoves() {
  return fullmoves;
};