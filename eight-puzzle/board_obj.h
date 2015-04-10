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
#include <random>

//std::random_device rd;
//std::mt19937_64 mt64(rd.operator()());

enum tilePosition { topLeft = 0, topMid = 1, topRight = 2,
    centerLeft = 3, centerMid = 4, centerRight = 5,
    botLeft = 6, botMid = 7, botRight = 8 };

enum tileMove { up = 0, left = 1, down = 2, right = 3 };

class Board {
private:
    std::array<uint32_t, 9> boardState = {{ 0,1,2,3,4,5,6,7,8 }} ;  //  stores a 1D representation of the eight-puzzle tile Board
    uint32_t emptyTile = topLeft;                         //  location of empty tile (0 tile) in array. It's easier to not have to search for this ever single time, plus, we get a very readable name to use!
    uint32_t pathlength = 0;                                        //  current path length from initial (start) boardState to goal boardState of 012345678
    std::vector<enum tileMove> moveHistory;                                        //  stores a record of tile moves (e.g. U,U,D,L,R,D etc)
    
    int lastMove ( void ) const;
    int moveReverse ( const enum tileMove& move ) const;
    Board( const Board& B, enum tileMove move );   //  INTERNAL copy constructor - copies board then applies move.
    bool okMove ( const enum tileMove& move ) const;
    
    
public:
    explicit Board ( void );     //  default constructor will initialise a randomised boardState array.
    Board ( const Board& B );    //  straight copy constructor
    ~Board ( void );             //  no need for anything but the (system-provided) shallow destructor.
    
    //  iterators for range functions used as [begin, end)
    auto* begin ( void ) const;
    auto* end ( void ) const;
    
    Board& operator= ( const Board& rhs );  //  assignment operator.
    
    const bool operator== ( const Board& rhs );             //  obj comparison
    const bool operator!= ( const Board& rhs );             //  obj comparison
    
    const uint32_t& operator[] ( const int index ) const;   //  boardState index position access
    uint32_t& operator[] ( const int index );               //  boardState index position assignment
    
    std::ostream& toStream( std::ostream& os ) const ;
    
    friend std::vector<Board> spawnBoardMovesFrom ( const Board& B );    //  spawns a vector of new Boards based on possible moves from current boardState. Equivalent to STATE EXPANSION! :-)
    friend void printBoard ( const Board& B );
    friend void printLastMove ( const Board& B );
    friend std::string getMoveHistoryString ( const Board& B );
    friend const std::vector<enum tileMove>& getMoveHistory ( const Board& B );
    friend Board recordMove ( const Board& B, enum tileMove move );
    friend const std::array<uint32_t, 9>& getState ( const Board& B );
    friend bool testForGoalState ( const Board& B );
    friend const uint32_t& getPathLength ( const Board& B );
};

std::ostream& operator<< ( std::ostream& os, const Board& B );

















#endif /* defined(__eight_puzzle__Board_obj__) */

