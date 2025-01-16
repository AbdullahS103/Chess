#include "GameManager.h"
#include "FENManager.h"

int main() {
  GameManager testBoard = GameManager("k7/5q2/8/8/8/8/8/RNBQKBNR w KQkq - 0 0");
  testBoard.movePiece(0, 0, 0, 1);
  std::cout << testBoard.getFENString() << std::endl;
  return 0;
}