//
//  main.cpp
//  eight-puzzle
//
//  Created by Benjamin Rood on 9/04/15.
//  Copyright (c) 2015 Dave & Ben. All rights reserved.
//

#include "board_obj.h"
#include <bitset>
#include <stack>
#include <set>

const uint32_t domain_size = 362880;

//	a tester function to do a print the status of flag
void print_inBitSet( const std::bitset<domain_size>& set, const Board& B )	{
	std::cout << "Board has been visited:\t";
	if (set[B.getHash()])
		std::cout << "YES\n";
	else std::cout << "NO\n";
}

int main(int argc, const char * argv[]) {
	
	// DEPTH-FIRST SEARCH:
	
	std::stack<std::shared_ptr<Board>> Q;
	std::stack<std::shared_ptr<Board>> expandedStack;
	std::bitset<domain_size> visitedList;			//	the same type gets used for both visited and strict expanded list.
													//	a bitset is initialised by default with all bits set to 0 (false).
	
	std::shared_ptr<Board> start = std::make_shared<Board>( simpleState );
	Q.push( start );
	std::shared_ptr<Board>& B = start;
	printBoard( B );
	
	while (!( Q.empty() ))	{
		
		B = Q.top();
		if ( testForGoalState(B) ) break;
		expandedStack = spawnBoardMovesFrom( B );
		Q.pop();	//	once expanded, get rid of B
		
		while (!( expandedStack.empty() )) {
			auto& top = expandedStack.top();
			Q.push( top );
			expandedStack.pop();
		}
	}
	
	std::string successPath = getMoveHistoryString( B );
	
	for ( auto& s : successPath )
		std::cout << s << " > ";
	std::cout << "GOAL\n";
	
}



//	std::vector<int> v { 3, 12, 4, 11, 5, 9 };
//
//	std::sort(v.begin(), v.end(), std::less<int>());
//
//	std::cout << "v: ";
//	for (auto i : v) std::cout << i << ' ';
//	std::cout << '\n';
//
//	v.push_back(1);
//
//	std::cout << "v: ";
//	for (auto i : v) std::cout << i << ' ';
//	std::cout << '\n';
//
//	std::sort(v.begin(), v.end(), std::less<int>());
//
//	std::cout << "v: ";
//	for (auto i : v) std::cout << i << ' ';
//	std::cout << '\n';
//
//
//
//	// TESTING BOARD FUNCTIONALITY
//	std::vector<std::shared_ptr<Board>> enqueuedList;
//	std::bitset<domain_size> visitedList;			//	the same type gets used for both visited and strict expanded list.
//													//	a bitset is initialised by default with all bits set to 0 (false).
//
//
//	enqueuedList.push_back(std::make_shared<Board>(start4State));
//	enqueuedList.push_back(std::make_shared<Board>(start1State));
//	enqueuedList.push_back(std::make_shared<Board>(start3State));
//
//	std::cout << "\nBefore make_heap:\n";
//	for (auto& b : enqueuedList) {
//		generateAdmissibleHeuristic( b );
//		std::cout << b->getHash() << std::endl;
//		std::cout << "f = " << b->getFCost() << std::endl;
//		//printBoard(b);
//		std::cout << '\n';
//	}
//
//	std::make_heap( enqueuedList.begin(), enqueuedList.end() );
//
//	std::cout << "\nAfter make_heap:\n";
//	for (auto& b : enqueuedList) {
//		std::cout << b->getHash() << std::endl;
//		std::cout << "f = " << b->getFCost() << std::endl;
//		//printBoard(b);
//		std::cout << '\n';
//	}
//
//	enqueuedList.push_back(std::make_shared<Board>(start5State));
//	generateAdmissibleHeuristic( enqueuedList[enqueuedList.size()-1] );
//
//	std::cout << "before push_heap: \n";
//	for (auto& b : enqueuedList) {
//		std::cout << b->getHash() << std::endl;
//		std::cout << "f = " << b->getFCost() << std::endl;
//		//printBoard(b);
//		std::cout << '\n';
//	}
//
//	std::push_heap(enqueuedList.begin(), enqueuedList.end());
//
//	std::cout << "after push_heap: \n";
//	for (auto& b : enqueuedList) {
//		std::cout << b->getHash() << std::endl;
//		std::cout << "f = " << b->getFCost() << std::endl;
//		//printBoard(b);
//		std::cout << '\n';
//	}
//
//
//	enqueuedList.push_back(std::make_shared<Board>(start2State));
//	generateAdmissibleHeuristic( enqueuedList[enqueuedList.size()-1] );
//
//	std::cout << "before push_heap: \n";
//	for (auto& b : enqueuedList) {
//		std::cout << b->getHash() << std::endl;
//		std::cout << "f = " << b->getFCost() << std::endl;
//		//printBoard(b);
//		std::cout << '\n';
//	}
//
//	std::push_heap(enqueuedList.begin(), enqueuedList.end());
//
//	std::cout << "after push_heap: \n";
//	for (auto& b : enqueuedList) {
//		std::cout << b->getHash() << std::endl;
//		std::cout << "f = " << b->getFCost() << std::endl;
//		//printBoard(b);
//		std::cout << '\n';
//	}
//
//
//	for (int i = 0; i < 5; ++i) {
//		enqueuedList.push_back(std::make_shared<Board>());
//		generateAdmissibleHeuristic( enqueuedList[enqueuedList.size()-1] );
//
//		std::cout << "before push_heap: \n";
//		for (auto& b : enqueuedList) {
//			std::cout << b->getHash() << std::endl;
//			std::cout << "f = " << b->getFCost() << std::endl;
//			//printBoard(b);
//			std::cout << '\n';
//		}
//
//		std::push_heap(enqueuedList.begin(), enqueuedList.end());
//
//		std::cout << "after push_heap: \n";
//		for (auto& b : enqueuedList) {
//			std::cout << b->getHash() << std::endl;
//			std::cout << "f = " << b->getFCost() << std::endl;
//			//printBoard(b);
//			std::cout << '\n';
//		}
//	}
//
//	std::pop_heap(enqueuedList.begin(), enqueuedList.end());
//	enqueuedList.pop_back();
//
//	std::cout << "after pop_heap: \n";
//	for (auto& b : enqueuedList) {
//		std::cout << b->getHash() << std::endl;
//		std::cout << "f = " << b->getFCost() << std::endl;
//		//printBoard(b);
//		std::cout << '\n';
//	}
//
//	std::cout << "is enqueuedList.front > enqueuedList.back?\n" << (enqueuedList.front() > enqueuedList.back()) << std::endl;
//
//	std::make_heap(enqueuedList.begin(), enqueuedList.end());
//
//	std::cout << "\nafter make_heap: \n";
//	for (auto& b : enqueuedList) {
//		std::cout << b->getHash() << std::endl;
//		std::cout << "f = " << b->getFCost() << std::endl;
//		//printBoard(b);
//		std::cout << '\n';
//	}
//
//	std::cout << "is enqueuedList.front > enqueuedList.back?\n" << (enqueuedList.front() > enqueuedList.back()) << std::endl;
//
