#include "FENManager.h"
#include "Errors.h"

bool FENManager::isValidFEN(std::string &fenString) {
  std::vector<std::string> fenSegments = split(fenString, ' ');
  if (fenSegments.size() != 6)
    return false;

  if (!isValidLayout(fenSegments[0]))
    return false;
  if (!isValidTurn(fenSegments[1]))
    return false;
  if (!isValidCastlingState(fenSegments[2]))
    return false;
  if (!isValidEnpassantState(fenSegments[3]))
    return false;
  if (!isValidHalfMoves(fenSegments[4]))
    return false;
  if (!isValidFullMoves(fenSegments[5]))
    return false;
  
  return true;
}

bool FENManager::isValidLayout(std::string layout) {
  std::vector<std::string> rowSegments = split(layout, '/');
  if (rowSegments.size() != 8) 
    return false;

  for (std::string row : rowSegments) {
    // TODO: Validate rows with logic
  }

  return true;
}

bool FENManager::isValidTurn(std::string turn) {
  if (turn.size() != 1)
    return false;
  return turn == "w" || turn == "b";
}

bool FENManager::isValidCastlingState(std::string castling) {
  if (castling == "-")
    return true;
  if (castling.size() == 0 || castling.size() > 4)
    return false;
  std::unordered_set<char> validCharacters = {'k', 'K', 'q', 'Q'};
  for (int i = 0; i < castling.length(); i++) {
    char c = castling.at(i);
    if (validCharacters.count(c) == 0)
      return false;
    validCharacters.erase(c);
  }
  return true;
}

bool FENManager::isValidEnpassantState(std::string enpassant) {
  return true;
}

bool FENManager::isValidHalfMoves(std::string halfmoves) {
  return true;
}

bool FENManager::isValidFullMoves(std::string fullmoves) {
  return true;
}

std::string FENManager::getBoardLayout(std::string &fenString) {
  return split(fenString, ' ')[0];
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