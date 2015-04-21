//
//  main.cpp
//  eight-puzzle
//
//  Created by Benjamin Rood on 9/04/15.
//  Copyright (c) 2015 Dave & Ben. All rights reserved.
//

#include "algorithm.h"
#include <map>



int main(int argc, const char * argv[])
{
	const std::string goalString = "012345678";
	const std::string s1 = "042158367";
	const std::string s2 = "168342750";
	const std::string s3 = "481302675";
	const std::string s4 = "876543210";
	const std::string s5 = "123804765";
	const std::string simple1 = "102345678";
	const std::string simple2 = "312645078";
	const std::string simple3 = "312645708";
	
	const std::string& initialState = s1;
	
	int numOfStateExpansions = 0;
	int maxQLength = 0;
	float actualRunningTime = 0.0f;
	int ultimateMaxDepth = 60;
	PDS_Visited_List(initialState, goalString, numOfStateExpansions, maxQLength, actualRunningTime, ultimateMaxDepth);
//	bestFirstSearch_Visited_List(initialState, goalString, numOfStateExpansions, maxQLength, actualRunningTime);
//	uniformCost_Exp_List(initialState, goalString, numOfStateExpansions, maxQLength, actualRunningTime);
//	aStar(initialState, goalString, numOfStateExpansions, maxQLength, actualRunningTime, misplacedTiles);
//	aStar_Exp_List(initialState, goalString, numOfStateExpansions, maxQLength, actualRunningTime, misplacedTiles);
//
	
	clock_t startTime = clock();
	
	
	// PROGRESSIVE DEEPENING SEARCH:
	printf("PROGRESSIVE DEEPENING SEARCH\n");
	bool success = false;
	std::map<hash_t, std::shared_ptr<Board>> visitedList;				//	using a map so we can arrange by hash for faster comparisons!
	std::map<hash_t, std::shared_ptr<Board>>::iterator visitedList_It;
	
	std::stack<std::shared_ptr<Board>> qStack;		//	LIFO Enqueued List!
	std::stack<std::shared_ptr<Board>> expandedStack;
	
	uint32_t depth_stop = 1;
	
	const std::shared_ptr<Board> S = std::make_shared<Board>( stringToBoardArrayRepresentation(initialState) );
	std::shared_ptr<Board> B = nullptr;
	
	
	while ( depth_stop != ultimateMaxDepth ) {
		++depth_stop;
		visitedList.clear();
		
		qStack.push( S );
		
		while ( !qStack.empty() )	{
			maxQLength = std::max( maxQLength,  (int)qStack.size() );
			
			B = qStack.top();
			qStack.pop();	//	once B retrieved from qStack, pop qStack.
			
			if ( testForGoalState(B) )	{
				success = true;
				actualRunningTime = ((float)(clock() - startTime) / CLOCKS_PER_SEC);
				print_SUCCESS(*B, maxQLength, numOfStateExpansions, actualRunningTime);
				break;
			}
			
			expandedStack = spawnBoardMovesFrom( B );
			++numOfStateExpansions;
			
			while (!( expandedStack.empty() ))	{
				auto& top = expandedStack.top();
				if (( getPathLength(top) != depth_stop )) {								//	discard anything with a path length greater than the depth stop
					if ( visitedList.count( top->getHash() ) > 0 ) {					//	checking if anything in the visitedList has the same 'state'...
						if ( top <= visitedList.at( top->getHash() ) ) {				//	...but what if the new state is chepaer than the one already there?
							std::swap( top, visitedList.at( top->getHash() ) );			//	...then we swap current entry in visited list with more optimal one.
							qStack.push( top );	//	this also means we have to remove the older entry from the stack, surely?
						}
					}
					else {
						visitedList.insert( std::make_pair( top->getHash(), top ) );	//	add top to visited list
						qStack.push( top );
					}
				}
				expandedStack.pop();
			}
		}
		//	end PDS dive at depth_stop
		if ( success )	break;
	}
	
	if (!( success )) {
		actualRunningTime = ((float)(clock() - startTime) / CLOCKS_PER_SEC);
		print_FAIL();
	}
	
	
	// BREADTH-FIRST SEARCH:
	{
		float actualRunningTime = 0.0f;
		printf("BREADTH-FIRST SEARCH\n");
		clock_t startTime;
		startTime = clock();
		
		size_t highest_Q_length = 0;
		uint32_t numOfStateExpansions = 0;
		
		std::queue<std::shared_ptr<Board>> BreadthFirst_Q;	//	FIFO Enqueued List!
		std::stack<std::shared_ptr<Board>> tempStack;
		std::stack<std::shared_ptr<Board>> expandedStack;
		std::bitset<domain_size> visitedList;			//	the same type gets used for both visited and strict expanded list.
		//	a bitset is initialised by default with all bits set to 0 (false).

		std::shared_ptr<Board> start = std::make_shared<Board>( stringToBoardArrayRepresentation(initialState) );
		BreadthFirst_Q.push( start );
		std::shared_ptr<Board>& B = start;


		while (!( BreadthFirst_Q.empty() ))	{
			if (highest_Q_length < BreadthFirst_Q.size() )	highest_Q_length = BreadthFirst_Q.size();
			B = BreadthFirst_Q.front();
			BreadthFirst_Q.pop();
			if ( testForGoalState(B) ) {
				actualRunningTime = ((float)(clock() - startTime) / CLOCKS_PER_SEC);
				print_SUCCESS(*B, highest_Q_length, numOfStateExpansions, actualRunningTime);
				break;
			}

			visitedList.set( B->getHash() );
			tempStack = spawnBoardMovesFrom( B );
			++numOfStateExpansions;
			
			
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

