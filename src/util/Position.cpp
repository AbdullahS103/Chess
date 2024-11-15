#include "Position.h"

// Constructor for Position
Position::Position(int row, int col) {
  this->row = row;
  this->col = col;
};
Position::~Position(){};

// Overloaded << operator for Position to display current position
std::ostream &operator<<(std::ostream &strm, const Position &position) {
  return strm << "Position: (" << position.row << ',' << position.col << ')';
};

// Getters for Position class
int Position::getRow() { return this->row; };
int Position::getCol() { return this->col; };

// Setters for Position class
void Position::setRow(int row) { this->row = row; };
void Position::setCol(int col) { this->col = col; };
void Position::setPosition(int row, int col) {
  this->row = row;
  this->col = col;
};