//
//  board_obj.cpp
//  eight-puzzle
//
//  Created by Benjamin Rood on 9/04/15.
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

//	initialise a Board with a defined array.
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

Board::Board(const Board &b, enum tileMove move) //  copy
                                                 //  constructor - copies board
                                                 //  then applies move.
                                                 //	 Used in state expansion.
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

Board::~Board() {}

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

const bool Board::operator==(const Board &rhs) { return (hash == rhs.hash); }

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

void Board::setHeuristic(const int32_t &hVal) { heuristic = hVal; }

const uint32_t &Board::getHash(void) const { return hash; }

const int32_t Board::getFCost(void) const { return pathlength + heuristic; }

//  END INTERNAL CLASS METHODS

//  BEGIN EXTERNAL BOARD FUNCTIONS

std::ostream &operator<<(std::ostream &os, const Board &B) {
  B.toStream(os);
  return os;
}

//  BEGIN BOARD-FRIENDLY FUNCTIONS

std::stack<std::shared_ptr<Board>>
spawnBoardMovesFrom(const std::shared_ptr<Board> B) {
  return spawnBoardMovesFrom(*B);
}

std::stack<std::shared_ptr<Board>> spawnBoardMovesFrom(const Board &B) {
  std::stack<std::shared_ptr<Board>> BoardMoves;

  switch (B.emptyTile) {
  case TOP_LEFT:
    if (B.okMove(RIGHT))
      BoardMoves.push(std::make_shared<Board>(B, RIGHT));
    if (B.okMove(DOWN))
      BoardMoves.push(std::make_shared<Board>(B, DOWN));
    break;

  case TOP_MID:
    if (B.okMove(RIGHT))
      BoardMoves.push(std::make_shared<Board>(B, RIGHT));
    if (B.okMove(DOWN))
      BoardMoves.push(std::make_shared<Board>(B, DOWN));
    if (B.okMove(LEFT))
      BoardMoves.push(std::make_shared<Board>(B, LEFT));
    break;

  case TOP_RIGHT:
    if (B.okMove(DOWN))
      BoardMoves.push(std::make_shared<Board>(B, DOWN));
    if (B.okMove(LEFT))
      BoardMoves.push(std::make_shared<Board>(B, LEFT));
    break;

  case CENTER_LEFT:
    if (B.okMove(UP))
      BoardMoves.push(std::make_shared<Board>(B, UP));
    if (B.okMove(RIGHT))
      BoardMoves.push(std::make_shared<Board>(B, RIGHT));
    if (B.okMove(DOWN))
      BoardMoves.push(std::make_shared<Board>(B, DOWN));
    break;

  case CENTER_MID:
    if (B.okMove(UP))
      BoardMoves.push(std::make_shared<Board>(B, UP));
    if (B.okMove(RIGHT))
      BoardMoves.push(std::make_shared<Board>(B, RIGHT));
    if (B.okMove(DOWN))
      BoardMoves.push(std::make_shared<Board>(B, DOWN));
    if (B.okMove(LEFT))
      BoardMoves.push(std::make_shared<Board>(B, LEFT));
    break;

  case CENTER_RIGHT:
    if (B.okMove(UP))
      BoardMoves.push(std::make_shared<Board>(B, UP));
    if (B.okMove(DOWN))
      BoardMoves.push(std::make_shared<Board>(B, DOWN));
    if (B.okMove(LEFT))
      BoardMoves.push(std::make_shared<Board>(B, LEFT));
    break;

  case BOT_LEFT:
    if (B.okMove(UP))
      BoardMoves.push(std::make_shared<Board>(B, UP));
    if (B.okMove(RIGHT))
      BoardMoves.push(std::make_shared<Board>(B, RIGHT));
    break;

  case BOT_MID:
    if (B.okMove(UP))
      BoardMoves.push(std::make_shared<Board>(B, UP));
    if (B.okMove(RIGHT))
      BoardMoves.push(std::make_shared<Board>(B, RIGHT));
    if (B.okMove(LEFT))
      BoardMoves.push(std::make_shared<Board>(B, LEFT));
    break;

  case BOT_RIGHT:
    if (B.okMove(UP))
      BoardMoves.push(std::make_shared<Board>(B, UP));
    if (B.okMove(LEFT))
      BoardMoves.push(std::make_shared<Board>(B, LEFT));
    break;
  default:
    std::cout << "unknown case presented to switch statement!\n";
    break;
  }

  return BoardMoves;
}

void generateManhattanHeuristic(std::shared_ptr<Board> B) {
  generateManhattanHeuristic(*B);
}

void generateManhattanHeuristic(Board &B) {
  for (int i = 0; i < boardSize; ++i)
    B.heuristic += abs((i - ((int)B[i] + 1)) / 2);
}

void generateMisplacedTilesHeuristic(Board &B) {
  for (int i = 0; i < boardSize; ++i)
    if (i != B[i])
      ++B.heuristic;
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
  std::string moveHistoryString;
  for (auto &m : B.moveHistory) {
    switch (m) {
    case UP:
      moveHistoryString += "U";
      break;

    case LEFT:
      moveHistoryString += "L";
      break;

    case DOWN:
      moveHistoryString += "D";
      break;

    case RIGHT:
      moveHistoryString += "R";
      break;

    default:
      break;
    }
  }
  return moveHistoryString;
}

const std::vector<enum tileMove> &
getMoveHistory(const std::shared_ptr<Board> B) {
  return getMoveHistory(*B);
}

const std::vector<enum tileMove> &getMoveHistory(const Board &B) {
  return B.moveHistory;
}

std::shared_ptr<Board> forceMove(const std::shared_ptr<Board> B,
                                 enum tileMove &move) {
  return std::make_shared<Board>(forceMove(*B, move));
}

Board forceMove(const Board &B, enum tileMove &move) {

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
  newBoard.hash = hashBoardState(newBoard);

  return newBoard;
}

const std::array<uint32_t, 9> &getState(const std::shared_ptr<Board> B) {
  return getState(*B);
}

const std::array<uint32_t, 9> &getState(const Board &B) { return B.boardState; }

bool testForGoalState(const std::shared_ptr<Board> B) {
  return testForGoalState(*B);
}

bool testForGoalState(const Board &B) {
  if (B.hash == 0)
    return true;
  return false;
}

const std::string getStateString(const std::shared_ptr<Board> B) {
  return getStateString(*B);
}

const std::string getStateString(const Board &B) {
  std::string boardStateString;
  for (auto &b : B.boardState)
    boardStateString.push_back(b + '0');
  return boardStateString;
}

const int &getPathLength(const std::shared_ptr<Board> B) {
  return getPathLength(*B);
}

const int &getPathLength(const Board &B) { return B.pathlength; }

const int32_t &getHeuristic(const std::shared_ptr<Board> B) {
  return getHeuristic(*B);
}

const int32_t &getHeuristic(const Board &B) { return B.heuristic; }

const int32_t f_CostOfState(const Board &B) { return B.getFCost(); }

const int32_t f_CostOfState(const std::shared_ptr<Board> B) {
  return f_CostOfState(*B);
}

//  END BOARD-FRIENDLY FUNCTIONS

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
  std::array<uint32_t, 9> cleared = {{0, 1, 2, 3, 4, 5, 6, 7, 8}};
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

const bool operator<(const std::shared_ptr<Board> &lhs,
                     const std::shared_ptr<Board> &rhs) {
  return operator<(*lhs, *rhs);
}

const bool operator<(const Board &lhs, const Board &rhs) {
  return f_CostOfState(lhs) < f_CostOfState(rhs);
}

const bool operator>(const std::shared_ptr<Board> &lhs,
                     const std::shared_ptr<Board> &rhs) {
  return !operator<(*lhs, *rhs);
}

const bool operator>(const Board &lhs, const Board &rhs) {
  return !operator<(lhs, rhs);
}

const bool operator<=(const Board &lhs, const Board &rhs) {
  return f_CostOfState(lhs) <= f_CostOfState(rhs);
}

const bool operator<=(const std::shared_ptr<Board> &lhs,
                      const std::shared_ptr<Board> &rhs) {
  return operator<=(*lhs, *rhs);
}

const bool operator>=(const Board &lhs, const Board &rhs) {
  return !operator<=(lhs, rhs);
}

const bool operator>=(const std::shared_ptr<Board> &lhs,
                      const std::shared_ptr<Board> &rhs) {
  return !operator<=(*lhs, *rhs);
}

//  END EXTERNAL BOARD FUNCTIONS
