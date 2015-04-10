//
//  main.cpp
//  eight-puzzle
//
//  Created by Benjamin Rood on 9/04/15.
//  Copyright (c) 2015 Dave & Ben. All rights reserved.
//

#include "board_obj.h"

int main(int argc, const char * argv[]) {
    Board begin;
    printBoard( begin );
    std::cout << std::endl;
    
    std::vector<Board> firstMoves = spawnBoardMovesFrom( begin );
    for (auto b : firstMoves)   {
        printLastMove( b );
        std::cout << std::endl;
        printBoard( b );
        std::cout << std::endl;
    }
}
