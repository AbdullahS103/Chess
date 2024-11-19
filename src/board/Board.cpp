#include "Board.h"
#include "Errors.h"

// Standard board
Board::Board() {
  this->boardSize = 64;
  this->rows = 8;
  this->columns = 8;
  this->board = new ChessPiece *[this->boardSize];
  for (int i = 0; i < this->boardSize; i++)
    this->board[i] = nullptr;
};

// Board with custom board size
Board::Board(int rows, int columns) {
  this->boardSize = rows * columns;
  this->rows = rows;
  this->columns = columns;
  this->board = new ChessPiece *[this->boardSize];
  for (int i = 0; i < this->boardSize; i++)
    this->board[i] = nullptr;
}

Board::~Board() {
  for (int i = 0; i < this->boardSize; i++)
    delete this->board[i];
  delete[] this->board;
};

int Board::getRows() const { return this->rows; };
int Board::getColumns() const { return this->columns; };
int Board::getRow(int index) const { return index / this->columns; };
int Board::getColumn(int index) const { return index % this->columns; };

bool Board::isOnBoard(int row, int col) const{
  return row >= 0 && row < this->rows && col >= 0 && col < this->columns;
}

const int Board::getIndex(int row, int col) const{ 
  if (row < 0 || row >= this->rows)
    throw InvalidIndexException("Invalid row " + std::to_string(row));
  if (col < 0 || col >= this->columns)
    throw InvalidIndexException("Invalid column " + std::to_string(col));
  int index = row * this->columns + col;
  return index; 
};

std::ostream &operator<<(std::ostream &strm, const Board &board) {
  return strm << board.toString();
};

std::string Board::toString() const {
  std::string boardString = "";
  for (int i = 0; i < this->boardSize; i++) {
    if (this->board[i] == nullptr)
      boardString += " .  ";
    else
      boardString += " " + this->board[i]->getSymbol() + "  ";
    if (i % this->rows == this->rows - 1)
      boardString += "\n\n";
  }
  return boardString;
}

ChessPiece *Board::getPiece(int row, int col) const{
  if (!isOnBoard(row, col))
    throw InvalidIndexException("Invalid row " + std::to_string(row) + " and column " + std::to_string(col));
  return this->board[this->getIndex(row, col)];
}

ChessPiece *Board::getPiece(int index) const{
  return getPiece(getRow(index), getColumn(index));
}

void Board::setPiece(int row, int col, ChessPiece *piece) {
  int index = getIndex(row, col);  
  this->board[index] = piece;
}

void Board::movePiece(int fromRow, int fromCol, int toRow, int toCol) {
  ChessPiece *fromPiece = this->getPiece(fromRow, fromCol);
  if (fromPiece == nullptr)
    throw std::invalid_argument("No piece at from position");

  // validate the move
  int fromIndex = this->getIndex(fromRow, fromCol);
  int toIndex = this->getIndex(toRow, toCol);
  if (!fromPiece->isValidMove(*this, fromRow, fromCol, toRow, toCol))
    throw InvalidMoveException("Invalid move");

  ChessPiece *toPiece = this->getPiece(toRow, toCol);
  if (fromPiece->isSameTeam(toPiece))
    throw std::invalid_argument("Move intersects with another piece from the same team!");

  // move the piece
  delete this->board[toIndex];
  this->board[toIndex] = fromPiece;
  this->board[fromIndex] = nullptr;
}

bool Board::operator==(const Board &board) const{
  if (board.columns != columns || board.rows != rows || board.boardSize != boardSize)
    return false;

  for (int i = 0; i < boardSize; i++) {
    ChessPiece *piece1 = board.getPiece(i);
    ChessPiece *piece2 = getPiece(i);
    // boths pts are null
    if (!piece1 && !piece2)
      continue;
    // one ptr is null and the other is not
    if ((piece1 && !piece2) || (piece2 && !piece1))
      return false;
    // both ptrs are not null, but pieces are different
    if (*piece1 != *piece2)
      return false;
  }
  return true;
}

bool Board::operator!=(const Board &board) const{ return !(*this == board); };
bool Board::operator==(std::nullptr_t) const { return false; };
bool Board::operator!=(std::nullptr_t) const { return true; };
bool operator==(std::nullptr_t, const Board &board) { return false; };
bool operator!=(std::nullptr_t, const Board &board) { return true; };

int Board::getBlackStrength() const { return 0; };
int Board::getWhiteStrength() const { return 0; };