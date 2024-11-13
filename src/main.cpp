#include "Board.h"
#include "BoardStateGenerator.h"

int main() {
  Board board = Board();
  BoardStateGenerator::standardBoard(board);
  
  cout << "board beginning" << endl;
  cout << board;
  cout << "board ending" << endl;

  board.movePiece(0, 1, 1, 3);
  cout << "board beginning" << endl;
  cout << board;
  cout << "board ending" << endl;
  return 0;
}