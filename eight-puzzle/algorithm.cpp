#include "algorithm.h"
#include <map>
#include <set>

//	a tester function to do a print the status of flag
void print_inBitSet(const std::bitset<domain_size> &set, const Board &B) {
	std::cout << "Board has been visited:\t";
	if (set[B.getHash()])
		std::cout << "YES\n";
	else
		std::cout << "NO\n";
}

void print_SUCCESS(Board &B, size_t queueSize, uint32_t numExpansions,
                   float runtime) {
	std::cout << "SUCCESS!\n\n";
	printBoard(B);
	std::cout << std::endl;
	std::cout << "Maximum Queue Length = " << queueSize << std::endl;
	std::cout << "Path Length = " << getPathLength(B) << std::endl;
	std::cout << "Number of State Expansions = " << numExpansions << std::endl;
	std::cout << "Runtime = " << runtime << std::endl;
	std::string successPath = getMoveHistoryString(B);
	for (auto &s : successPath)
		std::cout << s << " > ";
	std::cout << "GOAL\n\n";
}

void print_FAIL(void) { std::cout << "FAILED to find GOAL.\n\n"; }

std::stack<Board_ptr>
reverseBoardStackOrder(std::stack<Board_ptr> stack) {
	std::stack<Board_ptr> temp;
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

std::array<uint32_t, 9>
stringToBoardArrayRepresentation(const std::string &initString) {
	std::array<uint32_t, 9> initialStateArray;
	int i = 0;
	for (auto &c : initString) {
		initialStateArray.data()[i++]
			= (uint32_t)(c - 48); //	converting characters in initialState string to numerical.
	}
	return initialStateArray;
}

///////////////////////////////////////////////////////////////////////////////////////////
//
// Search Algorithm:  Progressive Deepening Search with Visited List
//
////////////////////////////////////////////////////////////////////////////////////////////

std::string PDS_Visited_List(std::string const initialState,
                             std::string const goalState,
                             int &numOfStateExpansions, int &maxQLength,
                             float &actualRunningTime, int ultimateMaxDepth) {

	clock_t startTime = clock();

	// PROGRESSIVE DEEPENING SEARCH:
	printf("PROGRESSIVE DEEPENING SEARCH\n");
	bool success = false;
	std::set<hash_t> visitedList;				//	the simplest self-banacing Red-Black tree variety in the STL

	std::stack<Board_ptr> qStack;				//	LIFO Enqueued List
	
	std::stack<Board_ptr> expandedStack;		//	holds states spawned from another
												//	(i.e. the result from expanding a state)

	uint32_t depth_stop = 1;

	const Board_ptr S =
	std::make_shared<Board>(stringToBoardArrayRepresentation(initialState));
	Board_ptr B = nullptr;

	while (depth_stop != ultimateMaxDepth) {
		++depth_stop;
		visitedList.clear();
		hash_t c = ((S->getHash()) ^ (((hash_t)S->getFCost()) << 20));	//	generate entry for "enhanced" visited list
		visitedList.insert(c);
		qStack.push(S);

		while (!qStack.empty()) {
			maxQLength = std::max(maxQLength, (int)qStack.size());

			B = qStack.top();
			qStack.pop(); //	once B retrieved from qStack, pop qStack.

			if (testForGoalState(B)) {
				success = true;
				actualRunningTime = ((float)(clock() - startTime) / CLOCKS_PER_SEC);
				print_SUCCESS(*B, maxQLength, numOfStateExpansions, actualRunningTime);
				return getMoveHistoryString(B);
			}

			expandedStack = spawnBoardMovesFrom(B);
			++numOfStateExpansions;

			while (!(expandedStack.empty())) {
				auto &top = expandedStack.top();
				if (getPathLength(top) != depth_stop) {
					c = ((top->getHash()) ^ (((hash_t)top->getFCost()) << 20));
												// merging fCost w/ hash. NOTE: the
												// fCost must be cast to uint32_t as
												// it had to be a signed int for the
												// manhattan distance algorithm
					if (visitedList.count(c) == false) {	// this is the test. As a set is
															// ‘unique’, count will only
															// return 1 or 0
						qStack.push( top );
						visitedList.insert(c);	// add the relevant entry to the visited list.
					}
				}
				expandedStack.pop();
			}
		}
		//	end PDS dive at depth_stop
		if (success)
		break;
	}

	if (!(success)) {
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
                                         int &numOfStateExpansions,
                                         int &maxQLength,
                                         float &actualRunningTime) {
	clock_t startTime;
	startTime = clock();
	printf("BEST-FIRST SEARCH\n");
	bool success = false;
	std::bitset<domain_size> visited_list;	//	where positions in SEL are the
											//	hash value of a Board object.
											//	a bitset is initialised by default with
											//	all bits set to 0 (false).
	
	Board_ptr S = std::make_shared<Board>
	(stringToBoardArrayRepresentation(initialState));	//	Initialisation
														//	state (search start
														//	point). Will be
														//	immediately added to Q
	Board_ptr B = nullptr;
	std::stack<Board_ptr> expandedStack;			//	temporary stack of new
                                                    //	Board objects generated by
                                                    //	expanding a Board object
                                                    //	by calling
                                                    //	spawnBoardMovesFrom(Board)
                                                    //

	std::multimap<fCost_t, Board_ptr> enqueuedTree;
	
	std::multimap<fCost_t, Board_ptr>::iterator enqueuedTree_it;

	generateManhattanHeuristic(*S);

	enqueuedTree.insert(std::make_pair(S->getFCost(), S));	//	first element in Q...

	while (enqueuedTree.empty() == false) {
		maxQLength = std::max(maxQLength, (int)enqueuedTree.size()); //	keeping track of Q size

		B = enqueuedTree.begin()->second;
		enqueuedTree.erase(enqueuedTree.begin());		// pop the reference to B off the Q.

		if (testForGoalState(*B)) {
			success = true;
			actualRunningTime = ((float)(clock() - startTime) / CLOCKS_PER_SEC);
			print_SUCCESS(*B, maxQLength, numOfStateExpansions, actualRunningTime);
			return getMoveHistoryString(B);
		}

		expandedStack = spawnBoardMovesFrom(*B);
		++numOfStateExpansions;

		while (expandedStack.empty() == false) {
			auto &top = expandedStack.top();
			generateManhattanHeuristic(*top);
			if (visited_list[top->getHash()] == false) {
				enqueuedTree.insert(std::make_pair(top->getFCost(), top)); //	add element in Q...
				visited_list.set(top->getHash());
			}
			expandedStack.pop();
		}
	}
	
	if (!(success)) {
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
std::string uniformCost_Exp_List(std::string const initialState,
                                 std::string const goalState,
                                 int &numOfStateExpansions, int &maxQLength,
                                 float &actualRunningTime) {
	//	UNIFORM-COST SEARCH

	clock_t startTime;
	printf("UNIFORM COST SEARCH w/SEL\n");
	startTime = clock();

	bool success = false;
	std::bitset<domain_size> strict_expanded_list; //	where positions in SEL
                                                 //	are the hash value of a Board
                                                 //	object. A bitset is initialised
												 //	by default with all bits set to 0 (false).
	Board_ptr S = std::make_shared<Board>
		(stringToBoardArrayRepresentation(initialState));	//	Initialisation
															//	state (search start
															//	point).

	Board_ptr B = nullptr;								//	working pointer
	
	std::stack<Board_ptr> expandedStack;				//	temporary stack of new
													   //	Board objects generated by
													   //	expanding a Board object
                                                       //	by calling
                                                       //	spawnBoardMovesFrom(Board)

	std::multimap<fCost_t, Board_ptr> enqueuedTree;
	std::multimap<fCost_t, Board_ptr>::iterator enqueuedTree_it;
	std::map<hash_t, Board_ptr> enqHashMap;				//	using a map so
														//	we can arrange by hash
														//	for faster comparisons!
	std::map<hash_t, Board_ptr>::iterator enqHashMap_it;

	enqueuedTree.insert(std::make_pair(S->getFCost(), S));	//	first element in Q...
	enqHashMap.insert(std::make_pair(S->getHash(), S));		//	...and to proxy hash map

	while (enqueuedTree.empty() == false) {
		maxQLength = std::max(maxQLength, (int)enqueuedTree.size()); //	keeping track of Q size

		B = enqueuedTree.begin()->second;			//	top
		enqueuedTree.erase(enqueuedTree.begin());	//	pop
		if ((enqHashMap_it = enqHashMap.find(B->getHash())) != enqHashMap.end()) {
			enqHashMap.erase(enqHashMap_it);		//	pop from proxy hash map
		}

		if (testForGoalState(*B)) {
			success = true;
			actualRunningTime = ((float)(clock() - startTime) / CLOCKS_PER_SEC);
			print_SUCCESS(*B, maxQLength, numOfStateExpansions, actualRunningTime);
			return getMoveHistoryString(B);
			break;
		}

		expandedStack = spawnBoardMovesFrom(*B);
		strict_expanded_list.set(B->getHash());
		++numOfStateExpansions;

		while (expandedStack.empty() == false) {
			auto &top = expandedStack.top();
			if (strict_expanded_list[top->getHash()] == false) {
				if ((enqHashMap_it = enqHashMap.find(top->getHash())) == enqHashMap.end()) { //	Not already in queue, just directly insert it
					enqueuedTree.insert(std::make_pair(top->getFCost(), top));	//	add element in Q...
					enqHashMap.insert(std::make_pair(top->getHash(),top));		//	...and to hash map
				} else {
					if (top->getFCost() < enqHashMap_it->second->getFCost()) {
						enqueuedTree_it = enqueuedTree.find(enqHashMap_it->second->getFCost());
						while (enqueuedTree_it->second->getHash() != top->getHash()) {
							++enqueuedTree_it;
							if (enqueuedTree_it == enqueuedTree.end())
								break;
						}
						if (enqueuedTree_it != enqueuedTree.end())
							enqueuedTree.erase(enqueuedTree_it);	//	remove 'older' and more expensive entry from Q
						enqueuedTree.insert(std::make_pair(top->getFCost(), top)); //	add element in Q...
						enqHashMap.insert(std::make_pair(top->getHash(), top)); //	...and to hash map
					}
				}
			}

			expandedStack.pop();
		}
	}

	if (!(success)) {
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
	std::bitset<domain_size> strict_expanded_list; //	where positions in SEL
                                                 //are the hash value of a Board
                                                 //object.
	//	a bitset is initialised by default with all bits set to 0 (false).
	Board_ptr S = std::make_shared<Board>
		(stringToBoardArrayRepresentation(initialState));	//	Initialisation
															//state (search start
															//point). Will be
															//immediately added to Q
	Board_ptr B = nullptr;
	std::stack<Board_ptr> expandedStack;			//	temporary stack of new
                                                    //	Board objects generated by
                                                    //	expanding a Board object
                                                    //	by calling
                                                    //	spawnBoardMovesFrom(Board)

	std::multimap<fCost_t, Board_ptr> enqueuedTree;	//	where Q is a Red-Black
                                                    //	tree which supports more
													//	than one node with the same key.
													//	In this case, our fCost
													//	= pathlength + admissible heuristic.

	std::map<hash_t, Board_ptr> enqHashMap;			//	using a hash-based map as a
													//	proxy for faster search
													//	through Q.

	std::map<hash_t, Board_ptr>::iterator enqHashMap_it;	//	defining a specific
															//	iterator to access the
															//	hash-based map.

	if (heuristic == manhattanDistance)
		generateManhattanHeuristic(*S);
	else
		generateMisplacedTilesHeuristic(*S);

	enqueuedTree.insert(std::make_pair(S->getFCost(), S));	//	first element in Q...
	enqHashMap.insert(std::make_pair(S->getHash(), S));		//	...and add accompanying Board state to hash-based proxy map

	while (enqueuedTree.empty() == false) {
		maxQLength = std::max(maxQLength, (int)enqueuedTree.size()); //	keeping track of Q size

		B = enqueuedTree.begin()->second;			//	B = Board object held at root node of Q
		enqueuedTree.erase(enqueuedTree.begin());	//	pop root node.
		if ((enqHashMap_it = enqHashMap.find(B->getHash())) != enqHashMap.end()) {
			enqHashMap.erase(enqHashMap_it);	//	...the same with the accompanying
												//	reference in the hash map, no?
		}

		if (testForGoalState(*B)) {
		  success = true;
		  actualRunningTime = ((float)(clock() - startTime) / CLOCKS_PER_SEC);
		  print_SUCCESS(*B, maxQLength, numOfStateExpansions, actualRunningTime);
		  return getMoveHistoryString(B);
		  break;
		}

		expandedStack = spawnBoardMovesFrom(*B);
		strict_expanded_list.set(B->getHash());
		++numOfStateExpansions;

		while (expandedStack.empty() == false) {
			auto &top = expandedStack.top();
			if (heuristic == manhattanDistance)
				generateManhattanHeuristic(*top);
			else
				generateMisplacedTilesHeuristic(*top);
			if (strict_expanded_list[top->getHash()] == false) {
				if ((enqHashMap_it = enqHashMap.find(top->getHash())) == enqHashMap.end()) { //	Not already in queue, just directly insert it
					enqueuedTree.insert(std::make_pair(top->getFCost(), top));	//	add element in Q...
					enqHashMap.insert(std::make_pair(top->getHash(), top));		//	...and to hash map
				}
			}
			expandedStack.pop();
		}
	}

	if (!(success)) {
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
std::string aStar_Exp_List(std::string const initialState,
                           std::string const goalState,
                           int &numOfStateExpansions, int &maxQLength,
                           float &actualRunningTime,
                           heuristicFunction heuristic)	{

	clock_t startTime;
	startTime = clock();
	printf("A* w/SEL\n");
	bool success = false;
	std::bitset<domain_size> strict_expanded_list;	//	where positions in SEL
													//	are the hash value of a Board
													//	object.
													//	a bitset is initialised by default
													//	with all bits set to 0 (false).
	
	Board_ptr S = std::make_shared<Board>
      (stringToBoardArrayRepresentation(initialState));	//	Initialisation
														//	state (search start
														//	point). Will be
														//	immediately added to Q
	Board_ptr B = nullptr;
	std::stack<Board_ptr> expandedStack;				//	temporary stack of new
														//	Board objects generated by
														//	expanding a Board object,
														//	by calling
														//	spawnBoardMovesFrom(Board)

	std::multimap<fCost_t, Board_ptr> enqueuedTree;
	std::multimap<fCost_t, Board_ptr>::iterator enqueuedTree_it;
	std::map<hash_t, Board_ptr> enqHashMap;				//	using a map so
														//	we can arrange by hash
														//	for faster comparisons!
	
	std::map<hash_t, Board_ptr>::iterator enqHashMap_it;

	if (heuristic == manhattanDistance)
		generateManhattanHeuristic(*S);
	else
		generateMisplacedTilesHeuristic(*S);

	enqueuedTree.insert(std::make_pair(S->getFCost(), S));	//	Add S as first element in Q...
	enqHashMap.insert(std::make_pair(S->getHash(), S));		//	...and add accompanying
															//	Board state to hash map

	while (enqueuedTree.empty() == false) {
		maxQLength = std::max(maxQLength, (int)enqueuedTree.size()); //	keeping track of Q size

		B = enqueuedTree.begin()->second;						//	set B to root of Q
		enqueuedTree.erase(enqueuedTree.begin());				//	once it's off the queue,
																//	surely get it actually OFF
																//	the queue?
		enqHashMap_it = enqHashMap.find(B->getHash());
		if (enqHashMap_it != enqHashMap.end())	{
			enqHashMap.erase(enqHashMap_it);					//	...the same with the accompanying
		}														//	reference in the hash map, no?

		if (testForGoalState(*B)) {
			success = true;
			actualRunningTime = ((float)(clock() - startTime) / CLOCKS_PER_SEC);
			print_SUCCESS(*B, maxQLength, numOfStateExpansions, actualRunningTime);
			return getMoveHistoryString(B);
			break;
		}

		expandedStack = spawnBoardMovesFrom(*B);
		strict_expanded_list.set(B->getHash());
		++numOfStateExpansions;

		while (expandedStack.empty() == false)	{
			auto &top = expandedStack.top();
			if (heuristic == manhattanDistance)
				generateManhattanHeuristic(*top);
			else
				generateMisplacedTilesHeuristic(*top);
			if (strict_expanded_list[top->getHash()] == false)	{
				enqHashMap_it = enqHashMap.find(top->getHash());
				if (enqHashMap_it == enqHashMap.end())	{	//	Not already in queue, just directly insert it
					enqueuedTree.insert(std::make_pair(top->getFCost(), top)); //	add element in Q...
					enqHashMap.insert(std::make_pair(top->getHash(),top)); //	...and add accompanying Board state to hash map
				} else {
					if (top->getFCost() < enqHashMap_it->second->getFCost()) {	//	if new state is cheaper than old state, replace it.
						enqueuedTree_it = enqueuedTree.find(enqHashMap_it->second->getFCost());
						while (enqueuedTree_it->second->getHash() != top->getHash()) {
							++enqueuedTree_it;
						if (enqueuedTree_it == enqueuedTree.end())
							break;
						}
						if (enqueuedTree_it == enqueuedTree.end())
							enqueuedTree.erase(enqueuedTree_it);					//	delete the old entry...
						enqueuedTree.insert(std::make_pair(top->getFCost(), top));	//	...before adding the new state to Q...
						enqHashMap.insert(std::make_pair(top->getHash(), top));		//	...and to proxy hash map
					}
				}
			}
			expandedStack.pop();
		}
	}

	if (!(success)) {
		actualRunningTime = ((float)(clock() - startTime) / CLOCKS_PER_SEC);
		print_FAIL();
		return "";
	}
	return getMoveHistoryString(B);
}
