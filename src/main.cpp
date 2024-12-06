#include "GameManager.h"

int main() {
  GameManager test = GameManager("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR");
  std::cout << test.inCheck(TeamColors::WHITE) << std::endl;
  return 0;
}