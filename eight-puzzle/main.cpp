//
//  main.cpp
//  eight-puzzle
//
//  Created by Benjamin Rood on 9/04/15.
//  Copyright (c) 2015 Dave & Ben. All rights reserved.
//

#include "board_obj.h"
#include <bitset>

//	a tester function to do a print the status of flag
void print_inBitSet( const std::bitset<362880>& set, const Board& B )	{
	std::cout << "Board has been visited:\t";
	if (set[B.getHash()])
		std::cout << "YES\n";
	else std::cout << "NO\n";
}

int main(int argc, const char * argv[]) {
	
	// TESTING BOARD FUNCTIONALITY
	std::vector<std::shared_ptr<Board>> enqueuedList;
	std::bitset<362880> visitedList; // the same type gets used for both visited and strict expanded list.
									 //	a bitset is initialised by default with all bits set to 0 (false).
    Board begin(start2State);
	generateAdmissibleHeuristic( begin );
	std::cout << begin.getHash() << std::endl;
	std::cout << "f = " << begin.getFCost() << std::endl;
	printBoard( begin );
	print_inBitSet(visitedList, begin);
    std::cout << std::endl;
	
	auto newMoves = spawnBoardMovesFrom( begin ); // automatically give me A STACK OF BOARD SHARED_PTRses! YAY!
	
	visitedList.set(begin.getHash());
	
	while (!( newMoves.empty() )) {
		generateAdmissibleHeuristic( newMoves.top() );
		std::cout << newMoves.top()->getHash() << std::endl;
		printLastMove( newMoves.top() );	//	note the lack of dereferencing because of function overloading.
		std::cout << "\tf = " << newMoves.top()->getFCost() << std::endl;
		printBoard( newMoves.top() );
		print_inBitSet(visitedList, *newMoves.top());
		std::cout << std::endl;
		visitedList.set(newMoves.top()->getHash());
		// insert functionality which checks for state clashes with visited list and enqued list...
		enqueuedList.push_back( newMoves.top() );	//	... if everything's kosher, add the Board to the enqueued list...
		newMoves.pop();	//	...and once that's done, pop it from the temporary stack
	}
	
	std::sort(enqueuedList.begin(), enqueuedList.end());
	
	
	
	for ( auto&b : enqueuedList )   {
		std::cout << b->getHash() << std::endl;
        printLastMove( b );	//	note the lack of dereferencing!
		std::cout << "\tf = " << b->getFCost() << std::endl;	//	see, if getFcost was a friend function instead of a member method we wouldn't have to use the -> arrow ;-)
        printBoard( b );
		print_inBitSet(visitedList, *b);
        std::cout << std::endl;
    }
	

	
	
	// now let's arbitrarily remove something from the end of the enqueued list to prove that reference counting works:
	enqueuedList.pop_back();
	std::cout<<"poop!"<<std::endl;
	
	// as you see in the terminal output, it did!
	
	// finally as main ends, all other Board instances are safely deleted!
	
}
