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

#define U = 0;
#define L = 1;
#define D = 2;
#define R = 3;

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
    
public:
    explicit Board( void );     //  default constructor will initialise a randomised boardState array.
    Board( const Board& b );    //  straight copy constructor
    Board( const Board& b, enum tileMove move );   //  copy constructor - copies board then applies move.
    Board( Board&& b );         //  move constructor
    ~Board( void );             //  no need for anything but the (system-provided) shallow destructor.
    
    Board& operator= ( const Board& rhs );
    
    const bool operator== ( const Board& rhs );             //  obj comparison
    const bool operator!= ( const Board& rhs );             //  obj comparison
    
    const uint32_t& operator[] ( const int index ) const;   //  boardState index position access
    uint32_t& operator[] ( const int index );               //  boardState index position assignment
    
    
    //  iterators for range functions used as [begin, end)
    auto* begin() const {   return &boardState[0];      }
    auto* end() const   {   return &boardState[9];      }   //  implementation spec calls for max array length +1 position.
    
    std::vector<Board> spawnBoardMoves ( void );    //  spawns a vector of new Boards based on possible moves from current boardState. Equivalent to STATE EXPANSION! :-)
    
    void recordMove ( enum tileMove m );
    void recordMove ( const Board& b, enum tileMove m );
    
    void setState ( const std::array<uint32_t, 9> array );

    auto& getState ( void );
    
    bool testForGoalState ( void );
    
    void setPathLength ( const uint32_t& pl );
    const uint32_t& getPathLength ();
    
    void setMoveHistory ( const std::vector<enum tileMove>& mh );
    const std::vector<enum tileMove>& getMoveHistory();
    
    auto lastMove ( void );
    auto moveReverse ( const enum tileMove& move );
    auto okMove ( const enum tileMove& move );
    
};






#endif /* defined(__eight_puzzle__Board_obj__) */

