#ifndef POSITION_H
#define POSITION_H

#include "CommonHeaders.h"

// Position is a class to hold the location of a chess piece on the board
class Position {
  int row;
  int col;

public:
  Position(int row, int col);
  ~Position();
  int getRow();
  int getCol();
  void setRow(int row);
  void setCol(int col);
  void setPosition(int row, int col);
  friend std::ostream &operator<<(std::ostream &strm, const Position &position);
};

#endif