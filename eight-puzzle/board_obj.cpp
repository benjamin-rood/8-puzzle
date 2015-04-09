//
//  board_obj.cpp
//  eight-puzzle
//
//  Created by Benjamin Rood on 9/04/15.
//  Copyright (c) 2015 Dave & Ben. All rights reserved.
//

#include "board_obj.h"

Board::Board()  {   //  default constructor will initialise a randomised boardState array.
    std::random_shuffle( std::begin( boardState ), std::end( boardState ) );    //  randomly shuffles the tiles!
    for (int i = 0; i < boardState.size(); ++i)
        if (boardState[i] == 0) {
            emptyTile = (enum tilePositions)i;  //  record location of empty (0) tile.
            break;
        }
}

Board::Board( const Board& b ) : emptyTile{ b.emptyTile },  pathlength{ b.pathlength }, moveHistory{ b.moveHistory }  {
    std::copy( std::begin( b ), std::end( b ), std::begin( boardState ) );
}

Board::Board( Board&& m ) : Board() {
    std::swap( *this, m );
}

Board::~Board() //  faster not to be inline.
{}


Board& Board::operator= ( const Board& rhs )   {
    if (this == &rhs)
        return *this;
    emptyTile = rhs.emptyTile;
    pathlength = rhs.pathlength;
    moveHistory = rhs.moveHistory;
    std::copy( std::begin( rhs ), std::end( rhs ), std::begin( boardState ) );
    return *this;
}

const bool Board::operator== ( const Board& rhs )   {
    if ( emptyTile != rhs.emptyTile )       return false;
    if ( pathlength != rhs.pathlength )     return false;
    if ( moveHistory != rhs.moveHistory )   return false;
    int i = 0;
    for ( auto x : rhs )
        if ( x != boardState[i++] )
            return false;
    
    return true;
}

const bool Board::operator!= ( const Board& rhs )   {
    return !operator== ( rhs );
}

const uint32_t& Board::operator[] ( const int index ) const {
    // run-time check, using STL exceptions:
    
    if (index < 0 || boardState.size() <= index)
        throw std::out_of_range{"Board::operator[]"};
    
    return boardState[index];
}

uint32_t& Board::operator[] ( const int index )   {
    // run-time check, using STL exceptions:
    
    if (index < 0 || boardState.size() <= index)
        throw std::out_of_range{"Board::operator[]"};
    
    return boardState[index];
}


std::vector<Board> Board::generateMoves ( void )    {
    //
}


void Board::recordMove ( const char& c )    {
    moveHistory.push_back(c);
}

void Board::recordMove ( const Board& b, const char& c )    {
    std::copy( std::begin( b ), std::end( b ), std::begin( boardState ) );
    emptyTile = b.emptyTile;
    pathlength = b.pathlength;
    moveHistory.push_back(c);
}


void Board::setState ( const std::array<uint32_t, 9> array )   {
    std::copy( &array[0], &array[array.size()], std::begin( boardState ) );
}

auto& Board::getState ()    {
    return boardState;
}

bool Board::testForGoalState ( void )   {
    std::array<uint32_t, 9> goalState = {{ 0,1,2,3,4,5,6,7,8 }};
    if ( std::equal( std::begin( boardState ), std::end( boardState ), std::begin( goalState ) ) )
        return true;
    return false;
}

void Board::setPathLength ( const uint32_t& pl ) {
    pathlength = pl;
}

const uint32_t& Board::getPathLength () {
    return pathlength;
}

void Board::setMoveHistory ( const std::string& mh )    {
    moveHistory = mh;
}

const std::string& Board::getMoveHistory()  {
    return moveHistory;
}




