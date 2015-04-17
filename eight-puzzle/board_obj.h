//
//  Board_obj.h
//  eight-puzzle
//
//  Created by Benjamin Rood on 9/04/15.
//  Copyright (c) 2015 Dave & Ben. All rights reserved.
//

#ifndef __eight_puzzle__Board_obj__
#define __eight_puzzle__Board_obj__

#include <iostream>
#include <vector>
#include <array>
#include <stack>
#include <random>


enum tilePosition { TOP_LEFT = 0, TOP_MID = 1, TOP_RIGHT = 2,
    CENTER_LEFT = 3, CENTER_MID = 4, CENTER_RIGHT = 5,
    BOT_LEFT = 6, BOT_MID = 7, BOT_RIGHT = 8 };

enum tileMove { UP = 0, LEFT = 1, DOWN = 2, RIGHT = 3 };

const int boardSize = 9;

const std::array<uint32_t, boardSize> defaultState = {{ 0,1,2,3,4,5,6,7,8 }};	//	same as Goal
const std::array<uint32_t, boardSize> start1State = {{ 0,4,2,1,5,8,3,6,7 }};
const std::array<uint32_t, boardSize> start2State = {{ 1,6,8,3,4,2,7,5,0 }};
const std::array<uint32_t, boardSize> start3State = {{ 4,8,1,3,0,2,6,7,5 }};
const std::array<uint32_t, boardSize> start4State = {{ 8,7,6,5,4,3,2,1,0 }};
const std::array<uint32_t, boardSize> start5State = {{ 1,2,3,8,0,4,7,6,5 }};
const std::array<uint32_t, boardSize> simpleState = {{ 1,0,2,3,4,5,6,7,8 }};	// L > GOAL
const std::array<uint32_t, boardSize> simpleState2 = {{ 3,1,2,6,4,5,0,7,8 }};	// U > U > GOAL
const std::array<uint32_t, boardSize> simpleState3 = {{ 3,1,2,6,4,5,7,0,8 }};	// L > U > U > GOAL


class Board {
private:
    std::array<uint32_t, boardSize> boardState = defaultState;	//  stores a 1-D representation of the eight-puzzle tile Board. On construction gets randomised using std::shuffle.
    uint32_t emptyTile = TOP_LEFT;                              //  location of empty tile (0 tile) in array. It's easier to not have to search for this every single time, plus, we get a very readable name to use!
    int32_t pathlength = 0;										//  current path length from initial (start) boardState to goal boardState of 012345678
	int32_t heuristic = 0;										//	initialised to 0
	uint32_t hash = 0;											//	initialised to 0 as init boardState = goalState.
	std::vector<enum tileMove> moveHistory;                     //  stores a record of tile moves (e.g. U,U,D,L,R,D etc)
	
	
    int lastMove ( void ) const;
    int moveReverse ( const enum tileMove& move ) const;
	bool okMove ( const enum tileMove& move ) const;
    
    
public:
    explicit Board ( void );									//  default constructor will initialise a randomised boardState array.
    Board ( const std::array<uint32_t, boardSize> boardState );	//  construct with specific boardState. Only accepts valid boardState array instances.
    Board ( const Board& B );									//  straight copy constructor
	Board ( const Board& B, enum tileMove move );               //  copy constructor - copies from another Board then applies move.
	Board ( Board&& B ) = default;								//	explicitly use default move constructor
	~Board ( void );											//  no need for anything but the (system-provided) shallow destructor.
    
    //  iterators for range functions used as [begin, end)
    const uint32_t* begin ( void ) const;
    const uint32_t* end ( void ) const;
    
    Board& operator= ( const Board& rhs );					//  copy assignment operator.
	Board& operator= ( Board&& rhs ) = default;				//	explicitly use default move assignment operator
    
    const bool operator== ( const Board& rhs );             //  obj comparison
    const bool operator!= ( const Board& rhs );             //  obj comparison
	
    const uint32_t& operator[] ( const int index ) const;   //  boardState index position access
    uint32_t& operator[] ( const int index );               //  boardState index position assignment
    
    std::ostream& toStream( std::ostream& os ) const ;
	
	void setHeuristic ( const int32_t& hVal );
	
	const uint32_t& getHash ( void ) const;
	const int32_t getFCost ( void ) const;
	
	
    friend std::stack<std::shared_ptr<Board>> spawnBoardMovesFrom ( const Board& B );    //  spawns a vector of new Boards based on possible moves from current boardState. Equivalent to STATE EXPANSION! :-)
	friend std::stack<std::shared_ptr<Board>> spawnBoardMovesFrom ( const std::shared_ptr<Board> B );    //  takes a shared_ptr!
	
	friend void printBoard ( const Board& B );
	friend void printBoard ( const std::shared_ptr<Board> B );
	
	friend void printLastMove ( const Board& B );
	friend void printLastMove ( const std::shared_ptr<Board> B );
	
	friend void generateAdmissibleHeuristic ( Board& B );
	friend void generateAdmissibleHeuristic ( std::shared_ptr<Board> B );
	
	friend std::string getMoveHistoryString ( const Board& B );
	friend std::string getMoveHistoryString ( const std::shared_ptr<Board> B );
	
	friend const std::vector<enum tileMove>& getMoveHistory ( const Board& B );
	friend const std::vector<enum tileMove>& getMoveHistory ( const std::shared_ptr<Board> B );
	
	friend Board forceMove ( const Board& B, enum tileMove& move );
	friend std::shared_ptr<Board> forceMove ( const std::shared_ptr<Board> B, enum tileMove& move );
	
	friend const std::array<uint32_t, 9>& getState ( const Board& B );
	friend const std::array<uint32_t, 9>& getState ( const std::shared_ptr<Board> B );
	
	friend bool testForGoalState ( const Board& B );
	friend bool testForGoalState ( const std::shared_ptr<Board> B );
	
	friend const int32_t& getPathLength ( const Board& B );
	friend const int32_t& getPathLength ( const std::shared_ptr<Board> B );

	friend const int32_t& getHeuristic ( const Board& B );
	friend const int32_t& getHeuristic ( const std::shared_ptr<Board> B );

	friend const int32_t f_CostOfState( const Board& B );
	friend const int32_t f_CostOfState( const std::shared_ptr<Board> B );
};

std::ostream& operator<< ( std::ostream& os, const Board& B );



//	HASHING FUNCTIONALITY
int factorial(int n);
const uint32_t hashBoardState ( const Board& board );
const uint32_t hashBoardState ( const std::shared_ptr<Board> B );
const uint32_t hashBoardState ( const std::array<uint32_t, boardSize> boardArray );


const bool operator< ( const Board& lhs, const Board& rhs );
const bool operator< ( const std::shared_ptr<Board>& lhs, const std::shared_ptr<Board>& rhs );
const bool operator> ( const Board& lhs, const Board& rhs );
const bool operator> ( const std::shared_ptr<Board>& lhs, const std::shared_ptr<Board>& rhs );












#endif /* defined(__eight_puzzle__Board_obj__) */

