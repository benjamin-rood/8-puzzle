//
//  board_obj.cpp
//  eight-puzzle
//
//  Created by Benjamin Rood on 9/04/15.
//  Copyright (c) 2015 Dave & Ben. All rights reserved.
//

#include "board_obj.h"


//  iterators for range functions used as [begin, end)
auto* Board::begin ( void ) const {
    return &boardState[0];
}

auto* Board::end ( void ) const   {
    return &boardState[9];
}   //  implementation spec calls for max array length +1 position.




Board::Board()  {   //  default constructor will initialise a randomised boardState array.
    
    std::random_device rd;
    std::mt19937 mt(rd.operator()());
    std::shuffle( std::begin( boardState ), std::end( boardState ), mt );    //  randomly shuffles the tiles!
    for (int i = 0; i < boardState.size(); ++i)
        if (boardState[i] == 0) {
            emptyTile = i;  //  record location of empty (0) tile.
            break;
        }
}

Board::Board( const Board& b ) : emptyTile{ b.emptyTile },  //  straight copy constructor
    pathlength{ b.pathlength }, moveHistory{ b.moveHistory }  {
        
    std::copy( std::begin( b ), std::end( b ), std::begin( boardState ) );
}


Board::Board( const Board& b, enum tileMove move )  //  copy constructor - copies board then applies move.
    : emptyTile{ b.emptyTile }, pathlength{ b.pathlength },
    moveHistory{ b.moveHistory }  {
        
    std::copy( std::begin( b ), std::end( b ), std::begin( boardState ) );
    
    switch ( move ) {
        case up:
            std::swap(boardState[emptyTile], boardState[emptyTile-3]);
            emptyTile -= 3;
            break;
        case left:
            std::swap(boardState[emptyTile], boardState[emptyTile-1]);
            emptyTile -= 1;
            break;
        case down:
            std::swap(boardState[emptyTile], boardState[emptyTile+3]);
            emptyTile += 3;
            break;
        case right:
            std::swap(boardState[emptyTile], boardState[emptyTile+1]);
            emptyTile += 1;
            break;
        default:
            break;
    }
    
    moveHistory.push_back( move );
    ++pathlength;
    
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


auto Board::lastMove( void )    {
    if ( moveHistory.empty() )
        return (enum tileMove)emptyTile;
    return moveHistory[moveHistory.size()-1];
}

auto Board::moveReverse( const enum tileMove& move )  {
    return ((move+2) % 4);
}

auto Board::okMove ( const enum tileMove& move )  {
    //  returning ( lastMove != ((move+2) mod 4 ))
    //  i.e. the reverse of any move U,L,D,R with values from 0-3,
    //  is (move+2)%4, and we compare this with the last move done
    //  to stop backtracking/obvious local loops.
    return ( lastMove() != moveReverse(move) );
}


std::vector<Board> Board::spawnBoardMoves ( void )    {
    std::vector<Board> BoardMoves;
    
    switch ( emptyTile ) {
        case topLeft:
            if ( okMove( down ) )
                BoardMoves.push_back( Board( *this, down ) );
            if ( okMove( right ) )
                BoardMoves.push_back( Board( *this, right ) );
            break;
            
        case topMid:
            if ( okMove( left ) )
                BoardMoves.push_back( Board( *this, left ) );
            if ( okMove( down ) )
                BoardMoves.push_back( Board( *this, down ) );
            if ( okMove( right ) )
                BoardMoves.push_back( Board( *this, right ) );
            break;
            
        case topRight:
            if ( okMove( left ) )
                BoardMoves.push_back( Board( *this, left ) );
            if ( okMove( down ) )
                BoardMoves.push_back( Board( *this, down ) );
            break;
            
        case centerLeft:
            if ( okMove( up ) )
                BoardMoves.push_back( Board( *this, down ) );
            if ( okMove( down ) )
                BoardMoves.push_back( Board( *this, down ) );
            if ( okMove( right ) )
                BoardMoves.push_back( Board( *this, right ) );
            break;
        
        case centerMid:
            if ( okMove( up ) )
                BoardMoves.push_back( Board( *this, down ) );
            if ( okMove( left ) )
                BoardMoves.push_back( Board( *this, left ) );
            if ( okMove( down ) )
                BoardMoves.push_back( Board( *this, down ) );
            if ( okMove( right ) )
                BoardMoves.push_back( Board( *this, right ) );
            break;
        
        case centerRight:
            if ( okMove( up ) )
                BoardMoves.push_back( Board( *this, down ) );
            if ( okMove( left ) )
                BoardMoves.push_back( Board( *this, left ) );
            if ( okMove( down ) )
                BoardMoves.push_back( Board( *this, down ) );
            break;
        
        case botLeft:
            if ( okMove( up ) )
                BoardMoves.push_back( Board( *this, down ) );
            if ( okMove( right ) )
                BoardMoves.push_back( Board( *this, right ) );
            break;
        
        case botMid:
            if ( okMove( up ) )
                BoardMoves.push_back( Board( *this, down ) );
            if ( okMove( left ) )
                BoardMoves.push_back( Board( *this, left ) );
            if ( okMove( right ) )
                BoardMoves.push_back( Board( *this, right ) );
            break;
        
        case botRight:
            if ( okMove( up ) )
                BoardMoves.push_back( Board( *this, down ) );
            if ( okMove( left ) )
                BoardMoves.push_back( Board( *this, left ) );
            break;
        default:
            break;
    }
    
    return BoardMoves;
}


void Board::recordMove ( enum tileMove m )    {
    moveHistory.push_back( m );
}

void Board::recordMove ( const Board& b, enum tileMove m )    {
    std::copy( std::begin( b ), std::end( b ), std::begin( boardState ) );
    emptyTile = b.emptyTile;
    pathlength = b.pathlength;
    moveHistory.push_back( m );
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

void Board::setMoveHistory ( const std::vector<enum tileMove>& mh )    {
    moveHistory = mh;
}

const std::vector<enum tileMove>& Board::getMoveHistory()  {
    return moveHistory;
}


void Board::printBoard( void )  {
    for ( int i = 0; i < boardState.size(); ++i ) {
        if (i == 3 || i == 6)
            std::cout << std::endl;
        std::cout << boardState[i] << " ";
    }
    std::cout << std::endl;
}


std::ostream& Board::toStream( std::ostream& os ) const {
    os << "{ ";
    for (auto& x : boardState)
        os << x << " ";
    os << "}";
    return os;
}


std::ostream& operator<< ( std::ostream& os, const Board& B ) {
    B.toStream( os );
    return os;
}


