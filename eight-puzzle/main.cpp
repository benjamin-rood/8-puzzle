//
//  main.cpp
//  eight-puzzle
//
//  Created by Benjamin Rood on 9/04/15.
//  Copyright (c) 2015 Dave & Ben. All rights reserved.
//

#include "board_obj.h"
#include <set>

int main(int argc, const char * argv[]) {
	
	// TESTING BOARD FUNCTIONALITY
	
	std::set<uint32_t> visitedList;	//	this is what the type of the visited list will be once I get it working.
	
    Board begin;
	printBoard( begin );
    std::cout << std::endl;
	
	
	
	auto newMoves = spawnBoardMovesFrom( begin ); // automatically give me A STACK OF BOARD SHARED_PTRses! YAY!

	std::vector<std::shared_ptr<Board>> enqueuedList;
	
	
	while (!( newMoves.empty() )) {
		generateAdmissibleHeuristic( newMoves.top() );
		// insert functionality which checks for state clashes with visited list and enqued list...
		enqueuedList.push_back( newMoves.top() );	//	... if everything's kosher, add the Board to the enqueued list...
		newMoves.pop();	//	...and once that's done, pop it from the temporary
	}
	
	for (auto& b : enqueuedList)   {
        printLastMove( b );	//	note the lack of dereferencing!
		std::cout << "\tf = " << b->getFCost() << std::endl;	//	see, if getFcost was a friend function instead of a member method we wouldn't have to use the -> arrow ;-)
        printBoard( b );
        std::cout << std::endl;
    }
	
	// now let's arbitrarily remove something from the end of the enqueued list to prove that reference counting works:
	enqueuedList.pop_back();
	std::cout<<"poop!"<<std::endl;
	
	// as you see in the terminal output, it did!
	
	// finally as main ends, all other Board instances are safely deleted!
	
}
