#include <gtest/gtest.h>
#include "Rook.h"
#include "King.h"
#include "Board.h"

// Rook can move horizontally
TEST(RookTests, ValidMoveTest1) {
  Board testBoard = Board();
  testBoard.setPiece(3, 4, new Rook(TeamColors::WHITE));
  EXPECT_TRUE(testBoard.getPiece(3, 4)->isValidMove(testBoard, 3, 4, 3, 5));
  EXPECT_TRUE(testBoard.getPiece(3, 4)->isValidMove(testBoard, 3, 4, 3, 6));
  EXPECT_TRUE(testBoard.getPiece(3, 4)->isValidMove(testBoard, 3, 4, 3, 2));
  EXPECT_TRUE(testBoard.getPiece(3, 4)->isValidMove(testBoard, 3, 4, 3, 0));
}

// Rook can move vertically
TEST(RookTests, ValidMoveTest2) {
  Board testBoard = Board();
  testBoard.setPiece(3, 4, new Rook(TeamColors::WHITE));
  EXPECT_TRUE(testBoard.getPiece(3, 4)->isValidMove(testBoard, 3, 4, 7, 4));
  EXPECT_TRUE(testBoard.getPiece(3, 4)->isValidMove(testBoard, 3, 4, 6, 4));
  EXPECT_TRUE(testBoard.getPiece(3, 4)->isValidMove(testBoard, 3, 4, 2, 4));
  EXPECT_TRUE(testBoard.getPiece(3, 4)->isValidMove(testBoard, 3, 4, 1, 4));
}

// Rook should not move to a space already occupied by piece of same team
TEST(RookTests, CollisionTest1) {
  Board testBoard = Board();
  testBoard.setPiece(3, 4, new Rook(TeamColors::WHITE));
  testBoard.setPiece(3, 3, new Rook(TeamColors::WHITE));
  EXPECT_FALSE(testBoard.getPiece(3, 4)->isValidMove(testBoard, 3, 4, 3, 3));
}

// Rook should be able to move to a space already occupied by piece of opposing team
TEST(RookTests, CollisionTest2) {
  Board testBoard = Board();
  testBoard.setPiece(3, 4, new Rook(TeamColors::BLACK));
  testBoard.setPiece(3, 3, new Rook(TeamColors::WHITE));
  EXPECT_TRUE(testBoard.getPiece(3, 4)->isValidMove(testBoard, 3, 4, 3, 3));
}

// Rook cannot move to the square it is already in
TEST(RookTests, InvalidMoveTest1) {
  Board testBoard = Board();
  testBoard.setPiece(3, 4, new Rook(TeamColors::WHITE));
  EXPECT_FALSE(testBoard.getPiece(3, 4)->isValidMove(testBoard, 3, 4, 3, 4));
}

// Rook should not be able to move more than 1 square diagonally/anywhere else
TEST(RookTests, InvalidMoveTest2) {
  Board testBoard = Board();
  testBoard.setPiece(3, 4, new Rook(TeamColors::WHITE));
  EXPECT_FALSE(testBoard.getPiece(3, 4)->isValidMove(testBoard, 3, 4, 0, 0));
  EXPECT_FALSE(testBoard.getPiece(3, 4)->isValidMove(testBoard, 3, 4, 4, 8));
  EXPECT_FALSE(testBoard.getPiece(3, 4)->isValidMove(testBoard, 3, 4, 4, 5));
  EXPECT_FALSE(testBoard.getPiece(3, 4)->isValidMove(testBoard, 3, 4, 2, 3));
  EXPECT_FALSE(testBoard.getPiece(3, 4)->isValidMove(testBoard, 3, 4, 7, 7));
}

// Rook should not be able go past another piece horizontally/vertically
TEST(RookTests, InvalidMoveTest3) {
  Board testBoard = Board();
  testBoard.setPiece(3, 4, new Rook(TeamColors::WHITE));
  testBoard.setPiece(3, 6, new Rook(TeamColors::BLACK));
  testBoard.setPiece(1, 4, new Rook(TeamColors::BLACK));

  EXPECT_TRUE(testBoard.getPiece(3, 4)->isValidMove(testBoard, 3, 4, 2, 4));
  EXPECT_FALSE(testBoard.getPiece(3, 4)->isValidMove(testBoard, 3, 4, 3, 7));
  EXPECT_FALSE(testBoard.getPiece(3, 4)->isValidMove(testBoard, 3, 4, 0, 4));
}


// Rook should not be able to move out of bounds
TEST(RookTests, OutOfBoundsCheck1) {
  Board testBoard = Board();
  testBoard.setPiece(0, 0, new Rook(TeamColors::WHITE));
  EXPECT_FALSE(testBoard.getPiece(0, 0)->isValidMove(testBoard, 0, 0, -5, 0));
  EXPECT_FALSE(testBoard.getPiece(0, 0)->isValidMove(testBoard, 0, 0, 0, -1));
}

TEST(RookTests, OutOfBoundsCheck2) {
  Board testBoard = Board();
  testBoard.setPiece(7, 7, new Rook(TeamColors::WHITE));
  EXPECT_FALSE(testBoard.getPiece(7, 7)->isValidMove(testBoard, 7, 7, 9, 7));
  EXPECT_FALSE(testBoard.getPiece(7, 7)->isValidMove(testBoard, 7, 7, 7, 10));
}

// Rooks's valid moves when in center of board and multiple pieces around it
TEST(RookTests, AllValidMovesTest1) {
  Board testBoard = Board();
  testBoard.setPiece(5, 5, new Rook(TeamColors::WHITE));
  testBoard.setPiece(5, 3, new Rook(TeamColors::WHITE));
  testBoard.setPiece(1, 5, new Rook(TeamColors::BLACK));
  testBoard.setPiece(5, 7, new Rook(TeamColors::BLACK));

  std::unordered_set<int> moves = testBoard.getPiece(5, 5)->getAllValidMoves(testBoard, 5, 5);
  std::unordered_set<int> answer;
  answer.insert(testBoard.getIndex(5, 6));
  answer.insert(testBoard.getIndex(5, 7));
  answer.insert(testBoard.getIndex(5, 4));
  answer.insert(testBoard.getIndex(1, 5));
  answer.insert(testBoard.getIndex(2, 5));
  answer.insert(testBoard.getIndex(3, 5));
  answer.insert(testBoard.getIndex(4, 5));
  answer.insert(testBoard.getIndex(6, 5));
  answer.insert(testBoard.getIndex(7, 5));

  EXPECT_TRUE(answer == moves);
}

// Rook's valid moves when in the corner
TEST(RookTests, AllValidMovesTest2) {
  Board testBoard = Board();
  testBoard.setPiece(0, 0, new Rook(TeamColors::WHITE));

  std::unordered_set<int> moves = testBoard.getPiece(0, 0)->getAllValidMoves(testBoard, 0, 0);
  std::unordered_set<int> answer;
  answer.insert(testBoard.getIndex(1, 0));
  answer.insert(testBoard.getIndex(2, 0));
  answer.insert(testBoard.getIndex(3, 0));
  answer.insert(testBoard.getIndex(4, 0));
  answer.insert(testBoard.getIndex(5, 0));
  answer.insert(testBoard.getIndex(6, 0));
  answer.insert(testBoard.getIndex(7, 0));
  answer.insert(testBoard.getIndex(0, 1));
  answer.insert(testBoard.getIndex(0, 2));
  answer.insert(testBoard.getIndex(0, 3));
  answer.insert(testBoard.getIndex(0, 4));
  answer.insert(testBoard.getIndex(0, 5));
  answer.insert(testBoard.getIndex(0, 6));
  answer.insert(testBoard.getIndex(0, 7));

  EXPECT_TRUE(answer == moves);
}

TEST(RookTests, OperatorTest1) {
  Rook rook1 = Rook(TeamColors::WHITE);
  Rook rook2 = Rook(TeamColors::WHITE);
  Rook rook3 = Rook(TeamColors::BLACK);

  EXPECT_TRUE(rook1 == rook1);
  EXPECT_TRUE(rook1 == rook2);
  EXPECT_TRUE(rook2 != rook3);
  EXPECT_FALSE(rook2 == rook3);
}

TEST(RookTests, OperatorTest2) {
  Rook rook = Rook(TeamColors::WHITE);
  King piece = King(TeamColors::WHITE);

  EXPECT_TRUE(rook != piece);
  EXPECT_FALSE(rook == piece);
}


