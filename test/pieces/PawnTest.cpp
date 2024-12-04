#include <gtest/gtest.h>
#include "Pawn.h"
#include "King.h"
#include "Board.h"

// Pawn can move vertically (assume white)
TEST(PawnTests, ValidMoveTest1) {
  Board testBoard = Board();
  testBoard.setPiece(3, 4, new Pawn(TeamColors::WHITE));
  EXPECT_TRUE(testBoard.getPiece(3, 4)->isValidMove(testBoard, 3, 4, 2, 4));
}

// Pawn can move vertically (assume black)
TEST(PawnTests, ValidMoveTest2) {
  Board testBoard = Board();
  testBoard.setPiece(3, 4, new Pawn(TeamColors::BLACK));
  EXPECT_TRUE(testBoard.getPiece(3, 4)->isValidMove(testBoard, 3, 4, 4, 4));
}

// Pawn should not be able to move forward if anything is blocking its way
TEST(PawnTests, CollisionTest1) {
  Board testBoard = Board();
  testBoard.setPiece(3, 4, new Pawn(TeamColors::WHITE));
  testBoard.setPiece(2, 4, new Pawn(TeamColors::WHITE));
  EXPECT_FALSE(testBoard.getPiece(3, 4)->isValidMove(testBoard, 3, 4, 2, 4));

  testBoard.setPiece(3, 4, new Pawn(TeamColors::BLACK));
  testBoard.setPiece(4, 4, new Pawn(TeamColors::BLACK));
  EXPECT_FALSE(testBoard.getPiece(3, 4)->isValidMove(testBoard, 3, 4, 4, 4));
}

// Pawn cannot capture pieces on its left and right if they are same team
TEST(PawnTests, CollisionTest2) {
  Board testBoard = Board();
  testBoard.setPiece(3, 4, new Pawn(TeamColors::BLACK));
  testBoard.setPiece(4, 3, new Pawn(TeamColors::BLACK));
  testBoard.setPiece(4, 5, new Pawn(TeamColors::BLACK));
  EXPECT_FALSE(testBoard.getPiece(3, 4)->isValidMove(testBoard, 3, 4, 4, 3));
  EXPECT_FALSE(testBoard.getPiece(3, 4)->isValidMove(testBoard, 3, 4, 4, 5));
}

// Pawn can capture pieces on its left and right if they are different team
TEST(PawnTests, CollisionTest3) {
  Board testBoard = Board();
  testBoard.setPiece(3, 4, new Pawn(TeamColors::BLACK));
  testBoard.setPiece(4, 3, new Pawn(TeamColors::WHITE));
  testBoard.setPiece(4, 5, new Pawn(TeamColors::WHITE));
  EXPECT_TRUE(testBoard.getPiece(3, 4)->isValidMove(testBoard, 3, 4, 4, 3));
  EXPECT_TRUE(testBoard.getPiece(3, 4)->isValidMove(testBoard, 3, 4, 4, 5));
}

// Pawn cannot move to the square it is already in
TEST(PawnTests, InvalidMoveTest1) {
  Board testBoard = Board();
  testBoard.setPiece(3, 4, new Pawn(TeamColors::WHITE));
  EXPECT_FALSE(testBoard.getPiece(3, 4)->isValidMove(testBoard, 3, 4, 3, 4));
}

// Pawn should not be able to move horizontally
TEST(PawnTests, InvalidMoveTest2) {
  Board testBoard = Board();
  testBoard.setPiece(3, 4, new Pawn(TeamColors::WHITE));
  EXPECT_FALSE(testBoard.getPiece(3, 4)->isValidMove(testBoard, 3, 4, 3, 5));
  EXPECT_FALSE(testBoard.getPiece(3, 4)->isValidMove(testBoard, 3, 4, 3, 7));
  EXPECT_FALSE(testBoard.getPiece(3, 4)->isValidMove(testBoard, 3, 4, 3, 2));
  EXPECT_FALSE(testBoard.getPiece(3, 4)->isValidMove(testBoard, 3, 4, 3, 1));
}

// Pawn should not be able to move more than 1 square forward or backwards at all
TEST(PawnTests, InvalidMoveTest3) {
  Board testBoard = Board();
  testBoard.setPiece(3, 4, new Pawn(TeamColors::WHITE));
  EXPECT_FALSE(testBoard.getPiece(3, 4)->isValidMove(testBoard, 3, 4, 1, 4));
  EXPECT_FALSE(testBoard.getPiece(3, 4)->isValidMove(testBoard, 3, 4, 0, 4));
  EXPECT_FALSE(testBoard.getPiece(3, 4)->isValidMove(testBoard, 3, 4, 5, 4));
  EXPECT_FALSE(testBoard.getPiece(3, 4)->isValidMove(testBoard, 3, 4, 6, 4));
}

// Pawn should not be able to move more than 1 square diagonally/anywhere else
TEST(PawnTests, InvalidMoveTest4) {
  Board testBoard = Board();
  testBoard.setPiece(3, 4, new Pawn(TeamColors::WHITE));
  EXPECT_FALSE(testBoard.getPiece(3, 4)->isValidMove(testBoard, 3, 4, 0, 0));
  EXPECT_FALSE(testBoard.getPiece(3, 4)->isValidMove(testBoard, 3, 4, 4, 8));
  EXPECT_FALSE(testBoard.getPiece(3, 4)->isValidMove(testBoard, 3, 4, 1, 4));
  EXPECT_FALSE(testBoard.getPiece(3, 4)->isValidMove(testBoard, 3, 4, 7, 7));
}

// King should not be able to move out of bounds
TEST(PawnTests, OutOfBoundsCheck1) {
  Board testBoard = Board();
  testBoard.setPiece(0, 0, new Pawn(TeamColors::WHITE));
  EXPECT_FALSE(testBoard.getPiece(0, 0)->isValidMove(testBoard, 0, 0, 0, -1));
}

TEST(PawnTests, OutOfBoundsCheck2) {
  Board testBoard = Board();
  testBoard.setPiece(7, 7, new Pawn(TeamColors::BLACK));
  EXPECT_FALSE(testBoard.getPiece(7, 7)->isValidMove(testBoard, 7, 7, 7, 8));
}

// Pawn's valid moves when in center of board and multiple pieces around it (White)
TEST(PawnTests, AllValidMovesTest1) {
  Board testBoard = Board();
  testBoard.setPiece(5, 5, new Pawn(TeamColors::WHITE));
  testBoard.setPiece(4, 5, new Pawn(TeamColors::WHITE));
  testBoard.setPiece(4, 4, new Pawn(TeamColors::BLACK));

  std::unordered_set<int> moves = testBoard.getPiece(5, 5)->getAllValidMoves(testBoard, 5, 5);
  std::unordered_set<int> answer;
  answer.insert(testBoard.getIndex(4, 4));

  EXPECT_TRUE(answer == moves);
}

// Pawn's valid moves when in center of board and multiple pieces around it (Black)
TEST(PawnTests, AllValidMovesTest2) {
  Board testBoard = Board();
  testBoard.setPiece(5, 5, new Pawn(TeamColors::BLACK));
  testBoard.setPiece(6, 4, new Pawn(TeamColors::BLACK));

  std::unordered_set<int> moves = testBoard.getPiece(5, 5)->getAllValidMoves(testBoard, 5, 5);
  std::unordered_set<int> answer;
  answer.insert(testBoard.getIndex(6, 5));

  EXPECT_TRUE(answer == moves);
}

// Pawn's valid moves when in the corner (White)
TEST(PawnTests, AllValidMovesTest3) {
  Board testBoard = Board();
  testBoard.setPiece(0, 0, new Pawn(TeamColors::WHITE));

  std::unordered_set<int> moves = testBoard.getPiece(0, 0)->getAllValidMoves(testBoard, 0, 0);
  std::unordered_set<int> answer;
  EXPECT_TRUE(answer == moves);
}

// Pawn's valid moves when in the corner (Black)
TEST(PawnTests, AllValidMovesTest4) {
  Board testBoard = Board();
  testBoard.setPiece(0, 0, new Pawn(TeamColors::BLACK));

  std::unordered_set<int> moves = testBoard.getPiece(0, 0)->getAllValidMoves(testBoard, 0, 0);
  std::unordered_set<int> answer;
  answer.insert(testBoard.getIndex(1, 0));

  EXPECT_TRUE(answer == moves);
}

TEST(PawnTests, OperatorTest1) {
  Pawn pawn1 = Pawn(TeamColors::WHITE);
  Pawn pawn2 = Pawn(TeamColors::WHITE);
  Pawn pawn3 = Pawn(TeamColors::BLACK);

  EXPECT_TRUE(pawn1 == pawn1);
  EXPECT_TRUE(pawn1 == pawn2);
  EXPECT_TRUE(pawn2 != pawn3);
  EXPECT_FALSE(pawn2 == pawn3);
}

TEST(PawnTests, OperatorTest2) {
  Pawn pawn = Pawn(TeamColors::WHITE);
  King piece = King(TeamColors::WHITE);

  EXPECT_TRUE(pawn != piece);
  EXPECT_FALSE(pawn == piece);
}

// TODO: En passant 