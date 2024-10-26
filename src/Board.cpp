#include "Board.h"
#include "Knight.h"
#include "Errors.h"

// Standard board
Board::Board() {
  this->boardSize = 64;
  this->rows = 8;
  this->cols = 8;
  this->board = new ChessPiece *[this->boardSize];

  this->board[0] = new Knight(TeamColors::WHITE);
};

// Board with custom board size
Board::Board(int rows, int cols) {
  this->boardSize = rows * cols;
  this->rows = rows;
  this->cols = cols;
  this->board = new ChessPiece *[this->boardSize];
}

Board::~Board() {
  for (int i = 0; i < this->boardSize; i++)
    delete this->board[i];
  delete[] this->board;
};

int Board::getCol(int index) { return index % this->cols; };
int Board::getRow(int index) { return index / this->cols; };
bool Board::isOnBoard(int row, int col) {
  return row >= 0 && row < this->rows && col >= 0 && col < this->cols;
}

const int Board::getIndex(int row, int col) { 
  if (row < 0 || row >= this->rows)
    throw InvalidIndexException("Invalid row " + to_string(row));
  if (col < 0 || col >= this->cols)
    throw InvalidIndexException("Invalid column " + to_string(col));
  int index = row * this->cols + col;
  return index; 
};

ostream &operator<<(ostream &strm, const Board &board) {
  return strm << board.getBoardString();
};

string Board::getBoardString() const {
  string boardString = "";
  for (int i = 0; i < this->boardSize; i++) {
    if (this->board[i] == nullptr)
      boardString += " .  ";
    else
      boardString += " " + this->board[i]->getIdentifier() + "  ";
    if (i % this->rows == this->rows - 1)
      boardString += "\n\n";
  }
  return boardString;
}

ChessPiece *Board::getPiece(int row, int col) {
  return this->board[this->getIndex(row, col)];
}

void Board::setPiece(int row, int col, ChessPiece *piece) {
  int index = getIndex(row, col);  
  this->board[index] = piece;
}

void Board::movePiece(int fromRow, int fromCol, int toRow, int toCol) {
  ChessPiece *piece = this->getPiece(fromRow, fromCol);
  if (piece == nullptr)
    throw std::invalid_argument("No piece at from position");

  // validate the move
  int fromIndex = this->getIndex(fromRow, fromCol);
  int toIndex = this->getIndex(toRow, toCol);
  if (!piece->isValidMove(*this, fromIndex, toIndex))
    throw InvalidMoveException("Invalid move");

  // move the piece
  delete this->board[toIndex];
  this->board[toIndex] = piece;
  this->board[fromIndex] = nullptr;
}

int getBlackStrength() { return 0; };
int getWhiteStrength() { return 0; };