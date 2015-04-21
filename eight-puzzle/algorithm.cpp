#include "algorithm.h"
#include <map>
#include <set>

//	a tester function to do a print the status of flag
void print_inBitSet( const std::bitset<domain_size>& set, const Board& B )	{
	std::cout << "Board has been visited:\t";
	if (set[B.getHash()])
		std::cout << "YES\n";
	else std::cout << "NO\n";
}

void print_SUCCESS ( Board& B, size_t queueSize, uint32_t numExpansions, float runtime )	{
	std::cout << "SUCCESS!\n\n";
	printBoard( B );
	std::cout << std::endl;
	std::cout << "Maximum Queue Length = " << queueSize << std::endl;
	std::cout << "Path Length = " << getPathLength(B) << std::endl;
	std::cout << "Number of State Expansions = " << numExpansions << std::endl;
	std::cout << "Runtime = " << runtime << std::endl;
	std::string successPath = getMoveHistoryString( B );
	for ( auto& s : successPath )
		std::cout << s << " > ";
	std::cout << "GOAL\n\n";
}

void print_FAIL ( void )	{
	std::cout << "FAILED to find GOAL.\n\n";
}

std::stack<std::shared_ptr<Board>>
reverseBoardStackOrder(std::stack<std::shared_ptr<Board>> stack) {
	std::stack<std::shared_ptr<Board>> temp;
	while (stack.empty() == false) {
		temp.push(stack.top());
		stack.pop();
	}
	
	while (temp.empty() == false) {
		stack.push(temp.top());
		temp.pop();
	}
	
	return stack;
}

std::array<uint32_t, 9> stringToBoardArrayRepresentation( const std::string& initString )	{
	std::array<uint32_t, 9> initialStateArray;
	int i = 0;
	for ( auto& c : initString )	{
		initialStateArray.data()[i++] = (uint32_t)( c - 48 );	//	converting characters in initialState string to numerical.
	}
	return initialStateArray;
}

///////////////////////////////////////////////////////////////////////////////////////////
//
// Search Algorithm:  Progressive Deepening Search with Visited List
//
////////////////////////////////////////////////////////////////////////////////////////////

std::string PDS_Visited_List(std::string const initialState, std::string const goalState,
                        int &numOfStateExpansions, int &maxQLength,
                        float &actualRunningTime, int ultimateMaxDepth)	{
	
	clock_t startTime = clock();
	
	
	// PROGRESSIVE DEEPENING SEARCH:
	printf("PROGRESSIVE DEEPENING SEARCH\n");
	bool success = false;
	std::bitset<domain_size> visitedList;			//	the same type gets used for both visited and strict expanded list.
													//	a bitset is initialised by default with all bits set to 0 (false).
	
	std::stack<std::shared_ptr<Board>> qStack;		//	LIFO Enqueued List!
	std::stack<std::shared_ptr<Board>> expandedStack;
	
	uint32_t depth_stop = 1;
	
	const std::shared_ptr<Board> S = std::make_shared<Board>( stringToBoardArrayRepresentation(initialState) );
	std::shared_ptr<Board> B = nullptr;
	
	
	while ( depth_stop != ultimateMaxDepth ) {
		++depth_stop;
		visitedList.reset();
		
		qStack.push( S );
		
		while ( !qStack.empty() )	{
			maxQLength = std::max( maxQLength,  (int)qStack.size() );
			
			B = qStack.top();
			qStack.pop();	//	once B retrieved from qStack, pop qStack.
			
			if ( testForGoalState(B) )	{
				success = true;
				actualRunningTime = ((float)(clock() - startTime) / CLOCKS_PER_SEC);
				print_SUCCESS(*B, maxQLength, numOfStateExpansions, actualRunningTime);
				return getMoveHistoryString(B);
			}
			
			expandedStack = spawnBoardMovesFrom( B );
			++numOfStateExpansions;
			
			while (!( expandedStack.empty() ))	{
				auto& top = expandedStack.top();
				if (( visitedList[top->getHash()] == false ) && ( getPathLength(top) != depth_stop ))	{
					visitedList.set( top->getHash() );	//	add top to visited list
					qStack.push( top );
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
		return "";
	}
	
	return getMoveHistoryString(B);
}

///////////////////////////////////////////////////////////////////////////////////////////
//
// Search Algorithm:  BEST FIRST SEARCH using Visited List
//
////////////////////////////////////////////////////////////////////////////////////////////
std::string bestFirstSearch_Visited_List(std::string const initialState,
										 std::string const goalState,
                                    int &numOfStateExpansions, int &maxQLength,
                                    float &actualRunningTime) {
	clock_t startTime;
	startTime = clock();
	printf("BEST-FIRST SEARCH\n");
	bool success = false;
	std::bitset<domain_size> visited_list;						//	where positions in SEL are the hash value of a Board object.
	//	a bitset is initialised by default with all bits set to 0 (false).
	std::shared_ptr<Board> S = std::make_shared<Board>( stringToBoardArrayRepresentation(initialState) );	//	Initialisation state (search start point). Will be immediately added to Q
	std::shared_ptr<Board> B = nullptr;
	std::stack<std::shared_ptr<Board>> expandedStack;					//	temporary stack of new Board objects generated by expanding a Board object ( by calling spawnBoardMovesFrom(Board) )
	
	
	std::multimap<fCost_t, std::shared_ptr<Board>> q_EnqueuedTree;
	std::multimap<fCost_t, std::shared_ptr<Board>>::iterator q_EnqueuedTree_It;
	std::map<hash_t, std::shared_ptr<Board>> q_hashMap;				//	using a map so we can arrange by hash for faster comparisons!
	std::map<hash_t, std::shared_ptr<Board>>::iterator q_hashMap_It;
	
	generateManhattanHeuristic( *S );
	
	q_EnqueuedTree.insert ( std::make_pair(S->getFCost(), S ) );		//	first element in Q...
	q_hashMap.insert ( std::make_pair(S->getHash(), S ) );			//	...and add accompanying Board state to hash map
	
	while ( q_EnqueuedTree.empty() == false )
	{
		maxQLength = std::max( maxQLength, (int)q_EnqueuedTree.size() );	//	keeping track of Q size
		
		B = q_EnqueuedTree.begin()->second;
		q_EnqueuedTree.erase( q_EnqueuedTree.begin() );	//	once it's off the queue, surely get it actually OFF the queue?
		if ( (q_hashMap_It = q_hashMap.find( B->getHash() )) != q_hashMap.end() ) {
			q_hashMap.erase( q_hashMap_It );	//	...the same with the accompanying reference in the hash map, no?
		}
		
		
		if ( testForGoalState( *B ) )	{
			success = true;
			actualRunningTime = ((float)(clock() - startTime) / CLOCKS_PER_SEC);
			print_SUCCESS(*B, maxQLength, numOfStateExpansions, actualRunningTime);
				return getMoveHistoryString(B);
			break;
		}
		
		expandedStack = spawnBoardMovesFrom( *B );
		++numOfStateExpansions;
		
		while (expandedStack.empty() == false) {
			auto& top = expandedStack.top();
			generateManhattanHeuristic( *top );
			if ( visited_list[top->getHash()] == false ) {
				if ( (q_hashMap_It = q_hashMap.find( top->getHash() )) == q_hashMap.end() )	{	//	Not already in queue, just directly insert it
					q_EnqueuedTree.insert ( std::make_pair(top->getFCost(), top) );			//	add element in Q...
					q_hashMap.insert ( std::make_pair(top->getHash(), top) );					//	...and add accompanying Board state to hash map
					visited_list.set( top->getHash() );
				}
			}
			expandedStack.pop();
		}
		
	}
	
	if (!( success )) {
		actualRunningTime = ((float)(clock() - startTime) / CLOCKS_PER_SEC);
		print_FAIL();
		return "";
	}
	
	return getMoveHistoryString(B);
}

///////////////////////////////////////////////////////////////////////////////////////////
//
// Search Algorithm:  Uniform Cost Search using the Strict ExpandedList
//
////////////////////////////////////////////////////////////////////////////////////////////
std::string uniformCost_Exp_List(std::string const initialState, std::string const goalState,
                            int &numOfStateExpansions, int &maxQLength,
                            float &actualRunningTime) {
	//	UNIFORM-COST SEARCH
	
	clock_t startTime;
	printf("UNIFORM COST SEARCH w/SEL\n");
	startTime = clock();
	
	bool success = false;
	std::bitset<domain_size> strict_expanded_list;						//	where positions in SEL are the hash value of a Board object.
	//	a bitset is initialised by default with all bits set to 0 (false).
	std::shared_ptr<Board> S = std::make_shared<Board>( stringToBoardArrayRepresentation(initialState) );	//	Initialisation state (search start point). Will be immediately added to Q
	std::shared_ptr<Board> B = nullptr;									//	working pointer
	std::stack<std::shared_ptr<Board>> expandedStack;					//	temporary stack of new Board objects generated by expanding a Board object ( by calling spawnBoardMovesFrom(Board) )
	
	
	std::multimap<fCost_t, std::shared_ptr<Board>> q_EnqueuedTree;
	std::multimap<fCost_t, std::shared_ptr<Board>>::iterator q_EnqueuedTree_It;
	std::map<hash_t, std::shared_ptr<Board>> q_hashMap;				//	using a map so we can arrange by hash for faster comparisons!
	std::map<hash_t, std::shared_ptr<Board>>::iterator q_hashMap_It;
	
	q_EnqueuedTree.insert ( std::make_pair(S->getFCost(), S ) );		//	first element in Q...
	q_hashMap.insert ( std::make_pair(S->getHash(), S ) );			//	...and add accompanying Board state to hash map
	
	while ( q_EnqueuedTree.empty() == false )
	{
		maxQLength = std::max( maxQLength, (int)q_EnqueuedTree.size() );	//	keeping track of Q size
		
		B = q_EnqueuedTree.begin()->second;
		q_EnqueuedTree.erase( q_EnqueuedTree.begin() );	//	once it's off the queue, surely get it actually OFF the queue?
		if ( (q_hashMap_It = q_hashMap.find( B->getHash() )) != q_hashMap.end() ) {
			q_hashMap.erase( q_hashMap_It );	//	...the same with the accompanying reference in the hash map, no?
		}
		
		
		if ( testForGoalState( *B ) )	{
			success = true;
			actualRunningTime = ((float)(clock() - startTime) / CLOCKS_PER_SEC);
			print_SUCCESS(*B, maxQLength, numOfStateExpansions, actualRunningTime);
				return getMoveHistoryString(B);
			break;
		}
		
		expandedStack = spawnBoardMovesFrom( *B );
		strict_expanded_list.set(B->getHash());
		++numOfStateExpansions;
		
		while (expandedStack.empty() == false) {
			auto& top = expandedStack.top();
			if ( strict_expanded_list[top->getHash()] == false ) {
				if ( (q_hashMap_It = q_hashMap.find( top->getHash() )) == q_hashMap.end() )	{	//	Not already in queue, just directly insert it
					q_EnqueuedTree.insert ( std::make_pair(top->getFCost(), top) );			//	add element in Q...
					q_hashMap.insert ( std::make_pair(top->getHash(), top) );					//	...and add accompanying Board state to hash map
				} else {
					if ( top->getFCost() < q_hashMap_It->second->getFCost() ) {
						q_EnqueuedTree_It = q_EnqueuedTree.find( q_hashMap_It->second->getFCost() );
						while ( q_EnqueuedTree_It->second->getHash() != top->getHash() ) {
							++q_EnqueuedTree_It;
							if ( q_EnqueuedTree_It == q_EnqueuedTree.end() ) break;
						}
						if ( q_EnqueuedTree_It == q_EnqueuedTree.end() )
							q_EnqueuedTree.erase( q_EnqueuedTree_It );
						q_EnqueuedTree.insert ( std::make_pair(top->getFCost(), top) );			//	add element in Q...
						q_hashMap.insert ( std::make_pair(top->getHash(), top) );					//	...and add accompanying Board state to hash map
					}
					
				}
			}
			
			expandedStack.pop();
		}
		
	}
	if (!( success )) {
		print_FAIL();
		return "";
	}
	
	return getMoveHistoryString(B);
}

///////////////////////////////////////////////////////////////////////////////////////////
//
// Search Algorithm:  A* without the ExpandedList
//
////////////////////////////////////////////////////////////////////////////////////////////
std::string aStar(std::string const initialState, std::string const goalState,
             int &numOfStateExpansions, int &maxQLength,
             float &actualRunningTime, heuristicFunction heuristic) {

	clock_t startTime;
	startTime = clock();
	printf("A*\n");
	bool success = false;
	std::bitset<domain_size> strict_expanded_list;						//	where positions in SEL are the hash value of a Board object.
	//	a bitset is initialised by default with all bits set to 0 (false).
	std::shared_ptr<Board> S = std::make_shared<Board>( stringToBoardArrayRepresentation(initialState) );	//	Initialisation state (search start point). Will be immediately added to Q
	std::shared_ptr<Board> B = nullptr;
	std::stack<std::shared_ptr<Board>> expandedStack;					//	temporary stack of new Board objects generated by expanding a Board object ( by calling spawnBoardMovesFrom(Board) )
	
	
	std::multimap<fCost_t, std::shared_ptr<Board>> q_EnqueuedTree;
	std::multimap<fCost_t, std::shared_ptr<Board>>::iterator q_EnqueuedTree_It;
	std::map<hash_t, std::shared_ptr<Board>> q_hashMap;				//	using a map so we can arrange by hash for faster comparisons!
	std::map<hash_t, std::shared_ptr<Board>>::iterator q_hashMap_It;
	
	if ( heuristic == manhattanDistance )
		generateManhattanHeuristic( *S );
	else
		generateMisplacedTilesHeuristic( *S );
	
	q_EnqueuedTree.insert ( std::make_pair(S->getFCost(), S ) );		//	first element in Q...
	q_hashMap.insert ( std::make_pair(S->getHash(), S ) );			//	...and add accompanying Board state to hash map
	
	while ( q_EnqueuedTree.empty() == false )
	{
		maxQLength = std::max( maxQLength, (int)q_EnqueuedTree.size() );	//	keeping track of Q size
		
		B = q_EnqueuedTree.begin()->second;
		q_EnqueuedTree.erase( q_EnqueuedTree.begin() );	//	once it's off the queue, surely get it actually OFF the queue?
		if ( (q_hashMap_It = q_hashMap.find( B->getHash() )) != q_hashMap.end() ) {
			q_hashMap.erase( q_hashMap_It );	//	...the same with the accompanying reference in the hash map, no?
		}
		
		
		if ( testForGoalState( *B ) )	{
			success = true;
			actualRunningTime = ((float)(clock() - startTime) / CLOCKS_PER_SEC);
			print_SUCCESS(*B, maxQLength, numOfStateExpansions, actualRunningTime);
				return getMoveHistoryString(B);
			break;
		}
		
		expandedStack = spawnBoardMovesFrom( *B );
		strict_expanded_list.set(B->getHash());
		++numOfStateExpansions;
		
		while (expandedStack.empty() == false) {
			auto& top = expandedStack.top();
			if ( heuristic == manhattanDistance )
				generateManhattanHeuristic( *top );
			else
				generateMisplacedTilesHeuristic( *top );
			if ( strict_expanded_list[top->getHash()] == false ) {
				if ( (q_hashMap_It = q_hashMap.find( top->getHash() )) == q_hashMap.end() )	{	//	Not already in queue, just directly insert it
					q_EnqueuedTree.insert ( std::make_pair(top->getFCost(), top) );			//	add element in Q...
					q_hashMap.insert ( std::make_pair(top->getHash(), top) );					//	...and add accompanying Board state to hash map
				}
			}
			expandedStack.pop();
		}
		
	}
	
	if (!( success )) {
		actualRunningTime = ((float)(clock() - startTime) / CLOCKS_PER_SEC);
		print_FAIL();
		return "";
	}
	
	return getMoveHistoryString(B);
}

///////////////////////////////////////////////////////////////////////////////////////////
//
// Search Algorithm:  A* using the Strict ExpandedList
//
////////////////////////////////////////////////////////////////////////////////////////////
std::string aStar_Exp_List(std::string const initialState, std::string const goalState,
                      int &numOfStateExpansions, int &maxQLength,
                      float &actualRunningTime, heuristicFunction heuristic) {

	clock_t startTime;
	startTime = clock();
	printf("A* w/SEL\n");
	bool success = false;
	std::bitset<domain_size> strict_expanded_list;						//	where positions in SEL are the hash value of a Board object.
																		//	a bitset is initialised by default with all bits set to 0 (false).
	std::shared_ptr<Board> S = std::make_shared<Board>( stringToBoardArrayRepresentation(initialState) );	//	Initialisation state (search start point). Will be immediately added to Q
	std::shared_ptr<Board> B = nullptr;
	std::stack<std::shared_ptr<Board>> expandedStack;					//	temporary stack of new Board objects generated by expanding a Board object ( by calling spawnBoardMovesFrom(Board) )
	
	
	std::multimap<fCost_t, std::shared_ptr<Board>> q_EnqueuedTree;
	std::multimap<fCost_t, std::shared_ptr<Board>>::iterator q_EnqueuedTree_It;
	std::map<hash_t, std::shared_ptr<Board>> q_hashMap;				//	using a map so we can arrange by hash for faster comparisons!
	std::map<hash_t, std::shared_ptr<Board>>::iterator q_hashMap_It;
	
	if ( heuristic == manhattanDistance )
		generateManhattanHeuristic( *S );
	else
		generateMisplacedTilesHeuristic( *S );
	
	q_EnqueuedTree.insert ( std::make_pair(S->getFCost(), S ) );		//	first element in Q...
	q_hashMap.insert ( std::make_pair(S->getHash(), S ) );			//	...and add accompanying Board state to hash map
	
	while ( q_EnqueuedTree.empty() == false )
	{
		maxQLength = std::max( maxQLength, (int)q_EnqueuedTree.size() );	//	keeping track of Q size
		
		B = q_EnqueuedTree.begin()->second;
		q_EnqueuedTree.erase( q_EnqueuedTree.begin() );	//	once it's off the queue, surely get it actually OFF the queue?
		if ( (q_hashMap_It = q_hashMap.find( B->getHash() )) != q_hashMap.end() ) {
			q_hashMap.erase( q_hashMap_It );	//	...the same with the accompanying reference in the hash map, no?
		}
		
		
		if ( testForGoalState( *B ) )	{
			success = true;
			actualRunningTime = ((float)(clock() - startTime) / CLOCKS_PER_SEC);
			print_SUCCESS(*B, maxQLength, numOfStateExpansions, actualRunningTime);
				return getMoveHistoryString(B);
			break;
		}
		
		expandedStack = spawnBoardMovesFrom( *B );
		strict_expanded_list.set(B->getHash());
		++numOfStateExpansions;
		
		while (expandedStack.empty() == false) {
			auto& top = expandedStack.top();
			if ( heuristic == manhattanDistance )
				generateManhattanHeuristic( *top );
			else
				generateMisplacedTilesHeuristic( *top );
			if ( strict_expanded_list[top->getHash()] == false ) {
				if ( (q_hashMap_It = q_hashMap.find( top->getHash() )) == q_hashMap.end() )	{	//	Not already in queue, just directly insert it
					q_EnqueuedTree.insert ( std::make_pair(top->getFCost(), top) );			//	add element in Q...
					q_hashMap.insert ( std::make_pair(top->getHash(), top) );					//	...and add accompanying Board state to hash map
				} else {
					if ( top->getFCost() < q_hashMap_It->second->getFCost() ) {
						q_EnqueuedTree_It = q_EnqueuedTree.find( q_hashMap_It->second->getFCost() );
						while ( q_EnqueuedTree_It->second->getHash() != top->getHash() ) {
							++q_EnqueuedTree_It;
							if ( q_EnqueuedTree_It == q_EnqueuedTree.end() ) break;
						}
						if ( q_EnqueuedTree_It == q_EnqueuedTree.end() )
							q_EnqueuedTree.erase( q_EnqueuedTree_It );
						q_EnqueuedTree.insert ( std::make_pair(top->getFCost(), top) );			//	add element in Q...
						q_hashMap.insert ( std::make_pair(top->getHash(), top) );					//	...and add accompanying Board state to hash map
					}
					
				}
			}
			
			expandedStack.pop();
		}
		
	}
	
	if (!( success )) {
		actualRunningTime = ((float)(clock() - startTime) / CLOCKS_PER_SEC);
		print_FAIL();
		return "";
	}
	
	return getMoveHistoryString(B);
}
