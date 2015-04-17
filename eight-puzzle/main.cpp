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
#include <queue>
#include <set>

const uint32_t domain_size = 362880;

//	a tester function to do a print the status of flag
void print_inBitSet( const std::bitset<domain_size>& set, const Board& B )	{
	std::cout << "Board has been visited:\t";
	if (set[B.getHash()])
		std::cout << "YES\n";
	else std::cout << "NO\n";
}

void print_SUCCESS ( std::shared_ptr<Board>& B, size_t queueSize, uint32_t numExpansions )	{
	std::cout << "SUCCESS!\n\n";
	printBoard( B );
	std::cout << std::endl;
	std::cout << "Maximum Queue Length = " << queueSize << std::endl;
	std::cout << "Path Length = " << getPathLength(B) << std::endl;
	std::cout << "Number of State Expansions = " << numExpansions << std::endl;
	std::string successPath = getMoveHistoryString( B );
	for ( auto& s : successPath )
		std::cout << s << " > ";
	std::cout << "GOAL\n\n";
}

void print_FAIL ( void )	{
	std::cout << "FAILED to find GOAL.\n\n";
}

int main(int argc, const char * argv[]) {
	
	
	// PROGRESSIVE DEEPING SEARCH:
	{
		bool success = false;
		size_t highest_Q_length = 0;
		uint32_t num_of_expansions = 0;
		std::bitset<domain_size> visitedList;			//	the same type gets used for both visited and strict expanded list.
														//	a bitset is initialised by default with all bits set to 0 (false).
		
		std::stack<std::shared_ptr<Board>> qStack;	//	LIFO Enqueued List!
		std::stack<std::shared_ptr<Board>> expandedStack;
		
		uint32_t depth_stop = 1;
		uint32_t depth_max = 60;
		
		std::cout << "PROGRESSIVE DEEPING SEARCH\n\n";
		
		while ( depth_stop != depth_max ) {
			++depth_stop;
			visitedList.reset();
//			std::cout << "Begin >> Progressive Deepening Search:\tDepth = " << depth_stop-1 << "\n\n";
			
			std::shared_ptr<Board> start = std::make_shared<Board>( start4State );
			qStack.push( start );
			std::shared_ptr<Board>& B = start;
			
			
			while (!( qStack.empty() ))	{
				if (highest_Q_length < qStack.size() )	highest_Q_length = qStack.size();
				
				B = qStack.top();
				qStack.pop();	//	once B retrieved from qStack, pop qStack.
				visitedList.set( B->getHash() );	//	add B to visited list
				
				if ( testForGoalState(B) )	{
					success = true;
					print_SUCCESS(B, highest_Q_length, num_of_expansions);
					break;
				}
				
//				printBoard( B );
//				
//				auto currentPath = std::make_shared<std::string>(getMoveHistoryString( B ));
//				for ( auto& cp : *currentPath )
//					std::cout << cp << " > ";
//				std::cout << "\n\n";
				
				
				expandedStack = spawnBoardMovesFrom( B );
				++num_of_expansions;
				
				while (!( expandedStack.empty() ))	{
					auto& top = expandedStack.top();
					if (( visitedList[top->getHash()] == false ) && ( getPathLength(top) != depth_stop ))
						qStack.push( top );
					expandedStack.pop();
				}
			}
			//	end PDS dive at depth_stop
			if ( success )	break;
		}
		
		if (!( success ))
			print_FAIL();
	}	//	end PDS.
	
	
	// BREADTH-FIRST SEARCH:
	{
		std::cout << "BREADTH-FIRST SEARCH\n\n";
		
		size_t highest_Q_length = 0;
		uint32_t num_of_expansions = 0;
		
		std::queue<std::shared_ptr<Board>> BreadthFirst_Q;	//	FIFO Enqueued List!
		std::stack<std::shared_ptr<Board>> tempStack;
		std::stack<std::shared_ptr<Board>> expandedStack;
		std::bitset<domain_size> visitedList;			//	the same type gets used for both visited and strict expanded list.
		//	a bitset is initialised by default with all bits set to 0 (false).

		std::shared_ptr<Board> start = std::make_shared<Board>( start4State );
		BreadthFirst_Q.push( start );
		std::shared_ptr<Board>& B = start;


		while (!( BreadthFirst_Q.empty() ))	{
			if (highest_Q_length < BreadthFirst_Q.size() )	highest_Q_length = BreadthFirst_Q.size();
			B = BreadthFirst_Q.front();
			BreadthFirst_Q.pop();
			if ( testForGoalState(B) ) {
				print_SUCCESS(B, highest_Q_length, num_of_expansions);
				break;
			}

			visitedList.set( B->getHash() );
			tempStack = spawnBoardMovesFrom( B );
			++num_of_expansions;
			
			
			while (!( tempStack.empty() ))	{
				expandedStack.push( tempStack.top() );
				tempStack.pop();
			}

			while (!( expandedStack.empty() )) {
				auto& top = expandedStack.top();
				if ( visitedList[top->getHash()] == false )
					BreadthFirst_Q.push( top );
				expandedStack.pop();
			}
		}
	}
}

