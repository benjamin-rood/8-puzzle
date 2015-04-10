//
//  board_obj.cpp
//  eight-puzzle
//
//  Created by Benjamin Rood on 9/04/15.
//  Copyright (c) 2015 Dave & Ben. All rights reserved.
//

#include "board_obj.h"



//  BEGIN INTERNAL CLASS METHODS


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


Board::Board( const Board& b, enum tileMove move )  //  INTERNAL PRIVATE copy constructor - copies board then applies move.
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


int Board::lastMove( void ) const  {
    return moveHistory[moveHistory.size()-1];
}

int Board::moveReverse( const enum tileMove& move ) const  {
    return ((move+2) % 4);
}

bool Board::okMove ( const enum tileMove& move )  const {
    //  returning ( lastMove != ((move+2) mod 4 ))
    //  i.e. the reverse of any move up, left, down, right with values from 0-3,
    //  is (move+2)%4, and we compare this with the last move done
    //  to stop backtracking/obvious local loops.
    if ( moveHistory.empty() )
        return true;
    return ( lastMove() != moveReverse(move) );
}


std::ostream& Board::toStream( std::ostream& os ) const {
    os << "{ ";
    for (auto& x : boardState)
        os << x << " ";
    os << "}";
    return os;
}


//  END INTERNAL CLASS METHODS




//  BEGIN EXTERNAL BOARD FUNCTIONS


std::ostream& operator<< ( std::ostream& os, const Board& B ) {
    B.toStream( os );
    return os;
}


//  END EXTERNAL BOARD FUNCTIONS



//  BEGING BOARD-FRIENDLY FUNCTIONS


std::vector<Board> spawnBoardMovesFrom ( const Board& B )    {
    std::vector<Board> BoardMoves;
    
    switch ( B.emptyTile ) {
        case topLeft:
            if ( B.okMove( down ) )
                BoardMoves.push_back( Board( B, down ) );
            if ( B.okMove( right ) )
                BoardMoves.push_back( Board( B, right ) );
            break;
            
        case topMid:
            if ( B.okMove( left ) )
                BoardMoves.push_back( Board( B, left ) );
            if ( B.okMove( down ) )
                BoardMoves.push_back( Board( B, down ) );
            if ( B.okMove( right ) )
                BoardMoves.push_back( Board( B, right ) );
            break;
            
        case topRight:
            if ( B.okMove( left ) )
                BoardMoves.push_back( Board( B, left ) );
            if ( B.okMove( down ) )
                BoardMoves.push_back( Board( B, down ) );
            break;
            
        case centerLeft:
            if ( B.okMove( up ) )
                BoardMoves.push_back( Board( B, up ) );
            if ( B.okMove( down ) )
                BoardMoves.push_back( Board( B, down ) );
            if ( B.okMove( right ) )
                BoardMoves.push_back( Board( B, right ) );
            break;
            
        case centerMid:
            if ( B.okMove( up ) )
                BoardMoves.push_back( Board( B, up ) );
            if ( B.okMove( left ) )
                BoardMoves.push_back( Board( B, left ) );
            if ( B.okMove( down ) )
                BoardMoves.push_back( Board( B, down ) );
            if ( B.okMove( right ) )
                BoardMoves.push_back( Board( B, right ) );
            break;
            
        case centerRight:
            if ( B.okMove( up ) )
                BoardMoves.push_back( Board( B, up ) );
            if ( B.okMove( left ) )
                BoardMoves.push_back( Board( B, left ) );
            if ( B.okMove( down ) )
                BoardMoves.push_back( Board( B, down ) );
            break;
            
        case botLeft:
            if ( B.okMove( up ) )
                BoardMoves.push_back( Board( B, up ) );
            if ( B.okMove( right ) )
                BoardMoves.push_back( Board( B, right ) );
            break;
            
        case botMid:
            if ( B.okMove( up ) )
                BoardMoves.push_back( Board( B, up ) );
            if ( B.okMove( left ) )
                BoardMoves.push_back( Board( B, left ) );
            if ( B.okMove( right ) )
                BoardMoves.push_back( Board( B, right ) );
            break;
            
        case botRight:
            if ( B.okMove( up ) )
                BoardMoves.push_back( Board( B, up ) );
            if ( B.okMove( left ) )
                BoardMoves.push_back( Board( B, left ) );
            break;
        default:
            break;
    }
    
    return BoardMoves;
}


void printBoard ( const Board& B )  {
    for ( int i = 0; i < B.boardState.size(); ++i ) {
        if (i == 3 || i == 6)
            std::cout << std::endl;
        std::cout << B.boardState[i] << " ";
    }
    std::cout << std::endl;
}


void printLastMove ( const Board& B )  {
    switch ( B.lastMove() ) {
        case up:
            std::cout << "U";
            break;
            
        case left:
            std::cout << "L";
            break;
            
        case down:
            std::cout << "D";
            break;
            
        case right:
            std::cout << "R";
            break;
            
        default:
            break;
    }
}


std::string getMoveHistoryString ( const Board& B ) {
    std::string mhs;
    for ( auto&m : B.moveHistory )  {
        switch ( m ) {
            case up:
                mhs += "U";
                break;
                
            case left:
                mhs += "L";
                break;
                
            case down:
                mhs += "D";
                break;
                
            case right:
                mhs += "R";
                break;
                
            default:
                break;
        }
    }
    return mhs;
}


const std::vector<enum tileMove>& getMoveHistory ( const Board& B ) {
    return B.moveHistory;
}


Board recordMove ( const Board& B, enum tileMove move )    {
    
    Board newBoard( B );
    std::copy( std::begin( B ), std::end( B ), std::begin( newBoard.boardState ) );
    newBoard.emptyTile = B.emptyTile;
    newBoard.pathlength = B.pathlength;
    
    switch ( move ) {
        case up:
            std::swap(newBoard.boardState[newBoard.emptyTile], newBoard.boardState[newBoard.emptyTile-3]);
            newBoard.emptyTile -= 3;
            break;
        case left:
            std::swap(newBoard.boardState[newBoard.emptyTile], newBoard.boardState[newBoard.emptyTile-1]);
            newBoard.emptyTile -= 1;
            break;
        case down:
            std::swap(newBoard.boardState[newBoard.emptyTile], newBoard.boardState[newBoard.emptyTile+3]);
            newBoard.emptyTile += 3;
            break;
        case right:
            std::swap(newBoard.boardState[newBoard.emptyTile], newBoard.boardState[newBoard.emptyTile+1]);
            newBoard.emptyTile += 1;
            break;
        default:
            break;
    }
    
    newBoard.moveHistory.push_back( move );
    ++newBoard.pathlength;
    
    return newBoard;
}


const std::array<uint32_t, 9>& getState ( const Board& B )    {
    return B.boardState;
}

bool testForGoalState ( const Board& B )   {
    std::array<uint32_t, 9> goalState = {{ 0,1,2,3,4,5,6,7,8 }};
    if ( std::equal( std::begin( B.boardState ), std::end( B.boardState ), std::begin( goalState ) ) )
        return true;
    return false;
}

const uint32_t& getPathLength ( const Board& B ) {
    return B.pathlength;
}


//  END BOARD-FRIENDLY FUNCTIONS
