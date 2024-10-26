#include "Board.h"

int main() {
  Board board = Board();
  
  cout << "board beginning" << endl;
  cout << board;
  cout << "board ending" << endl;

  board.movePiece(0, 0, 1, 2);
  cout << "board beginning" << endl;
  cout << board;
  cout << "board ending" << endl;
  return 0;
}