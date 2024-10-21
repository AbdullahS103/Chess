#include <iostream>
#include <stdexcept>
#include <unordered_set>
using namespace std;

enum TeamColors { WHITE, BLACK };

//--------------------------------------------------------------------------------------
class Board;

// Class Declarations
// Position is a class to hold the location of a chess piece on the board
class Position {
  int row;
  int col;

public:
  Position(int row, int col);
  ~Position();
  int getRow();
  int getCol();
  void setRow(int row);
  void setCol(int col);
  void setPosition(int row, int col);
  friend ostream &operator<<(ostream &strm, const Position &position);
};
//--------------------------------------------------------------------------------------
// ChessPiece is an abstract class that represents a general chess piece
class ChessPiece {
protected:
  // The team that the piece belongs to
  TeamColors color;

public:
  ChessPiece(TeamColors color);
  virtual ~ChessPiece();

  // Mandatory functions that all pieces that inherit from ChessPiece must implement
  // Given the board state and the move, return if the move is legal
  virtual bool isValidMove(Board &board, int fromIndex, int toIndex);
  // Given the board state and the location, return all valid moves for a given piece
  virtual unordered_set<int> getAllValidMoves(Board &board, int index);
  // Return a unique character associated with piece (mainly for debugging :0)
  virtual string getIdentifier() const;

  // Print the the identifier of a chess piece
  friend ostream &operator<<(ostream &strm, const ChessPiece &chessPiece);
};

class Knight : public ChessPiece {
public:
  Knight(TeamColors color);
  ~Knight();

  bool isValidMove(Board &board, int fromIndex, int toIndex);
  unordered_set<int> getAllValidMoves(Board &board, int index);
  string getIdentifier() const;
};

class Bishop : public ChessPiece {
public:
  Bishop(TeamColors color);
  ~Bishop();

  bool isValidMove(Board &board, int fromIndex, int toIndex);
  unordered_set<int> getAllValidMoves(Board &board, int index);
  string getIdentifier() const;
};

class Rook : public ChessPiece {
public:
  Rook(TeamColors color);
  ~Rook();

  bool isValidMove(Board &board, int fromIndex, int toIndex);
  unordered_set<int> getAllValidMoves(Board &board, int index);
  string getIdentifier() const;
};

class Queen : public ChessPiece {
public:
  Queen(TeamColors color);
  ~Queen();

  bool isValidMove(Board &board, int fromIndex, int toIndex);
  unordered_set<int> getAllValidMoves(Board &board, int index);
  string getIdentifier() const;
};

class King : public ChessPiece {
public:
  King(TeamColors color);
  ~King();

  bool isValidMove(Board &board, int fromIndex, int toIndex);
  unordered_set<int> getAllValidMoves(Board &board, int index);
  string getIdentifier() const;
};

class Pawn : public ChessPiece {
public:
  Pawn(TeamColors color);
  ~Pawn();

  bool isValidMove(Board &board, int fromIndex, int toIndex);
  unordered_set<int> getAllValidMoves(Board &board, int index);
  string getIdentifier() const;
};
//--------------------------------------------------------------------------------------

// Board is a class that represents the state of the current chess board
class Board {
  ChessPiece **board;
  int rows;
  int cols;
  int boardSize;

  string getBoardString() const;

public:
  Board();
  Board(int rows, int cols);
  ~Board();

  // getters
  int getRow(int index);
  int getCol(int index);
  const int getIndex(int row, int col);
  ChessPiece *getPiece(int row, int col);

  // setters
  void setPiece(int row, int col, ChessPiece *piece);

  void movePiece(int fromRow, int fromCol, int toRow, int toCol);
  bool isOnBoard(int row, int col);
  int getBlackStrength();
  int getWhiteStrength();
  friend ostream &operator<<(ostream &strm, const Board &board);
};
