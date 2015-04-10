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
    begin.printBoard();
    std::cout << std::endl;
    
    std::vector<Board> firstMoves = begin.spawnBoardMoves();
    for (auto b : firstMoves)   {
        b.printLastMove();
        std::cout << std::endl;
        b.printBoard();
        std::cout << std::endl;
    }
}
