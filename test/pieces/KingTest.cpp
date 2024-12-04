#include <gtest/gtest.h>
#include "King.h"
#include "Queen.h"
#include "Board.h"

// Test if King can move horizontally
TEST(KingTests, ValidMoveTest1) {
  Board testBoard = Board();
  testBoard.setPiece(3, 4, new King(TeamColors::WHITE));
  EXPECT_TRUE(testBoard.getPiece(3, 4)->isValidMove(testBoard, 3, 4, 3, 5));
  EXPECT_TRUE(testBoard.getPiece(3, 4)->isValidMove(testBoard, 3, 4, 3, 3));
}

// King can move vertically
TEST(KingTests, ValidMoveTest2) {
  Board testBoard = Board();
  testBoard.setPiece(3, 4, new King(TeamColors::WHITE));
  EXPECT_TRUE(testBoard.getPiece(3, 4)->isValidMove(testBoard, 3, 4, 2, 4));
  EXPECT_TRUE(testBoard.getPiece(3, 4)->isValidMove(testBoard, 3, 4, 4, 4));
}

// King can move diagonally
TEST(KingTests, ValidMoveTest3) {
  Board testBoard = Board();
  testBoard.setPiece(3, 4, new King(TeamColors::WHITE));
  EXPECT_TRUE(testBoard.getPiece(3, 4)->isValidMove(testBoard, 3, 4, 4, 5));
  EXPECT_TRUE(testBoard.getPiece(3, 4)->isValidMove(testBoard, 3, 4, 4, 3));
  EXPECT_TRUE(testBoard.getPiece(3, 4)->isValidMove(testBoard, 3, 4, 2, 3));
  EXPECT_TRUE(testBoard.getPiece(3, 4)->isValidMove(testBoard, 3, 4, 2, 5));
}

// King should not move to a space already occupied by piece of same team
TEST(KingTests, CollisionTest1) {
  Board testBoard = Board();
  testBoard.setPiece(3, 4, new King(TeamColors::WHITE));
  testBoard.setPiece(3, 3, new King(TeamColors::WHITE));
  EXPECT_FALSE(testBoard.getPiece(3, 4)->isValidMove(testBoard, 3, 4, 3, 3));
}

// King should be able to move to a space already occupied by piece of opposing team
TEST(KingTests, CollisionTest2) {
  Board testBoard = Board();
  testBoard.setPiece(3, 4, new King(TeamColors::BLACK));
  testBoard.setPiece(3, 3, new King(TeamColors::WHITE));
  EXPECT_TRUE(testBoard.getPiece(3, 4)->isValidMove(testBoard, 3, 4, 3, 3));
}

// King cannot move to the square it is already in
TEST(KingTests, InvalidMoveTest1) {
  Board testBoard = Board();
  testBoard.setPiece(3, 4, new King(TeamColors::WHITE));
  EXPECT_FALSE(testBoard.getPiece(3, 4)->isValidMove(testBoard, 3, 4, 3, 4));
}

// King should not be able to move more than 1 square horizontally
TEST(KingTests, InvalidMoveTest2) {
  Board testBoard = Board();
  testBoard.setPiece(3, 4, new King(TeamColors::WHITE));
  EXPECT_FALSE(testBoard.getPiece(3, 4)->isValidMove(testBoard, 3, 4, 3, 6));
  EXPECT_FALSE(testBoard.getPiece(3, 4)->isValidMove(testBoard, 3, 4, 3, 7));
  EXPECT_FALSE(testBoard.getPiece(3, 4)->isValidMove(testBoard, 3, 4, 3, 2));
  EXPECT_FALSE(testBoard.getPiece(3, 4)->isValidMove(testBoard, 3, 4, 3, 1));
}

// King should not be able to move more than 1 square vertically
TEST(KingTests, InvalidMoveTest3) {
  Board testBoard = Board();
  testBoard.setPiece(3, 4, new King(TeamColors::WHITE));
  EXPECT_FALSE(testBoard.getPiece(3, 4)->isValidMove(testBoard, 3, 4, 1, 4));
  EXPECT_FALSE(testBoard.getPiece(3, 4)->isValidMove(testBoard, 3, 4, 0, 4));
  EXPECT_FALSE(testBoard.getPiece(3, 4)->isValidMove(testBoard, 3, 4, 5, 4));
  EXPECT_FALSE(testBoard.getPiece(3, 4)->isValidMove(testBoard, 3, 4, 6, 4));
}

// King should not be able to move more than 1 square diagonally/anywhere else
TEST(KingTests, InvalidMoveTest4) {
  Board testBoard = Board();
  testBoard.setPiece(3, 4, new King(TeamColors::WHITE));
  EXPECT_FALSE(testBoard.getPiece(3, 4)->isValidMove(testBoard, 3, 4, 0, 0));
  EXPECT_FALSE(testBoard.getPiece(3, 4)->isValidMove(testBoard, 3, 4, 4, 8));
  EXPECT_FALSE(testBoard.getPiece(3, 4)->isValidMove(testBoard, 3, 4, 1, 4));
  EXPECT_FALSE(testBoard.getPiece(3, 4)->isValidMove(testBoard, 3, 4, 7, 7));
}

// King should not be able to move out of bounds
TEST(KingTests, OutOfBoundsCheck1) {
  Board testBoard = Board();
  testBoard.setPiece(0, 0, new King(TeamColors::WHITE));
  EXPECT_FALSE(testBoard.getPiece(0, 0)->isValidMove(testBoard, 0, 0, -1, 0));
  EXPECT_FALSE(testBoard.getPiece(0, 0)->isValidMove(testBoard, 0, 0, 0, -1));
}

TEST(KingTests, OutOfBoundsCheck2) {
  Board testBoard = Board();
  testBoard.setPiece(7, 7, new King(TeamColors::WHITE));
  EXPECT_FALSE(testBoard.getPiece(7, 7)->isValidMove(testBoard, 7, 7, 8, 7));
  EXPECT_FALSE(testBoard.getPiece(7, 7)->isValidMove(testBoard, 7, 7, 7, 8));
}

// King's valid moves when in center of board and multiple pieces around it
TEST(KingTests, AllValidMovesTest1) {
  Board testBoard = Board();
  testBoard.setPiece(5, 5, new King(TeamColors::WHITE));
  testBoard.setPiece(5, 6, new King(TeamColors::WHITE));
  testBoard.setPiece(4, 4, new King(TeamColors::BLACK));

  std::unordered_set<int> moves = testBoard.getPiece(5, 5)->getAllValidMoves(testBoard, 5, 5);
  std::unordered_set<int> answer;
  answer.insert(testBoard.getIndex(6, 4));
  answer.insert(testBoard.getIndex(6, 5));
  answer.insert(testBoard.getIndex(6, 6));
  answer.insert(testBoard.getIndex(5, 4));
  answer.insert(testBoard.getIndex(4, 4));
  answer.insert(testBoard.getIndex(4, 5));
  answer.insert(testBoard.getIndex(4, 6));

  EXPECT_TRUE(answer == moves);
}

// King's valid moves when in the corner
TEST(KingTests, AllValidMovesTest2) {
  Board testBoard = Board();
  testBoard.setPiece(0, 0, new King(TeamColors::WHITE));

  std::unordered_set<int> moves = testBoard.getPiece(0, 0)->getAllValidMoves(testBoard, 0, 0);
  std::unordered_set<int> answer;
  answer.insert(testBoard.getIndex(1, 0));
  answer.insert(testBoard.getIndex(0, 1));
  answer.insert(testBoard.getIndex(1, 1));

  EXPECT_TRUE(answer == moves);
}

TEST(KingTests, OperatorTest1) {
  King king1 = King(TeamColors::WHITE);
  King king2 = King(TeamColors::WHITE);
  King king3 = King(TeamColors::BLACK);

  EXPECT_TRUE(king1 == king1);
  EXPECT_TRUE(king1 == king2);
  EXPECT_TRUE(king2 != king3);
  EXPECT_FALSE(king2 == king3);
}

TEST(KingTests, OperatorTest2) {
  King king = King(TeamColors::WHITE);
  Queen queen = Queen(TeamColors::WHITE);

  EXPECT_TRUE(king != queen);
  EXPECT_FALSE(king == queen);
}


