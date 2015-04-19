#include <stack>
#include <ctime>
#include <string>
#include "board_obj.h"
#include <algorithm>
#include <bitset>
#include <queue>

const uint32_t domain_size = 362880;
const heuristicFunction HEURISTIC_FUNCTION = manhattanDistance;

//	a tester function to do a print the status of flag
void print_inBitSet( const std::bitset<domain_size>& set, const Board& B );
void print_SUCCESS ( Board& B, size_t queueSize, uint32_t numExpansions, float runtime );
void print_FAIL ( void );

std::stack<std::shared_ptr<Board>> reverseBoardStackOrder ( std::stack<std::shared_ptr<Board>> stack );



//Progressive Deepening Seach with Visited List
std::string PDS_Visited_List(std::string const initialState, std::string const goalState, int &numOfStateExpansions, int& maxQLength, float &actualRunningTime, int ultimateMaxDepth);

//Best-First Search with Visited List
std::string bestFirstSearch_Visited_List(std::string const initialState, std::string const goalState, int &numOfStateExpansions, int& maxQLength, float &actualRunningTime);

//Uniform Cost Search with Expanded List
std::string uniformCost_Exp_List(std::string const initialState, std::string const goalState, int &numOfStateExpansions, int& maxQLength, float &actualRunningTime);

//A* Search
std::string aStar(std::string const initialState, std::string const goalState, int &numOfStateExpansions, int& maxQLength, float &actualRunningTime, heuristicFunction heuristic);

//A* using the Strict Expanded List
std::string aStar_Exp_List(std::string const initialState, std::string const goalState, int &numOfStateExpansions, int& maxQLength, float &actualRunningTime, heuristicFunction heuristic);