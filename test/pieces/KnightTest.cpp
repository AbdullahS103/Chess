#include <gtest/gtest.h>
#include "Knight.h"
#include "King.h"
#include "Board.h"

// Knight should be able to move lol
TEST(KnightTests, ValidMoveTest1) {
  Board testBoard = Board();
  testBoard.setPiece(2, 2, new Knight(TeamColors::WHITE));
  EXPECT_TRUE(testBoard.getPiece(2, 2)->isValidMove(testBoard, 2, 2, 0, 1));
  EXPECT_TRUE(testBoard.getPiece(2, 2)->isValidMove(testBoard, 2, 2, 0, 3));
  EXPECT_TRUE(testBoard.getPiece(2, 2)->isValidMove(testBoard, 2, 2, 1, 0));
  EXPECT_TRUE(testBoard.getPiece(2, 2)->isValidMove(testBoard, 2, 2, 1, 4));
  EXPECT_TRUE(testBoard.getPiece(2, 2)->isValidMove(testBoard, 2, 2, 3, 0));
  EXPECT_TRUE(testBoard.getPiece(2, 2)->isValidMove(testBoard, 2, 2, 3, 4));
  EXPECT_TRUE(testBoard.getPiece(2, 2)->isValidMove(testBoard, 2, 2, 4, 1));
  EXPECT_TRUE(testBoard.getPiece(2, 2)->isValidMove(testBoard, 2, 2, 4, 3));
}

// Knight should not move to a space already occupied by piece of same team
TEST(KnightTests, CollisionTest1) {
  Board testBoard = Board();
  testBoard.setPiece(3, 4, new Knight(TeamColors::WHITE));
  testBoard.setPiece(2, 2, new Knight(TeamColors::WHITE));
  EXPECT_FALSE(testBoard.getPiece(3, 4)->isValidMove(testBoard, 3, 4, 2, 2));
}

// Knight should be able to move to a space already occupied by piece of opposing team
TEST(KnightTests, CollisionTest2) {
  Board testBoard = Board();
  testBoard.setPiece(3, 4, new Knight(TeamColors::BLACK));
  testBoard.setPiece(2, 2, new Knight(TeamColors::WHITE));
  EXPECT_TRUE(testBoard.getPiece(3, 4)->isValidMove(testBoard, 3, 4, 2, 2));
}

// Knight cannot move to the square it is already in
TEST(KnightTests, InvalidMoveTest1) {
  Board testBoard = Board();
  testBoard.setPiece(3, 4, new Knight(TeamColors::WHITE));
  EXPECT_FALSE(testBoard.getPiece(3, 4)->isValidMove(testBoard, 3, 4, 3, 4));
}

// Knight should not be able to move in a non-L fashion
TEST(KnightTests, InvalidMoveTest2) {
  Board testBoard = Board();
  testBoard.setPiece(3, 4, new Knight(TeamColors::WHITE));
  EXPECT_FALSE(testBoard.getPiece(3, 4)->isValidMove(testBoard, 3, 4, 3, 5));
  EXPECT_FALSE(testBoard.getPiece(3, 4)->isValidMove(testBoard, 3, 4, 5, 4));
  EXPECT_FALSE(testBoard.getPiece(3, 4)->isValidMove(testBoard, 3, 4, 2, 5));
  EXPECT_FALSE(testBoard.getPiece(3, 4)->isValidMove(testBoard, 3, 4, 7, 7));
}

// Knight should not be able to move out of bounds
TEST(KnightTests, OutOfBoundsCheck1) {
  Board testBoard = Board();
  testBoard.setPiece(0, 0, new Knight(TeamColors::WHITE));
  EXPECT_FALSE(testBoard.getPiece(0, 0)->isValidMove(testBoard, 0, 0, -1, -2));
  EXPECT_FALSE(testBoard.getPiece(0, 0)->isValidMove(testBoard, 0, 0, -2, -1));
}

TEST(KnightTests, OutOfBoundsCheck2) {
  Board testBoard = Board();
  testBoard.setPiece(7, 7, new Knight(TeamColors::WHITE));
  EXPECT_FALSE(testBoard.getPiece(7, 7)->isValidMove(testBoard, 7, 7, 8, 9));
  EXPECT_FALSE(testBoard.getPiece(7, 7)->isValidMove(testBoard, 7, 7, 9, 8));
}

// Knight's valid moves when in center of board and multiple pieces around it
TEST(KnightTests, AllValidMovesTest1) {
  Board testBoard = Board();
  testBoard.setPiece(2, 2, new Knight(TeamColors::WHITE));
  testBoard.setPiece(0, 1, new Knight(TeamColors::WHITE));
  testBoard.setPiece(1, 4, new Knight(TeamColors::BLACK));
  testBoard.setPiece(4, 3, new Knight(TeamColors::BLACK));
  testBoard.setPiece(3, 4, new Knight(TeamColors::WHITE));

  std::unordered_set<int> moves = testBoard.getPiece(2, 2)->getAllValidMoves(testBoard, 2, 2);
  std::unordered_set<int> answer;
  answer.insert(testBoard.getIndex(0, 3));
  answer.insert(testBoard.getIndex(1, 0));
  answer.insert(testBoard.getIndex(1, 4));
  answer.insert(testBoard.getIndex(3, 0));
  answer.insert(testBoard.getIndex(4, 1));
  answer.insert(testBoard.getIndex(4, 3));

  EXPECT_TRUE(answer == moves);
}

// Knight's valid moves when in the corner
TEST(KnightTests, AllValidMovesTest2) {
  Board testBoard = Board();
  testBoard.setPiece(0, 0, new Knight(TeamColors::WHITE));

  std::unordered_set<int> moves = testBoard.getPiece(0, 0)->getAllValidMoves(testBoard, 0, 0);
  std::unordered_set<int> answer;
  answer.insert(testBoard.getIndex(2, 1));
  answer.insert(testBoard.getIndex(1, 2));

  EXPECT_TRUE(answer == moves);
}

TEST(KnightTests, OperatorTest1) {
  Knight knight1 = Knight(TeamColors::WHITE);
  Knight knight2 = Knight(TeamColors::WHITE);
  Knight knight3 = Knight(TeamColors::BLACK);

  EXPECT_TRUE(knight1 == knight1);
  EXPECT_TRUE(knight1 == knight2);
  EXPECT_TRUE(knight2 != knight3);
  EXPECT_FALSE(knight2 == knight3);
}

TEST(KnightTests, OperatorTest2) {
  Knight knight = Knight(TeamColors::WHITE);
  King piece = King(TeamColors::WHITE);

  EXPECT_TRUE(knight != piece);
  EXPECT_FALSE(knight == piece);
}