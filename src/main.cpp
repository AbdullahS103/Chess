#include "Board.h"
#include "BoardStateGenerator.h"

int main() {
  Board board = Board();
  BoardStateGenerator::standardBoard(board);
  
  std::cout << "board beginning" << std::endl;
  std::cout << board;
  std::cout << "board ending" << std::endl;

  board.movePiece(0, 1, 2, 2);
  std::cout << "board beginning" << std::endl;
  std::cout << board;
  std::cout << "board ending" << std::endl;
  return 0;
}