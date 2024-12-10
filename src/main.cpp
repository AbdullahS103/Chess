#include "GameManager.h"
#include "FENManager.h"

int main() {
  FENManager fen = FENManager("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 0");
  GameManager test = GameManager(fen);
  std::cout << test.inCheck(TeamColors::WHITE) << std::endl;
  return 0;
}