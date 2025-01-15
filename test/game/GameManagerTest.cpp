#include <gtest/gtest.h>
#include "GameManager.h"

TEST(GameManagerTests, FENStringTest1) {
  GameManager testBoard = GameManager();
  testBoard.movePiece(7, 1, 5, 2);
  EXPECT_EQ(testBoard.getFENString(), "rnbqkbnr/pppppppp/8/8/8/2N5/PPPPPPPP/R1BQKBNR");
}

TEST(GameManagerTests, FENStringTest2) {
  GameManager testBoard = GameManager('')
}