#include "algorithm.h"

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

///////////////////////////////////////////////////////////////////////////////////////////
//
// Search Algorithm:  Progressive Deepening Search with Visited List
//
////////////////////////////////////////////////////////////////////////////////////////////

std::string PDS_Visited_List(std::string const initialState, std::string const goalState,
                        int &numOfStateExpansions, int &maxQLength,
                        float &actualRunningTime, int ultimateMaxDepth)	{
	clock_t startTime = clock();
	
	std::array<uint32_t, 9> initialStateArray;
	int i = 0;
	for ( auto& c : initialState )	{
		initialStateArray.data()[i++] = (uint32_t)( c - 48 );	//	converting characters in initialState string to numerical.
	}
	
	// PROGRESSIVE DEEPING SEARCH:
	
	bool success = false;
	size_t highest_Q_length = 0;
	uint32_t num_of_expansions = 0;
	std::bitset<domain_size> visitedList;			//	the same type gets used for both visited and strict expanded list.
													//	a bitset is initialised by default with all bits set to 0 (false).
	
	std::stack<std::shared_ptr<Board>> qStack;		//	LIFO Enqueued List!
	std::stack<std::shared_ptr<Board>> expandedStack;
	
	uint32_t depth_stop = 1;
	uint32_t depth_max = 60;
	
	std::shared_ptr<Board> start = std::make_shared<Board>( initialStateArray );
	std::shared_ptr<Board>& B = start;
	
	std::cout << "PROGRESSIVE DEEPING SEARCH\n\n";
	
	while ( depth_stop != depth_max ) {
		++depth_stop;
		visitedList.reset();
		
		start = std::make_shared<Board>( start4State );
		qStack.push( start );
		
		while (!( qStack.empty() ))	{
			if (highest_Q_length < qStack.size() )	highest_Q_length = qStack.size();
			
			B = qStack.top();
			qStack.pop();	//	once B retrieved from qStack, pop qStack.
			
			if ( testForGoalState(B) )	{
				success = true;
				actualRunningTime = ((float)(clock() - startTime) / CLOCKS_PER_SEC);
				print_SUCCESS(*B, highest_Q_length, num_of_expansions, actualRunningTime);
				break;
			}
			
			expandedStack = spawnBoardMovesFrom( B );
			++num_of_expansions;
			
			while (!( expandedStack.empty() ))	{
				auto& top = expandedStack.top();
				visitedList.set( top->getHash() );	//	add top to visited list
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
	
	//	end PDS.


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
	std::string path;
  clock_t startTime;

  startTime = clock();

  //***********************************************************************************************************
  // BEGIN_DUMMY_CODES - YOU SHOULD DELETE THIS BLOCK OF CODES AND REPLACE WITH
  // YOUR ALGORITHM IMPLEMENTATION
  //
  // run search algorithm here
  //
  //
  numOfStateExpansions = 777; // this is for testing only
  maxQLength = 333; // this is for testing only

  long i, j;

  j = 0;
  while (j < 99999) {
    j++;
    i = 0;
    while (i < 99) {
      i++;
    }
  }
  path = "LUULD";
  //
  // END_DUMMY_CODES
  //***********************************************************************************************************
  actualRunningTime = ((float)(clock() - startTime) / CLOCKS_PER_SEC);

  return path;
}

///////////////////////////////////////////////////////////////////////////////////////////
//
// Search Algorithm:  Uniform Cost Search using the Strict ExpandedList
//
////////////////////////////////////////////////////////////////////////////////////////////
std::string uniformCost_Exp_List(std::string const initialState, std::string const goalState,
                            int &numOfStateExpansions, int &maxQLength,
                            float &actualRunningTime) {
	clock_t startTime;

	startTime = clock();

	bool success = false;
	size_t highest_Q_length = 0;
	uint32_t num_of_expansions = 0;
	std::bitset<domain_size> strict_expanded_list;						//	where positions in SEL are the hash value of a Board object.
																		//	a bitset is initialised by default with all bits set to 0 (false).
																		//std::shared_ptr<Board> B = nullptr;
	std::shared_ptr<Board> S = std::make_shared<Board>( start2State );	//	Initialisation state (search start point). Will be immediately added to Q
	std::shared_ptr<Board> B = nullptr;
	std::stack<std::shared_ptr<Board>> expandedStack;					//	temporary stack of new Board objects generated by expanding a Board object ( by calling spawnBoardMovesFrom(Board) )
																		//std::shared_ptr<Board> B = nullptr;									//	working pointer
	std::vector<Board> min_heap_Q;										//	vector used as a Min Heap
	
	min_heap_Q.push_back( *S );											//	first element in Q - technically with only one element it is still a 'good' heap.
	
	while ( min_heap_Q.empty() == false )
	{
		if ( highest_Q_length < min_heap_Q.size() )						//	keeping track of Q size
			highest_Q_length = min_heap_Q.size();
		
		B = std::make_shared<Board>(min_heap_Q.front());
		if ( testForGoalState( B ) )	{
			success = true;
			actualRunningTime = ((float)(clock() - startTime) / CLOCKS_PER_SEC);
			print_SUCCESS(*B, highest_Q_length, num_of_expansions, actualRunningTime);
			break;
		}
		expandedStack = spawnBoardMovesFrom(B);
		std::pop_heap( min_heap_Q.begin(),min_heap_Q.end(), std::greater<Board>() );
		min_heap_Q.pop_back();
		strict_expanded_list.set(B->getHash());
		++num_of_expansions;
		
		while (expandedStack.empty() == false) {
			auto& top = expandedStack.top();
			
			if ( strict_expanded_list[top->getHash()] == false ) {
				min_heap_Q.push_back(*top);
				std::push_heap(min_heap_Q.begin(),min_heap_Q.end(), std::greater_equal<Board>() );
			}
			
			expandedStack.pop();
		}
	}
	if (!( success )) print_FAIL();
	
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
	
	bool success = false;
	size_t highest_Q_length = 0;
	uint32_t num_of_expansions = 0;
	
	std::shared_ptr<Board> S = std::make_shared<Board>( start2State );	//	Initialisation state (search start point). Will be immediately added to Q
	std::shared_ptr<Board> B = nullptr;
	std::stack<std::shared_ptr<Board>> expandedStack;					//	temporary stack of new Board objects generated by expanding a Board object ( by calling spawnBoardMovesFrom(Board) )
																		//std::shared_ptr<Board> B = nullptr;									//	working pointer
	std::vector<Board> min_heap_Q;										//	vector used as a Min Heap
	
	generateManhattanHeuristic(*S);
//	generateMisplacedTilesHeuristic(*S);
	min_heap_Q.push_back( *S );											//	first element in Q - technically with only one element it is still a 'good' heap.
	
	while ( min_heap_Q.empty() == false )
	{
		if ( highest_Q_length < min_heap_Q.size() )						//	keeping track of Q size
			highest_Q_length = min_heap_Q.size();
		
		B = std::make_shared<Board>(min_heap_Q.front());
		if ( testForGoalState( B ) )	{
			success = true;
			actualRunningTime = ((float)(clock() - startTime) / CLOCKS_PER_SEC);
			print_SUCCESS(*B, highest_Q_length, num_of_expansions, actualRunningTime);
			break;
		}
		expandedStack = spawnBoardMovesFrom(B);
		std::pop_heap( min_heap_Q.begin(),min_heap_Q.end(), std::greater<Board>() );
		min_heap_Q.pop_back();
		++num_of_expansions;
		
		while (expandedStack.empty() == false) {
			auto& top = expandedStack.top();
			generateManhattanHeuristic(*top);
//			generateMisplacedTilesHeuristic(*top);
			min_heap_Q.push_back(*top);
			std::push_heap(min_heap_Q.begin(),min_heap_Q.end(), std::greater_equal<Board>() );
			expandedStack.pop();
		}
	}
	if (!( success )) print_FAIL();
	
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
	
	bool success = false;
	size_t highest_Q_length = 0;
	uint32_t num_of_expansions = 0;
	std::bitset<domain_size> strict_expanded_list;						//	where positions in SEL are the hash value of a Board object.
																		//	a bitset is initialised by default with all bits set to 0 (false).
																		//std::shared_ptr<Board> B = nullptr;
	std::shared_ptr<Board> S = std::make_shared<Board>( start2State );	//	Initialisation state (search start point). Will be immediately added to Q
	std::shared_ptr<Board> B = nullptr;
	std::stack<std::shared_ptr<Board>> expandedStack;					//	temporary stack of new Board objects generated by expanding a Board object ( by calling spawnBoardMovesFrom(Board) )
																		//std::shared_ptr<Board> B = nullptr;									//	working pointer
	std::vector<Board> min_heap_Q;										//	vector used as a Min Heap
	
//	generateManhattanHeuristic(*S);
	generateMisplacedTilesHeuristic(*S);
	min_heap_Q.push_back( *S );											//	first element in Q - technically with only one element it is still a 'good' heap.
	
	while ( min_heap_Q.empty() == false )
	{
		if ( highest_Q_length < min_heap_Q.size() )						//	keeping track of Q size
			highest_Q_length = min_heap_Q.size();
		
		B = std::make_shared<Board>(min_heap_Q.front());
		if ( testForGoalState( B ) )	{
			success = true;
			actualRunningTime = ((float)(clock() - startTime) / CLOCKS_PER_SEC);
			print_SUCCESS(*B, highest_Q_length, num_of_expansions, actualRunningTime);
			break;
		}
		expandedStack = spawnBoardMovesFrom(B);
		std::pop_heap( min_heap_Q.begin(),min_heap_Q.end(), std::greater<Board>() );
		min_heap_Q.pop_back();
		strict_expanded_list.set(B->getHash());
		++num_of_expansions;
		
		while (expandedStack.empty() == false) {
			auto& top = expandedStack.top();
			//				generateManhattanHeuristic(*top);
			generateMisplacedTilesHeuristic(*top);
			if ( strict_expanded_list[top->getHash()] == false ) {
				min_heap_Q.push_back(*top);
				std::push_heap(min_heap_Q.begin(),min_heap_Q.end(), std::greater_equal<Board>() );
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
