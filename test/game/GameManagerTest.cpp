#include <gtest/gtest.h>
#include "GameManager.h"

TEST(GameManagerTests, FENStringTest1) {
  GameManager testBoard = GameManager();
  testBoard.movePiece(7, 1, 5, 2);
  EXPECT_EQ(testBoard.getFENString(), "rnbqkbnr/pppppppp/8/8/8/2N5/PPPPPPPP/R1BQKBNR");
}

TEST(GameManagerTests, FENStringTest2) {
  GameManager testBoard = GameManager("k7/5q2/8/8/8/8/8/RNBQKBNR w KQkq - 0 0");
  testBoard.movePiece(0, 0, 0, 1);
  EXPECT_EQ(testBoard.getFENString(), "1k6/5q2/8/8/8/8/8/RNBQKBNR");
}