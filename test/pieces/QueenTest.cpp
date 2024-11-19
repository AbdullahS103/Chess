#include <gtest/gtest.h>
#include "Queen.h"
#include "Board.h"

// Queen can move horizontally
TEST(QueenTests, ValidMoveTest1) {
  Board testBoard = Board();
  testBoard.setPiece(3, 4, new Queen(TeamColors::WHITE));
  EXPECT_TRUE(testBoard.getPiece(3, 4)->isValidMove(testBoard, 3, 4, 3, 5));
  EXPECT_TRUE(testBoard.getPiece(3, 4)->isValidMove(testBoard, 3, 4, 3, 3));
  EXPECT_TRUE(testBoard.getPiece(3, 4)->isValidMove(testBoard, 3, 4, 3, 7));
  EXPECT_TRUE(testBoard.getPiece(3, 4)->isValidMove(testBoard, 3, 4, 3, 2));
}

// Queen can move vertically
TEST(QueenTests, ValidMoveTest2) {
  Board testBoard = Board();
  testBoard.setPiece(3, 4, new Queen(TeamColors::WHITE));
  EXPECT_TRUE(testBoard.getPiece(3, 4)->isValidMove(testBoard, 3, 4, 2, 4));
  EXPECT_TRUE(testBoard.getPiece(3, 4)->isValidMove(testBoard, 3, 4, 4, 4));
  EXPECT_TRUE(testBoard.getPiece(3, 4)->isValidMove(testBoard, 3, 4, 0, 4));
  EXPECT_TRUE(testBoard.getPiece(3, 4)->isValidMove(testBoard, 3, 4, 6, 4));
}

// Queen can move diagonally
TEST(QueenTests, ValidMoveTest3) {
  Board testBoard = Board();
  testBoard.setPiece(3, 4, new Queen(TeamColors::WHITE));
  EXPECT_TRUE(testBoard.getPiece(3, 4)->isValidMove(testBoard, 3, 4, 4, 5));
  EXPECT_TRUE(testBoard.getPiece(3, 4)->isValidMove(testBoard, 3, 4, 1, 2));
  EXPECT_TRUE(testBoard.getPiece(3, 4)->isValidMove(testBoard, 3, 4, 6, 1));
  EXPECT_TRUE(testBoard.getPiece(3, 4)->isValidMove(testBoard, 3, 4, 0, 7));
}

// Queen should not move to a space already occupied by piece of same team
TEST(QueenTests, CollisionTest1) {
  Board testBoard = Board();
  testBoard.setPiece(3, 4, new Queen(TeamColors::WHITE));
  testBoard.setPiece(3, 3, new Queen(TeamColors::WHITE));
  EXPECT_FALSE(testBoard.getPiece(3, 4)->isValidMove(testBoard, 3, 4, 3, 3));
}

// Queen should be able to move to a space already occupied by piece of opposing team
TEST(QueenTests, CollisionTest2) {
  Board testBoard = Board();
  testBoard.setPiece(3, 4, new Queen(TeamColors::BLACK));
  testBoard.setPiece(3, 3, new Queen(TeamColors::WHITE));
  EXPECT_TRUE(testBoard.getPiece(3, 4)->isValidMove(testBoard, 3, 4, 3, 3));
}

// Queen cannot move to the square it is already in
TEST(QueenTests, InvalidMoveTest1) {
  Board testBoard = Board();
  testBoard.setPiece(3, 4, new Queen(TeamColors::WHITE));
  EXPECT_FALSE(testBoard.getPiece(3, 4)->isValidMove(testBoard, 3, 4, 3, 4));
}

// Queen should not be able to move to spots that are not horizontal, vertical or diagonal
TEST(QueenTests, InvalidMoveTest2) {
  Board testBoard = Board();
  testBoard.setPiece(3, 4, new Queen(TeamColors::WHITE));
  EXPECT_FALSE(testBoard.getPiece(3, 4)->isValidMove(testBoard, 3, 4, 5, 5));
  EXPECT_FALSE(testBoard.getPiece(3, 4)->isValidMove(testBoard, 3, 4, 2, 7));
  EXPECT_FALSE(testBoard.getPiece(3, 4)->isValidMove(testBoard, 3, 4, 0, 0));
  EXPECT_FALSE(testBoard.getPiece(3, 4)->isValidMove(testBoard, 3, 4, 4, 8));
}

// Queen should not be able go past another piece at all
TEST(QueenTests, InvalidMoveTest3) {
  Board testBoard = Board();
  testBoard.setPiece(3, 4, new Queen(TeamColors::WHITE));
  testBoard.setPiece(5, 6, new Queen(TeamColors::BLACK));
  testBoard.setPiece(2, 5, new Queen(TeamColors::BLACK));
  testBoard.setPiece(3, 6, new Queen(TeamColors::BLACK));
  testBoard.setPiece(1, 4, new Queen(TeamColors::BLACK));

  EXPECT_TRUE(testBoard.getPiece(3, 4)->isValidMove(testBoard, 3, 4, 2, 4));
  EXPECT_FALSE(testBoard.getPiece(3, 4)->isValidMove(testBoard, 3, 4, 3, 7));
  EXPECT_FALSE(testBoard.getPiece(3, 4)->isValidMove(testBoard, 3, 4, 0, 4));
  EXPECT_TRUE(testBoard.getPiece(3, 4)->isValidMove(testBoard, 3, 4, 4, 5));
  EXPECT_FALSE(testBoard.getPiece(3, 4)->isValidMove(testBoard, 3, 4, 6, 7));
  EXPECT_FALSE(testBoard.getPiece(3, 4)->isValidMove(testBoard, 3, 4, 1, 6));
}

// Queen should not be able to move out of bounds
TEST(QueenTests, OutOfBoundsCheck1) {
  Board testBoard = Board();
  testBoard.setPiece(0, 0, new Queen(TeamColors::WHITE));
  EXPECT_FALSE(testBoard.getPiece(0, 0)->isValidMove(testBoard, 0, 0, -3, 0));
  EXPECT_FALSE(testBoard.getPiece(0, 0)->isValidMove(testBoard, 0, 0, 0, -3));
  EXPECT_FALSE(testBoard.getPiece(0, 0)->isValidMove(testBoard, 0, 0, -2, -2));
  EXPECT_FALSE(testBoard.getPiece(0, 0)->isValidMove(testBoard, 0, 0, 5, -5));
}

TEST(QueenTests, OutOfBoundsCheck2) {
  Board testBoard = Board();
  testBoard.setPiece(7, 7, new Queen(TeamColors::WHITE));
  EXPECT_FALSE(testBoard.getPiece(7, 7)->isValidMove(testBoard, 7, 7, 9, 9));
  EXPECT_FALSE(testBoard.getPiece(7, 7)->isValidMove(testBoard, 7, 7, 10, 4));
  EXPECT_FALSE(testBoard.getPiece(7, 7)->isValidMove(testBoard, 7, 7, 9, 7));
  EXPECT_FALSE(testBoard.getPiece(7, 7)->isValidMove(testBoard, 7, 7, 7, 10));
}

// Queen's valid moves when in center of board and multiple pieces around it
TEST(QueenTests, AllValidMovesTest1) {
  Board testBoard = Board();
  testBoard.setPiece(5, 5, new Queen(TeamColors::WHITE));
  testBoard.setPiece(5, 5, new Queen(TeamColors::WHITE));
  testBoard.setPiece(5, 3, new Queen(TeamColors::WHITE));
  testBoard.setPiece(1, 5, new Queen(TeamColors::BLACK));
  testBoard.setPiece(5, 7, new Queen(TeamColors::BLACK));
  testBoard.setPiece(5, 5, new Queen(TeamColors::WHITE));
  testBoard.setPiece(7, 7, new Queen(TeamColors::WHITE));
  testBoard.setPiece(2, 2, new Queen(TeamColors::BLACK));
  testBoard.setPiece(3, 7, new Queen(TeamColors::BLACK));

  std::unordered_set<int> moves = testBoard.getPiece(5, 5)->getAllValidMoves(testBoard, 5, 5);
  std::unordered_set<int> answer;
  answer.insert(testBoard.getIndex(6, 6));
  answer.insert(testBoard.getIndex(4, 4));
  answer.insert(testBoard.getIndex(3, 3));
  answer.insert(testBoard.getIndex(2, 2));
  answer.insert(testBoard.getIndex(4, 6));
  answer.insert(testBoard.getIndex(3, 7));
  answer.insert(testBoard.getIndex(6, 4));
  answer.insert(testBoard.getIndex(7, 3));
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

// Queen's valid moves when in the corner
TEST(QueenTests, AllValidMovesTest2) {
  Board testBoard = Board();
  testBoard.setPiece(0, 0, new Queen(TeamColors::WHITE));

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
  answer.insert(testBoard.getIndex(1, 1));
  answer.insert(testBoard.getIndex(2, 2));
  answer.insert(testBoard.getIndex(3, 3));
  answer.insert(testBoard.getIndex(4, 4));
  answer.insert(testBoard.getIndex(5, 5));
  answer.insert(testBoard.getIndex(6, 6));
  answer.insert(testBoard.getIndex(7, 7));

  EXPECT_TRUE(answer == moves);
}

TEST(QueenTests, OperatorTest1) {
  Queen queen1 = Queen(TeamColors::WHITE);
  Queen queen2 = Queen(TeamColors::WHITE);
  Queen queen3 = Queen(TeamColors::BLACK);

  EXPECT_TRUE(queen1 == queen1);
  EXPECT_TRUE(queen1 == queen2);
  EXPECT_TRUE(queen2 != queen3);
  EXPECT_FALSE(queen2 == queen3);
}

TEST(QueenTests, OperatorTest2) {
  Queen queen = Queen(TeamColors::WHITE);
  ChessPiece piece = ChessPiece(TeamColors::WHITE);

  EXPECT_TRUE(queen != piece);
  EXPECT_FALSE(queen == piece);
}