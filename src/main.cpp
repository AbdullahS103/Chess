#include "GameManager.h"

int main() {
  GameManager test = GameManager();
  std::cout << test.inCheck(TeamColors::WHITE) << std::endl;
  return 0;
}