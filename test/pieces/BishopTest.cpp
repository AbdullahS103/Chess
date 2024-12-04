#include <gtest/gtest.h>
#include "Bishop.h"
#include "King.h"
#include "Board.h"

// Bishop can move diagonally
TEST(BishopTests, ValidMoveTest1) {
  Board testBoard = Board();
  testBoard.setPiece(3, 4, new Bishop(TeamColors::WHITE));
  EXPECT_TRUE(testBoard.getPiece(3, 4)->isValidMove(testBoard, 3, 4, 4, 5));
  EXPECT_TRUE(testBoard.getPiece(3, 4)->isValidMove(testBoard, 3, 4, 1, 2));
  EXPECT_TRUE(testBoard.getPiece(3, 4)->isValidMove(testBoard, 3, 4, 6, 1));
  EXPECT_TRUE(testBoard.getPiece(3, 4)->isValidMove(testBoard, 3, 4, 0, 7));
}

// Bishop should not move to a space already occupied by piece of same team
TEST(BishopTests, CollisionTest1) {
  Board testBoard = Board();
  testBoard.setPiece(3, 4, new Bishop(TeamColors::WHITE));
  testBoard.setPiece(2, 3, new Bishop(TeamColors::WHITE));
  EXPECT_FALSE(testBoard.getPiece(3, 4)->isValidMove(testBoard, 3, 4, 2, 3));
}

// Bishop should be able to move to a space already occupied by piece of opposing team
TEST(BishopTests, CollisionTest2) {
  Board testBoard = Board();
  testBoard.setPiece(3, 4, new Bishop(TeamColors::BLACK));
  testBoard.setPiece(2, 3, new Bishop(TeamColors::WHITE));
  EXPECT_TRUE(testBoard.getPiece(3, 4)->isValidMove(testBoard, 3, 4, 2, 3));
}

// Bishop cannot move to the square it is already in
TEST(BishopTests, InvalidMoveTest1) {
  Board testBoard = Board();
  testBoard.setPiece(3, 4, new Bishop(TeamColors::WHITE));
  EXPECT_FALSE(testBoard.getPiece(3, 4)->isValidMove(testBoard, 3, 4, 3, 4));
}

// Bishop should not be able to move horizontally/vertically/anywhere non-diagonal
TEST(BishopTests, InvalidMoveTest2) {
  Board testBoard = Board();
  testBoard.setPiece(3, 4, new Bishop(TeamColors::WHITE));
  EXPECT_FALSE(testBoard.getPiece(3, 4)->isValidMove(testBoard, 3, 4, 3, 5));
  EXPECT_FALSE(testBoard.getPiece(3, 4)->isValidMove(testBoard, 3, 4, 3, 6));
  EXPECT_FALSE(testBoard.getPiece(3, 4)->isValidMove(testBoard, 3, 4, 6, 4));
  EXPECT_FALSE(testBoard.getPiece(3, 4)->isValidMove(testBoard, 3, 4, 2, 4));
  EXPECT_FALSE(testBoard.getPiece(3, 4)->isValidMove(testBoard, 3, 4, 1, 4));
  EXPECT_FALSE(testBoard.getPiece(3, 4)->isValidMove(testBoard, 3, 4, 0, 0));
}

// Bishop should not be able go past another piece diagonally
TEST(BishopTests, InvalidMoveTest3) {
  Board testBoard = Board();
  testBoard.setPiece(3, 4, new Bishop(TeamColors::WHITE));
  testBoard.setPiece(5, 6, new Bishop(TeamColors::BLACK));
  testBoard.setPiece(2, 5, new Bishop(TeamColors::BLACK));

  EXPECT_TRUE(testBoard.getPiece(3, 4)->isValidMove(testBoard, 3, 4, 4, 5));
  EXPECT_FALSE(testBoard.getPiece(3, 4)->isValidMove(testBoard, 3, 4, 6, 7));
  EXPECT_FALSE(testBoard.getPiece(3, 4)->isValidMove(testBoard, 3, 4, 1, 6));
}

// Bishop should not be able to move out of bounds
TEST(BishopTests, OutOfBoundsCheck1) {
  Board testBoard = Board();
  testBoard.setPiece(0, 0, new Bishop(TeamColors::WHITE));
  EXPECT_FALSE(testBoard.getPiece(0, 0)->isValidMove(testBoard, 0, 0, -2, -2));
  EXPECT_FALSE(testBoard.getPiece(0, 0)->isValidMove(testBoard, 0, 0, 5, -5));
}

TEST(BishopTests, OutOfBoundsCheck2) {
  Board testBoard = Board();
  testBoard.setPiece(7, 7, new Bishop(TeamColors::WHITE));
  EXPECT_FALSE(testBoard.getPiece(7, 7)->isValidMove(testBoard, 7, 7, 9, 9));
  EXPECT_FALSE(testBoard.getPiece(7, 7)->isValidMove(testBoard, 7, 7, 10, 4));
}

// Bishop's valid moves when in center of board and multiple pieces around it
TEST(BishopTests, AllValidMovesTest1) {
  Board testBoard = Board();
  testBoard.setPiece(5, 5, new Bishop(TeamColors::WHITE));
  testBoard.setPiece(7, 7, new Bishop(TeamColors::WHITE));
  testBoard.setPiece(2, 2, new Bishop(TeamColors::BLACK));
  testBoard.setPiece(3, 7, new Bishop(TeamColors::BLACK));

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

  EXPECT_TRUE(answer == moves);
}

// Bishop's valid moves when in the corner
TEST(BishopTests, AllValidMovesTest2) {
  Board testBoard = Board();
  testBoard.setPiece(0, 0, new Bishop(TeamColors::WHITE));

  std::unordered_set<int> moves = testBoard.getPiece(0, 0)->getAllValidMoves(testBoard, 0, 0);
  std::unordered_set<int> answer;
  answer.insert(testBoard.getIndex(1, 1));
  answer.insert(testBoard.getIndex(2, 2));
  answer.insert(testBoard.getIndex(3, 3));
  answer.insert(testBoard.getIndex(4, 4));
  answer.insert(testBoard.getIndex(5, 5));
  answer.insert(testBoard.getIndex(6, 6));
  answer.insert(testBoard.getIndex(7, 7));

  EXPECT_TRUE(answer == moves);
}

TEST(BishopTests, OperatorTest1) {
  Bishop bishop1 = Bishop(TeamColors::WHITE);
  Bishop bishop2 = Bishop(TeamColors::WHITE);
  Bishop bishop3 = Bishop(TeamColors::BLACK);

  EXPECT_TRUE(bishop1 == bishop1);
  EXPECT_TRUE(bishop1 == bishop2);
  EXPECT_TRUE(bishop2 != bishop3);
  EXPECT_FALSE(bishop2 == bishop3);
}

TEST(BishopTests, OperatorTest2) {
  Bishop bishop = Bishop(TeamColors::WHITE);
  King piece = King(TeamColors::WHITE);

  EXPECT_TRUE(bishop != piece);
  EXPECT_FALSE(bishop == piece);
}