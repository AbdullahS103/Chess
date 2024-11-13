#include "Board.h"
#include "Errors.h"

// Standard board
Board::Board() {
  this->boardSize = 64;
  this->rows = 8;
  this->columns = 8;
  this->board = new ChessPiece *[this->boardSize];
};

// Board with custom board size
Board::Board(int rows, int columns) {
  this->boardSize = rows * columns;
  this->rows = rows;
  this->columns = columns;
  this->board = new ChessPiece *[this->boardSize];
}

Board::~Board() {
  for (int i = 0; i < this->boardSize; i++)
    delete this->board[i];
  delete[] this->board;
};

int Board::getRows() { return this->rows; };
int Board::getColumns() { return this->columns; };
int Board::getRow(int index) { return index / this->columns; };
int Board::getColumn(int index) { return index % this->columns; };

bool Board::isOnBoard(int row, int col) {
  return row >= 0 && row < this->rows && col >= 0 && col < this->columns;
}

const int Board::getIndex(int row, int col) { 
  if (row < 0 || row >= this->rows)
    throw InvalidIndexException("Invalid row " + to_string(row));
  if (col < 0 || col >= this->columns)
    throw InvalidIndexException("Invalid column " + to_string(col));
  int index = row * this->columns + col;
  return index; 
};

ostream &operator<<(ostream &strm, const Board &board) {
  return strm << board.toString();
};

string Board::toString() const {
  string boardString = "";
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

ChessPiece *Board::getPiece(int row, int col) {
  if (!isOnBoard(row, col))
    throw InvalidIndexException("Invalid row " + to_string(row) + " and column " + to_string(col));
  return this->board[this->getIndex(row, col)];
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

int getBlackStrength() { return 0; };
int getWhiteStrength() { return 0; };