//
//  main.cpp
//  eight-puzzle
//
//  Created by Benjamin Rood on 9/04/15.
//  Copyright (c) 2015 Dave & Ben. All rights reserved.
//

#include "algorithm.h"



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
	
	const std::string& initialState = s4;
	
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
		//std::bitset<domain_size> visitedList;			//	the same type gets used for both visited and strict expanded list.
		//	a bitset is initialised by default with all bits set to 0 (false).
		std::set<hash_t> visitedList;
		Board_ptr S = std::make_shared<Board>( stringToBoardArrayRepresentation(initialState) );
		generateManhattanHeuristic(S);
		std::cout << *S << "\t\t" << std::hex << S->getHash() << "\t\tfCost = " << S->getFCost() << std::endl;
		std::cout << (((hash_t)S->getFCost()) << 20) << std::endl;
		std::cout << ((S->getHash()) ^ (((hash_t)S->getFCost()) << 20)) << std::endl;
		std::cout << (((hash_t)S->getFCost()) << 20) << " +  " << S->getHash() << " = " << (((hash_t)S->getFCost()) << 20) + S->getHash() << std::endl;
		
		hash_t c = ((S->getHash()) ^ (((hash_t)S->getFCost()) << 20));
		BreadthFirst_Q.push( S );
		std::shared_ptr<Board>& B = S;
		visitedList.insert(c);


		while (!( BreadthFirst_Q.empty() ))	{
			if (highest_Q_length < BreadthFirst_Q.size() )	highest_Q_length = BreadthFirst_Q.size();
			B = BreadthFirst_Q.front();
			BreadthFirst_Q.pop();
			
			if ( testForGoalState(B) ) {
				std::cout << std::dec;
				actualRunningTime = ((float)(clock() - startTime) / CLOCKS_PER_SEC);
				print_SUCCESS(*B, highest_Q_length, numOfStateExpansions, actualRunningTime);
				break;
			}
			tempStack = spawnBoardMovesFrom( B );
			++numOfStateExpansions;
			
			
			while (!( tempStack.empty() ))	{
				expandedStack.push( tempStack.top() );
				tempStack.pop();
			}

			while (!( expandedStack.empty() )) {
				auto& top = expandedStack.top();
				c = ((top->getHash()) ^ (((hash_t)top->getFCost()) << 20));
				//std::cout << visitedList.count(c) << std::endl;
				if ( visitedList.count(c) == false ) {
					BreadthFirst_Q.push( top );
					visitedList.insert(c);
				}
				expandedStack.pop();
			}
		}
	}
}

