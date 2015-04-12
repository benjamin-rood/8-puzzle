//
//  board_obj.cpp
//  eight-puzzle
//
//  Created by Benjamin Rood on 9/04/15.
//  Copyright (c) 2015 Dave & Ben. All rights reserved.
//

#include "board_obj.h"

//  BEGIN INTERNAL CLASS METHODS

Board::Board(void) { //  default constructor will initialise a randomised
                     //  boardState array.

  std::random_device rd;
  std::mt19937 mt(rd.operator()());
  std::shuffle(std::begin(boardState), std::end(boardState),
               mt); //  randomly shuffles the tiles!
  for (int i = 0; i < boardSize; ++i)
    if (boardState[i] == 0) {
      emptyTile = i; //  record location of empty (0) tile.
      break;
    }
  hash = hashBoardState(boardState);
}

Board::Board(const std::array<uint32_t, boardSize> boardState) {
  std::copy(std::begin(boardState), std::end(boardState),
            std::begin(this->boardState));
  for (int i = 0; i < boardSize; ++i) {
    if (boardState[i] == 0) {
      emptyTile = i; //  record location of empty (0) tile.
      break;
    }
  }
  hash = hashBoardState(boardState);
}

Board::Board(const Board &b) //  straight copy constructor
    : emptyTile{b.emptyTile},
      pathlength{b.pathlength},
      heuristic(b.heuristic),
      hash{b.hash},
      moveHistory{b.moveHistory} {

  std::copy(std::begin(b), std::end(b), std::begin(boardState));
}

Board::Board(const Board &b, enum tileMove move) //  INTERNAL PRIVATE copy
                                                 //  constructor - copies board
                                                 //  then applies move.
    : emptyTile{b.emptyTile},
      pathlength{b.pathlength},
      moveHistory{b.moveHistory} {

  std::copy(std::begin(b), std::end(b), std::begin(boardState));

  switch (move) {
  case UP:
    std::swap(boardState[emptyTile], boardState[emptyTile - 3]);
    emptyTile -= 3;
    break;
  case LEFT:
    std::swap(boardState[emptyTile], boardState[emptyTile - 1]);
    emptyTile -= 1;
    break;
  case DOWN:
    std::swap(boardState[emptyTile], boardState[emptyTile + 3]);
    emptyTile += 3;
    break;
  case RIGHT:
    std::swap(boardState[emptyTile], boardState[emptyTile + 1]);
    emptyTile += 1;
    break;
  default:
    break;
  }

  moveHistory.push_back(move);
  ++pathlength;
  hash = hashBoardState(boardState);
}

Board::~Board() {
  std::cout << "destroying Board with state:\t" << *this << std::endl;
}

//  iterators for range functions used as [begin, end)
const uint32_t *Board::begin(void) const { return &boardState[0]; }

const uint32_t *Board::end(void) const {
  return &boardState[9];
} //  implementation spec calls for max array length +1 position.

Board &Board::operator=(const Board &rhs) {
  if (this == &rhs)
    return *this;
  emptyTile = rhs.emptyTile;
  pathlength = rhs.pathlength;
  moveHistory = rhs.moveHistory;
  std::copy(std::begin(rhs), std::end(rhs), std::begin(boardState));
  return *this;
}

const bool Board::operator==(const Board &rhs) {
  if (emptyTile != rhs.emptyTile)
    return false;
  if (pathlength != rhs.pathlength)
    return false;
  if (moveHistory != rhs.moveHistory)
    return false;
  int i = 0;
  for (auto x : rhs)
    if (x != boardState[i++])
      return false;

  return true;
}

const bool Board::operator!=(const Board &rhs) { return !operator==(rhs); }

const uint32_t &Board::operator[](const int index) const {
  // run-time check, using STL exceptions:

  if (index < 0 || boardSize <= index)
    throw std::out_of_range{"Board::operator[]"};

  return boardState[index];
}

uint32_t &Board::operator[](const int index) {
  // run-time check, using STL exceptions:

  if (index < 0 || boardSize <= index)
    throw std::out_of_range{"Board::operator[]"};

  return boardState[index];
}

int Board::lastMove(void) const { return moveHistory[moveHistory.size() - 1]; }

int Board::moveReverse(const enum tileMove &move) const {
  return ((move + 2) % 4);
}

bool Board::okMove(const enum tileMove &move) const {
  //  returning ( lastMove != ((move+2) mod 4 ))
  //  i.e. the reverse of any move UP, LEFT, DOWN, RIGHT with values from 0-3,
  //  is (move+2)%4, and we compare this with the last move done
  //  to stop backtracking/obvious local loops.
  if (moveHistory.empty())
    return true;
  return (lastMove() != moveReverse(move));
}

std::ostream &Board::toStream(std::ostream &os) const {
  os << "{ ";
  for (auto &x : boardState)
    os << x << " ";
  os << "}";
  return os;
}

void Board::setHeuristic(const int32_t& hVal) { heuristic = hVal; }

const uint32_t &Board::getHash(void) const { return hash; }

const int Board::getFCost(void) const { return pathlength + heuristic; }

//  END INTERNAL CLASS METHODS

//  BEGIN EXTERNAL BOARD FUNCTIONS

std::ostream &operator<<(std::ostream &os, const Board &B) {
  B.toStream(os);
  return os;
}

//  BEGING BOARD-FRIENDLY FUNCTIONS

std::stack<std::shared_ptr<Board>>
spawnBoardMovesFrom(const std::shared_ptr<Board> B) {
  return spawnBoardMovesFrom(*B);
}

std::stack<std::shared_ptr<Board>> spawnBoardMovesFrom(const Board &B) {
  std::stack<std::shared_ptr<Board>> BoardMoves;

  switch (B.emptyTile) {
  case TOP_LEFT:
    if (B.okMove(DOWN))
      BoardMoves.emplace(std::make_shared<Board>(B, DOWN));
    if (B.okMove(RIGHT))
      BoardMoves.emplace(std::make_shared<Board>(B, RIGHT));
    break;

  case TOP_MID:
    if (B.okMove(LEFT))
      BoardMoves.emplace(std::make_shared<Board>(B, LEFT));
    if (B.okMove(DOWN))
      BoardMoves.emplace(std::make_shared<Board>(B, DOWN));
    if (B.okMove(RIGHT))
      BoardMoves.emplace(std::make_shared<Board>(B, RIGHT));
    break;

  case TOP_RIGHT:
    if (B.okMove(LEFT))
      BoardMoves.emplace(std::make_shared<Board>(B, LEFT));
    if (B.okMove(DOWN))
      BoardMoves.emplace(std::make_shared<Board>(B, DOWN));
    break;

  case CENTER_LEFT:
    if (B.okMove(UP))
      BoardMoves.emplace(std::make_shared<Board>(B, UP));
    if (B.okMove(DOWN))
      BoardMoves.emplace(std::make_shared<Board>(B, DOWN));
    if (B.okMove(RIGHT))
      BoardMoves.emplace(std::make_shared<Board>(B, RIGHT));
    break;

  case CENTER_MID:
    if (B.okMove(UP))
      BoardMoves.emplace(std::make_shared<Board>(B, UP));
    if (B.okMove(LEFT))
      BoardMoves.emplace(std::make_shared<Board>(B, LEFT));
    if (B.okMove(DOWN))
      BoardMoves.emplace(std::make_shared<Board>(B, DOWN));
    if (B.okMove(RIGHT))
      BoardMoves.emplace(std::make_shared<Board>(B, RIGHT));
    break;

  case CENTER_RIGHT:
    if (B.okMove(UP))
      BoardMoves.emplace(std::make_shared<Board>(B, UP));
    if (B.okMove(LEFT))
      BoardMoves.emplace(std::make_shared<Board>(B, LEFT));
    if (B.okMove(DOWN))
      BoardMoves.emplace(std::make_shared<Board>(B, DOWN));
    break;

  case BOT_LEFT:
    if (B.okMove(UP))
      BoardMoves.emplace(std::make_shared<Board>(B, UP));
    if (B.okMove(RIGHT))
      BoardMoves.emplace(std::make_shared<Board>(B, RIGHT));
    break;

  case BOT_MID:
    if (B.okMove(UP))
      BoardMoves.emplace(std::make_shared<Board>(B, UP));
    if (B.okMove(LEFT))
      BoardMoves.emplace(std::make_shared<Board>(B, LEFT));
    if (B.okMove(RIGHT))
      BoardMoves.emplace(std::make_shared<Board>(B, RIGHT));
    break;

  case BOT_RIGHT:
    if (B.okMove(UP))
      BoardMoves.emplace(std::make_shared<Board>(B, UP));
    if (B.okMove(LEFT))
      BoardMoves.emplace(std::make_shared<Board>(B, LEFT));
    break;
  default:
    break;
  }

  return BoardMoves;
}

void generateAdmissibleHeuristic(std::shared_ptr<Board> B) {
  generateAdmissibleHeuristic(*B);
}

void generateAdmissibleHeuristic(Board &B) {
  for (int i = 0; i < boardSize; ++i)
    B.heuristic += abs((i - ((int)B[i] + 1)) / 2);
}

void printBoard(const std::shared_ptr<Board> B) { printBoard(*B); }

void printBoard(const Board &B) {
  for (int i = 0; i < boardSize; ++i) {
    if (i == 3 || i == 6)
      std::cout << std::endl;
    std::cout << B.boardState[i] << " ";
  }
  std::cout << std::endl;
}

void printLastMove(const std::shared_ptr<Board> B) { printLastMove(*B); }

void printLastMove(const Board &B) {
  switch (B.lastMove()) {
  case UP:
    std::cout << "U";
    break;

  case LEFT:
    std::cout << "L";
    break;

  case DOWN:
    std::cout << "D";
    break;

  case RIGHT:
    std::cout << "R";
    break;

  default:
    break;
  }
}

std::string getMoveHistoryString(const std::shared_ptr<Board> B) {
  return getMoveHistoryString(*B);
}

std::string getMoveHistoryString(const Board &B) {
  std::string mhs;
  for (auto &m : B.moveHistory) {
    switch (m) {
    case UP:
      mhs += "U";
      break;

    case LEFT:
      mhs += "L";
      break;

    case DOWN:
      mhs += "D";
      break;

    case RIGHT:
      mhs += "R";
      break;

    default:
      break;
    }
  }
  return mhs;
}

const std::vector<enum tileMove>& getMoveHistory ( const std::shared_ptr<Board> B ) {
	return getMoveHistory( *B );
}

const std::vector<enum tileMove>& getMoveHistory(const Board &B) {
  return B.moveHistory;
}

std::shared_ptr<Board> forceMove ( const std::shared_ptr<Board> B, enum tileMove& move ) {
	return std::make_shared<Board>( forceMove( *B, move ) );
}

Board forceMove(const Board &B, enum tileMove& move) {

  Board newBoard(B);
  std::copy(std::begin(B), std::end(B), std::begin(newBoard.boardState));
  newBoard.emptyTile = B.emptyTile;
  newBoard.pathlength = B.pathlength;

  switch (move) {
  case UP:
    std::swap(newBoard.boardState[newBoard.emptyTile],
              newBoard.boardState[newBoard.emptyTile - 3]);
    newBoard.emptyTile -= 3;
    break;
  case LEFT:
    std::swap(newBoard.boardState[newBoard.emptyTile],
              newBoard.boardState[newBoard.emptyTile - 1]);
    newBoard.emptyTile -= 1;
    break;
  case DOWN:
    std::swap(newBoard.boardState[newBoard.emptyTile],
              newBoard.boardState[newBoard.emptyTile + 3]);
    newBoard.emptyTile += 3;
    break;
  case RIGHT:
    std::swap(newBoard.boardState[newBoard.emptyTile],
              newBoard.boardState[newBoard.emptyTile + 1]);
    newBoard.emptyTile += 1;
    break;
  default:
    break;
  }

  newBoard.moveHistory.push_back(move);
  ++newBoard.pathlength;
	newBoard.hash = hashBoardState( newBoard );

  return newBoard;
}

const std::array<uint32_t, 9>& getState ( const std::shared_ptr<Board> B ) {
	return getState( *B );
}

const std::array<uint32_t, 9>& getState(const Board &B) { return B.boardState; }

bool testForGoalState ( const std::shared_ptr<Board> B ) {
	return testForGoalState( *B );
}


bool testForGoalState(const Board &B) {
  if (std::equal(std::begin(B.boardState), std::end(B.boardState),
                 std::begin(goalState)))
    return true;
  return false;
}

const int& getPathLength(const std::shared_ptr<Board> B) {
  return getPathLength(*B);
}

const int& getPathLength(const Board &B) { return B.pathlength; }

//	HASHING FUNCTIONALITY

int factorial(int n) {
  // recursive factorial function
  return (n == 1 || n == 0) ? 1 : factorial(n - 1) * n;
}

const uint32_t hashBoardState(const Board &board) {
	std::array<uint32_t, boardSize> boardArray = getState(board);
	return hashBoardState(boardArray);
}

const uint32_t hashBoardState(const std::shared_ptr<Board> B) {
	return hashBoardState(*B);
}

const uint32_t
hashBoardState(const std::array<uint32_t, boardSize> boardArray) {
  // returns a UNIQUE value for every gamestate.
  // "012345678" (solved) returns 0
  // "012345687" returns 1
  // ...
  // "876543210" returns 362879 (highest result)
  // '9' could be replaced with variable,
  // and cleared[] modified to be a smaller or larger array
  std::array<uint32_t, 9> cleared = {0, 1, 2, 3, 4, 5, 6, 7, 8};
  int permutation_id = 0;
  int location_count = 0;
  int current_num;

  for (int i = 0; i < boardSize; i++) {
    current_num = boardArray[i];
    for (int j = 0; j < current_num; j++) {
      if (cleared[j] != -1) {
        location_count++;
      }
    }
    cleared[current_num] = -1;
    permutation_id += factorial(boardSize - i - 1) * location_count;
    location_count = 0;
  }
  return permutation_id;
}

//  END BOARD-FRIENDLY FUNCTIONS

//  END EXTERNAL BOARD FUNCTIONS
