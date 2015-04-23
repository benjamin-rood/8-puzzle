//
//  main.cpp
//  eight-puzzle
//
//  Created by Benjamin Rood on 9/04/15.
//  Copyright (c) 2015 Dave & Ben. All rights reserved.
//

#include "algorithm.h"
#include <map>
#include <unordered_map>
#include <unordered_set>



int main(int argc, const char * argv[])
{
	const std::string goalString	= "012345678";
	const std::string s1			= "042158367";
	const std::string s2			= "168342750";
	const std::string s3			= "481302675";
	const std::string s4			= "876543210";
	const std::string s5			= "123804765";
	const std::string simple1		= "102345678";
	const std::string simple2		= "312645078";
	const std::string simple3		= "312645708";
	
	const std::string& initialState = s1;
	
	int numOfStateExpansions = 0;
	int maxQLength = 0;
	float actualRunningTime = 0.0f;
	int ultimateMaxDepth = 60;
	PDS_Visited_List(initialState, goalString, numOfStateExpansions, maxQLength, actualRunningTime, ultimateMaxDepth);
	bestFirstSearch_Visited_List(initialState, goalString, numOfStateExpansions, maxQLength, actualRunningTime);
	uniformCost_Exp_List(initialState, goalString, numOfStateExpansions, maxQLength, actualRunningTime);
	aStar(initialState, goalString, numOfStateExpansions, maxQLength, actualRunningTime, misplacedTiles);
	aStar_Exp_List(initialState, goalString, numOfStateExpansions, maxQLength, actualRunningTime, misplacedTiles);

	
	
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
			std::cout << *B << "\t" << B->getHash() << "\tfCost = " << B->getFCost() << std::endl;
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

