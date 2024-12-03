#include "GameManager.h"

int main() {
  GameManager test = GameManager(5);
  std::cout << test.inCheck(TeamColors::WHITE) << std::endl;
  return 0;
}