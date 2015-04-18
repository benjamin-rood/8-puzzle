#include "algorithm.h"

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
		//			std::cout << "Begin >> Progressive Deepening Search:\tDepth = " << depth_stop-1 << "\n\n";
		
		start = std::make_shared<Board>( start4State );
		qStack.push( start );
		
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
	
	//	end PDS.


	actualRunningTime = ((float)(clock() - startTime) / CLOCKS_PER_SEC);
	return getMoveHistoryString(B);
}

///////////////////////////////////////////////////////////////////////////////////////////
//
// Search Algorithm:  Uniform Cost Search using the Strict ExpandedList
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
// Search Algorithm:  A* without the ExpandedList
//
////////////////////////////////////////////////////////////////////////////////////////////
std::string aStar(std::string const initialState, std::string const goalState,
             int &numOfStateExpansions, int &maxQLength,
             float &actualRunningTime, heuristicFunction heuristic) {
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
// Search Algorithm:  A* using the Strict ExpandedList
//
////////////////////////////////////////////////////////////////////////////////////////////
std::string aStar_Exp_List(std::string const initialState, std::string const goalState,
                      int &numOfStateExpansions, int &maxQLength,
                      float &actualRunningTime, heuristicFunction heuristic) {
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
