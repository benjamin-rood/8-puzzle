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

enum tilePositions { topLeft, topMid, topRight,
    centerLeft, centerMid, centerRight,
    botLeft, botMid, botRight };

class Board {
private:
    std::array<uint32_t, 9> boardState = {{ 0,1,2,3,4,5,6,7,8 }} ;  //  stores a 1D representation of the eight-puzzle tile Board
    enum tilePositions emptyTile = topLeft;                         //  location of empty tile (0 tile) in array. It's easier to not have to search for this ever single time, plus, we get a very readable name to use!
    uint32_t pathlength = 0;                                        //  current path length from initial (start) boardState to goal boardState of 012345678
    std::string moveHistory;                                        //  stores a record of tile moves (e.g. U,U,D,L,R,D etc)
    
public:
    explicit Board( void );     //  default constructor will initialise a randomised boardState array.
    Board( const Board& b );    //  copy constructor
    Board( Board&& b );         //  move constructor
    ~Board( void );             //  no need for anything but the (system-provided) shallow destructor.
    
    Board& operator= ( const Board& rhs );
    
    const bool operator== ( const Board& rhs );             //  obj comparison
    const bool operator!= ( const Board& rhs );             //  obj comparison
    
    const uint32_t& operator[] ( const int index ) const;   //  boardState index position access
    uint32_t& operator[] ( const int index );               //  boardState index position assignment
    
    
    //  const uint32_t* iterators for range functions used as [begin, end)
    auto* begin() const {   return &boardState[0];      }
    auto* end() const   {   return &boardState[9];      }   //  implementation spec calls for max array length +1 position.
    
    std::vector<Board> generateMoves ( void );
    void recordMove ( const char& c );
    void recordMove ( const Board& b, const char& c );
    
    void setState ( const std::array<uint32_t, 9> array );
    //const std::array<uint32_t, 9>& getState ( void );
    auto& getState ( void );
    
    bool testForGoalState ( void );
    
    void setPathLength ( const uint32_t& pl );
    const uint32_t& getPathLength ();
    
    void setMoveHistory ( const std::string& mh );
    const std::string& getMoveHistory();
    
    
};





#endif /* defined(__eight_puzzle__Board_obj__) */

