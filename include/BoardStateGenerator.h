#ifndef BOARD_STATE_GENERATOR_H
#define BOARD_STATE_GENERATOR_H

#include "CommonHeaders.h"

class Board;

class BoardStateGenerator {
  public:
    static void standardBoard(Board &board);
};

#endif 