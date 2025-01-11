#include "GameManager.h"
#include "BoardStateGenerator.h"
#include "Errors.h"

#include "King.h"
#include "Pawn.h"
#include "Rook.h"
#include "PieceGenerator.h"

GameManager::GameManager() {
  this->board = new Board();
  BoardStateGenerator::standardBoard(*this->board);
  intializeMemberVariables();

  whiteQueenSideCastling = true;
  whiteKingSideCastling = true;
  blackQueenSideCastling = true;
  blackKingSideCastling = true;
  blackQueenSideRook = board->getPiece(0);
  blackKingSideRook = board->getPiece(7);
  whiteQueenSideRook = board->getPiece(56);
  whiteKingSideRook = board->getPiece(63);
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
  promotionTargets = {"Rook", "Knight", "Bishop", "Queen"};

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
  if (!board->isOnBoard(fromRow, fromCol) || !board->isOnBoard(toRow, toCol))
    throw InvalidMoveException("ERROR: Move is outside board");
  
  ChessPiece *fromPiece = board->getPiece(fromRow, fromCol);
  if (fromPiece == nullptr)
    throw InvalidMoveException("ERROR: No piece at selected position");

  ChessPiece *toPiece = board->getPiece(toRow, toCol);
  if (fromPiece->isSameTeam(toPiece))
    throw InvalidMoveException("ERROR: Move intersects with another piece from the same team!");

  // Check additional edge cases if move is not valid before throwing error
  // These include enpassant and castling
  if (!fromPiece->isValidMove(*board, fromRow, fromCol, toRow, toCol)) {
    if (typeid(*fromPiece) == typeid(Pawn))
      return executeEnpassantRules(fromPiece, toRow, toCol);
    if (typeid(*fromPiece) == typeid(King))
      return executeCastlingRules(fromPiece, toRow, toCol);
    throw InvalidMoveException("ERROR: Invalid move");
  }
  
  // Check if king is safe after move
  TeamColors team = fromPiece->getColor();
  std::map<int, std::unordered_set<ChessPiece*>> enemyControlMap = (team == TeamColors::WHITE) ? blackControlledSpaces : whiteControlledSpaces;

  int fromPieceIndex = board->getIndex(fromRow, fromCol);
  int kingIndex = (team == TeamColors::WHITE ? whiteKingIndex : blackKingIndex);
  int kingRow = board->getRow(kingIndex);
  int kingCol = board->getColumn(kingIndex);
  
  bool isPinned = isPiecePinnedToKing(kingIndex, fromPieceIndex);
  bool isCheck = inCheck(team);
  if (isPinned && isCheck)
    throw InvalidMoveException("ERROR: Piece is pinned and cannot be moved until check resolves.");
  
  // temporarily make the move
  board->setPiece(toRow, toCol, fromPiece);
  board->setPiece(fromRow, fromCol, nullptr);

  // Ensure king is safe after move
  if (isCheck || isPinned) {
    int index = isCheck ? kingIndex : fromPieceIndex;
    for (ChessPiece* attacker : enemyControlMap[index]) {
      // Skip pieces that would be captured with this move
      if (toPiece && *toPiece == *attacker)
        continue;
      int attackerRow = board->getRow(reversePieceMap[attacker]);
      int attackerCol = board->getColumn(reversePieceMap[attacker]);
      if (attacker->isValidMove(*board, attackerRow, attackerCol, kingRow, kingCol)) {
        board->setPiece(toRow, toCol, toPiece);
        board->setPiece(fromRow, fromCol, fromPiece);
        throw InvalidMoveException("ERROR: Move allows opponent to capture king on next turn.");
      }
    }
  }

  // at this point move has been confirmed to be valid, so delete the last piece and update data structures
  delete toPiece;
  updateMemberVariables(fromPiece, toRow, toCol);  

  // update game state variables
  enpassantPiece = nullptr;
  if (typeid(*fromPiece) == typeid(King)) 
    disableKingCastling(team);
  if (typeid(*fromPiece) == typeid(Rook)) {
    whiteKingSideRook = (whiteKingSideRook == fromPiece) ? nullptr : whiteKingSideRook;
    whiteQueenSideRook = (whiteQueenSideRook == fromPiece) ? nullptr : whiteQueenSideRook;
    blackQueenSideRook = (blackQueenSideRook == fromPiece) ? nullptr : blackQueenSideRook;
    blackKingSideRook = (blackKingSideRook == fromPiece) ? nullptr : blackKingSideRook;
  }

  // Edge case enabled by the world of chess - promotion of a pawn
  if (typeid(*fromPiece) == typeid(Pawn) && 
     (fromPiece->getColor() == TeamColors::WHITE && toRow == 0) || 
     (fromPiece->getColor() == TeamColors::BLACK && toRow == board->getRows() - 1))
    executePromotionRules(fromPiece, toRow, toCol);
}

void GameManager::disableKingCastling(TeamColors color) {
  if (color == TeamColors::WHITE) {
    whiteKingSideCastling = false;
    whiteQueenSideCastling = false;
    whiteKingSideRook = nullptr;
    whiteQueenSideRook = nullptr;
  } else {
    blackKingSideCastling = false;
    blackQueenSideCastling = false;
    blackKingSideRook = nullptr;
    blackQueenSideRook = nullptr;
  }
}

void GameManager::executePromotionRules(ChessPiece *pawn, int row, int col) {
  std::cout << "Detected pawn promotion event. Promotion targets are listed below : \n";
  for (std::string target : promotionTargets) 
    std::cout << "  - " << target << "\n";
  std::cout << "To select target, enter target name (case sensitive) : ";
  
  std::string input;
  std::cin >> input;
  while (promotionTargets.find(input) == promotionTargets.end()) {
    std::cout << "Target " << input << " is not a valid promotion target. Please re-enter target name : ";
    std::cin >> input;
  }

  ChessPiece *promotionPiece = PieceGenerator::createPieceByName(input, pawn->getColor());
  while (!promotionPiece) {
    promotionTargets.erase(input);
    std::cout << "Target " << input << " is currently not supported. Updated promotion targets are listed below : \n";
    for (std::string target : promotionTargets) 
      std::cout << "  - " << target << "\n";
    std::cout << "To select target, enter target name (case sensitive) : ";
    std::cin >> input;
    
    while (promotionTargets.find(input) == promotionTargets.end()) {
      std::cout << "Target " << input << " is not a valid promotion target. Please re-enter target name : ";
      std::cin >> input;
    }

    ChessPiece *promotionPiece = PieceGenerator::createPieceByName(input, pawn->getColor());
  }

  delete pawn;
  int index = board->getIndex(row, col);
  board->setPiece(row, col, promotionPiece);

  pieceControlMap.erase(pawn);
  pieceMap[index] = promotionPiece;
  reversePieceMap.erase(pawn);
  reversePieceMap[promotionPiece] = index;
  updateControlSquares(promotionPiece);
}

void GameManager::executeEnpassantRules(ChessPiece *pawn, int toRow, int toCol) {
  TeamColors team = pawn->getColor();
  int fromRow = board->getRow(reversePieceMap[pawn]);
  int fromCol = board->getColumn(reversePieceMap[pawn]);
  
  // If pawn is moving up two spaces, execute a different function to set up enpassant opportunity
  if (abs(fromRow - toRow) == 2 && toCol == fromCol)
    return executeEnpassantMovement(pawn, toRow, toCol);
  
  // Get the piece that would be captured via enpassant
  int pieceRow = (team == TeamColors::WHITE) ? toRow - 1 : toRow + 1;
  ChessPiece *enpassantTarget = board->getPiece(pieceRow, toCol);
  int enpassantTargetRow = board->getRow(reversePieceMap[enpassantTarget]);
  int enpassantTargetCol = board->getColumn(reversePieceMap[enpassantTarget]);

  if (enpassantTarget != enpassantPiece)
    throw InvalidMoveException("ERROR: Chess piece can not be captured via enpassant");
  if ((abs(fromCol - toCol) > 1 || abs(fromRow - toRow) != 1) ||
      (team == TeamColors::WHITE && fromRow < toRow) ||
      (team == TeamColors::BLACK && fromRow > toRow))
    throw InvalidMoveException("ERROR: Invalid pawn move");
 
  // Check if king is safe after move
  std::map<int, std::unordered_set<ChessPiece*>> enemyControlMap = (team == TeamColors::WHITE) ? blackControlledSpaces : whiteControlledSpaces;

  int fromIndex = board->getIndex(fromRow, fromCol);
  int kingIndex = (team == TeamColors::WHITE ? whiteKingIndex : blackKingIndex);
  int kingRow = board->getRow(kingIndex);
  int kingCol = board->getColumn(kingIndex);
  
  bool isPinned = isPiecePinnedToKing(kingIndex, fromIndex);
  bool isCheck = inCheck(team);
  if (isPinned && isCheck)
    throw InvalidMoveException("ERROR: Piece is pinned and cannot be moved until check resolves.");
  
  // temporarily make the move
  board->setPiece(toRow, toCol, pawn);
  board->setPiece(fromRow, fromCol, nullptr);
  board->setPiece(enpassantTargetRow, enpassantTargetCol, nullptr);

  // Ensure king is safe after move
  if (isCheck || isPinned) {
    int index = isCheck ? kingIndex : fromIndex;
    for (ChessPiece* attacker : enemyControlMap[index]) {
      int attackerRow = board->getRow(reversePieceMap[attacker]);
      int attackerCol = board->getColumn(reversePieceMap[attacker]);
      if (attacker->isValidMove(*board, attackerRow, attackerCol, kingRow, kingCol)) {
        board->setPiece(toRow, toCol, nullptr);
        board->setPiece(fromRow, fromCol, pawn);
        board->setPiece(enpassantTargetRow, enpassantTargetCol, enpassantTarget);
        throw InvalidMoveException("ERROR: Move allows opponent to capture king on next turn.");
      }
    }
  }

  // at this point move has been confirmed to be valid, so delete the last piece and update data structures
  delete enpassantTarget;

  int enpassantTargetIndex = reversePieceMap[enpassantTarget];
  int toIndex = board->getIndex(toRow, toCol);

  // Update data structures that track piece location
  reversePieceMap.erase(pieceMap[enpassantTargetIndex]);
  reversePieceMap[pawn] = toIndex;

  pieceMap[enpassantTargetIndex] = nullptr;
  pieceMap[fromIndex] = nullptr;
  pieceMap[toIndex] = pawn;

  // update data structrues that track piece control
  // this includes the original piece that moved AND 
  // all pieces that were looking at where that piece was and where it is now
  updateControlSquares(pawn);
  for (ChessPiece *piece : whiteControlledSpaces[fromIndex]) 
    updateControlSquares(piece);
  for (ChessPiece *piece : blackControlledSpaces[fromIndex])
    updateControlSquares(piece);
  for (ChessPiece *piece : whiteControlledSpaces[toIndex])
    updateControlSquares(piece);
  for (ChessPiece *piece : blackControlledSpaces[toIndex])
    updateControlSquares(piece);
  for (ChessPiece *piece : whiteControlledSpaces[enpassantTargetIndex])
    updateControlSquares(piece);
  for (ChessPiece *piece : blackControlledSpaces[enpassantTargetIndex])
    updateControlSquares(piece);
  
  enpassantPiece = nullptr;
}

void GameManager::executeEnpassantMovement(ChessPiece *pawn, int toRow, int toCol) {
  TeamColors team = pawn->getColor();
  int fromRow = board->getRow(reversePieceMap[pawn]);
  int fromCol = board->getColumn(reversePieceMap[pawn]);

  if ((team == TeamColors::WHITE && fromRow != board->getRows() - 2) || (team == TeamColors::BLACK && fromRow != 1)) 
    throw InvalidMoveException("ERROR: Pawn cannot move two spaces");
  if (!board->getPiece(toRow, toCol) || !board->getPiece((fromRow + toRow) / 2, toCol))
    throw InvalidMoveException("ERROR: Piece blocks forward movement of pawn");

  // Check if king is safe after move
  std::map<int, std::unordered_set<ChessPiece*>> enemyControlMap = (team == TeamColors::WHITE) ? blackControlledSpaces : whiteControlledSpaces;

  int fromIndex = board->getIndex(fromRow, fromCol);
  int kingIndex = (team == TeamColors::WHITE ? whiteKingIndex : blackKingIndex);
  int kingRow = board->getRow(kingIndex);
  int kingCol = board->getColumn(kingIndex);
  
  bool isPinned = isPiecePinnedToKing(kingIndex, fromIndex);
  bool isCheck = inCheck(team);
  if (isPinned && isCheck)
    throw InvalidMoveException("ERROR: Piece is pinned and cannot be moved until check resolves.");
  
  // temporarily make the move
  board->setPiece(toRow, toCol, pawn);
  board->setPiece(fromRow, fromCol, nullptr);

  // Ensure king is safe after move
  if (isCheck || isPinned) {
    int index = isCheck ? kingIndex : fromIndex;
    for (ChessPiece* attacker : enemyControlMap[index]) {
      int attackerRow = board->getRow(reversePieceMap[attacker]);
      int attackerCol = board->getColumn(reversePieceMap[attacker]);
      if (attacker->isValidMove(*board, attackerRow, attackerCol, kingRow, kingCol)) {
        board->setPiece(toRow, toCol, nullptr);
        board->setPiece(fromRow, fromCol, pawn);
        throw InvalidMoveException("ERROR: Move allows opponent to capture king on next turn.");
      }
    }
  }

  int toIndex = board->getIndex(toRow, toCol);

  // Update data structures that track piece location
  reversePieceMap[pawn] = toIndex;

  pieceMap[fromIndex] = nullptr;
  pieceMap[toIndex] = pawn;

  // update data structrues that track piece control
  // this includes the original piece that moved AND 
  // all pieces that were looking at where that piece was and where it is now
  updateControlSquares(pawn);
  for (ChessPiece *piece : whiteControlledSpaces[fromIndex]) 
    updateControlSquares(piece);
  for (ChessPiece *piece : blackControlledSpaces[fromIndex])
    updateControlSquares(piece);
  for (ChessPiece *piece : whiteControlledSpaces[toIndex])
    updateControlSquares(piece);
  for (ChessPiece *piece : blackControlledSpaces[toIndex])
    updateControlSquares(piece);
  
  enpassantPiece = pawn;  
}

void GameManager::executeCastlingRules(ChessPiece *king, int toRow, int toCol) {
  int fromRow = board->getRow(reversePieceMap[king]);
  int fromCol = board->getColumn(reversePieceMap[king]);
  int fromIndex = board->getIndex(fromRow, fromCol);
  int toIndex = board->getIndex(toRow, toCol);
  TeamColors color = king->getColor();

  // series of error checks
  // castling is defined as the king moving two squares in the same
  if (toRow != fromRow || abs(toCol - fromCol) != 2) 
    throw InvalidMoveException("ERROR: Invalid king move. Casling does not apply");
  // king cannot castle if the king or its corresponding rook has moved
  if ((color == TeamColors::WHITE && toCol > fromCol && (!whiteKingSideCastling || !whiteKingSideRook)) ||
      (color == TeamColors::BLACK && toCol > fromCol && (!blackKingSideCastling || !blackKingSideCastling)))
    throw InvalidMoveException("ERROR: King can not castle king side!");
  if ((color == TeamColors::WHITE && toCol < fromCol && (!whiteQueenSideCastling || !whiteQueenSideRook)) || 
      (color == TeamColors::BLACK && toCol < fromCol && (!blackQueenSideCastling || !blackQueenSideRook)))
    throw InvalidMoveException("ERROR: King can not castle queen side!");
  // king canot castle if in check or if the castling move would put king into check
  if (inCheck(color))
    throw InvalidMoveException("ERROR: King is in check and cannot castle");
  if ((color == TeamColors::WHITE && blackControlledSpaces[toIndex].size() > 0) || 
      (color == TeamColors::BLACK && whiteControlledSpaces[toIndex].size() > 0))
    throw InvalidMoveException("ERROR: King is castling into check");

  ChessPiece* castledRook;
  if (color == TeamColors::BLACK)
    castledRook = (toCol > fromCol) ? blackKingSideRook : blackQueenSideRook;
  else
    castledRook = (toCol > fromCol) ? whiteKingSideRook : whiteQueenSideRook;
  int rookIndex = reversePieceMap[castledRook];

  // ensure king has direct line of sight with rook
  int searchStartIndex = (rookIndex < fromIndex) ? rookIndex : fromIndex;
  int searchStopIndex = (rookIndex < fromIndex) ? fromIndex : rookIndex;
  for (int i = searchStartIndex + 1; i <searchStopIndex; i++)
    if (pieceMap[i])
      throw InvalidMoveException("ERROR: " + pieceMap[i]->getName() + " blocks castling opportunity!");
  
  // move the pieces
  int castledRookColumn = (fromCol + toCol) / 2;
  int castledRookIndex = board->getIndex(fromRow, castledRookColumn);
  board->setPiece(fromRow, castledRookColumn, castledRook);
  board->setPiece(toRow, toCol, king);

  // update castling game state variables
  disableKingCastling(color);

  // update data structures
  pieceMap[castledRookIndex] = castledRook;
  pieceMap[toIndex] = king;
  reversePieceMap[castledRook] = castledRookIndex;
  reversePieceMap[king] = toIndex;
  for (int i = 0; i < board->getColumns(); i++) {
    ChessPiece *piece = board->getPiece(board->getIndex(fromRow, i));
    if (piece)
      updateControlSquares(piece);
  }
}

void GameManager::updateMemberVariables(ChessPiece *figure, int toRow, int toCol) {
  int fromRow = board->getRow(reversePieceMap[figure]);
  int fromCol = board->getColumn(reversePieceMap[figure]);
  int fromIndex = board->getIndex(fromRow, fromCol);
  int toIndex = board->getIndex(toRow, toCol);

  // Update data structures that track piece location
  reversePieceMap.erase(pieceMap[toIndex]);
  reversePieceMap[figure] = toIndex;
  pieceMap[fromIndex] = nullptr;
  pieceMap[toIndex] = figure;

  // update data structrues that track piece control
  // this includes the original piece that moved AND 
  // all pieces that were looking at where that piece was and where it is now
  updateControlSquares(figure);
  for (ChessPiece *piece : whiteControlledSpaces[fromIndex]) 
    updateControlSquares(piece);
  for (ChessPiece *piece : blackControlledSpaces[fromIndex])
    updateControlSquares(piece);
  for (ChessPiece *piece : whiteControlledSpaces[toIndex])
    updateControlSquares(piece);
  for (ChessPiece *piece : blackControlledSpaces[toIndex])
    updateControlSquares(piece);
}

void GameManager::updateControlSquares(ChessPiece *piece) {
  std::map<int, std::unordered_set<ChessPiece*>> controlledSpaces = 
      (piece->getColor() == TeamColors::WHITE) ? whiteControlledSpaces : blackControlledSpaces;

  int row = board->getRow(reversePieceMap[piece]);
  int col = board->getColumn(reversePieceMap[piece]);

  std::unordered_set<int> oldSquares = pieceControlMap[piece];
  std::unordered_set<int> newSquares = piece->getAllControlSquares(*board, row, col);
  pieceControlMap[piece] = newSquares;

  // If a square is no longer controlled by the piece, than remove piece from the map
  for (int index : oldSquares) 
    if (newSquares.find(index) == newSquares.end())
      controlledSpaces[index].erase(piece);
  // If a square is now newly target by piece, then add it to the map
  for (int index : newSquares)
    if (oldSquares.find(index) == oldSquares.end())
      controlledSpaces[index].insert(piece);
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

std::ostream &operator<<(std::ostream &strm, const GameManager &game) {
  return strm << game.board;
};