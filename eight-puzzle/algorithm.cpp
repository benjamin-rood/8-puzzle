
#include <queue>
#include <vector>
#include "algorithm.h"

using namespace std;


///////////////////////////////////////////////////////////////////////////////////////////
//
// Search Algorithm:  Progressive Deepening Search with Visited List
//
////////////////////////////////////////////////////////////////////////////////////////////



string PDS_Visited_List(string const initialState, string const goalState, int &numOfStateExpansions, int& maxQLength, float &actualRunningTime, int ultimateMaxDepth){
   string path;
	clock_t startTime;
	
	startTime = clock();
	
//***********************************************************************************************************
// BEGIN_DUMMY_CODES - YOU SHOULD DELETE THIS BLOCK OF CODES AND REPLACE WITH YOUR ALGORITHM IMPLEMENTATION 
	//
	// run search algorithm here
	//
	//
	numOfStateExpansions = 777; //this is for testing only
	maxQLength=333; //this is for testing only
	
	
	long i,j;
	
	j=0;
	while(j < 99999){
	   j++;
		i=0;
		while( i < 99){
		   i++;
	   }	
	}
	path = "LUULD";
	//
// END_DUMMY_CODES
//***********************************************************************************************************
	actualRunningTime = ((float)(clock() - startTime)/CLOCKS_PER_SEC);
	             
	return path;
	
}

///////////////////////////////////////////////////////////////////////////////////////////
//
// Search Algorithm:  Uniform Cost Search using the Strict ExpandedList
//
////////////////////////////////////////////////////////////////////////////////////////////
string bestFirstSearch_Visited_List(string const initialState, string const goalState, int &numOfStateExpansions, int& maxQLength, float &actualRunningTime){
   string path;
	clock_t startTime;
	
	startTime = clock();
	
//***********************************************************************************************************
// BEGIN_DUMMY_CODES - YOU SHOULD DELETE THIS BLOCK OF CODES AND REPLACE WITH YOUR ALGORITHM IMPLEMENTATION 
	//
	// run search algorithm here
	//
	//
	numOfStateExpansions = 777; //this is for testing only
	maxQLength=333; //this is for testing only
	
	
	long i,j;
	
	j=0;
	while(j < 99999){
	   j++;
		i=0;
		while( i < 99){
		   i++;
	   }	
	}
	path = "LUULD";
	//
// END_DUMMY_CODES
//***********************************************************************************************************
	actualRunningTime = ((float)(clock() - startTime)/CLOCKS_PER_SEC);
	             
	return path;
}

///////////////////////////////////////////////////////////////////////////////////////////
//
// Search Algorithm:  Uniform Cost Search using the Strict ExpandedList
//
////////////////////////////////////////////////////////////////////////////////////////////
string uniformCost_Exp_List(string const initialState, string const goalState, int &numOfStateExpansions, int& maxQLength, float &actualRunningTime){
	string path;
	clock_t startTime;
	
	startTime = clock();
	
//***********************************************************************************************************
	//see alg from slides (slide #109)

	//variables
	std::vector<board> queueVector;
	//std::priority_queue<Board, std::vector<Board> queuePriority;
	int currentQueueLength = 0;
	Board* currentBoard = new Board(initialState);
	std::vector<Board> expandedNodes;
	Board* expandedBoard = NULL;

	//initialise values
	numOfStateExpansions = 0;	
	maxQLength = 0;
		
	

	//push s into queue
	//add s to expanded list

	while(!testForGoalState(currentBoard)){
		expandedNodes = spawnBoardMovesFrom(currentBoard);
		numOfStateExpansions++;
		while(!expandedNodes.empty())
  		{
    		expandedBoard = expandedNodes.back();
    		expandedNodes.pop_back();
  	
  			//test for match in expanded list
			if(!findInList(hashBoardState(getState(expandedBoard))){
				//check if there is a path collision in the queue
				for(){ //boards in queue
					if(expandedBoard.hash == boardFromQueue.hash) //compare hashes only, no fcost inclusion
						//keep one already in queue if = or lower fcost
						if(boardFromQueue.fcost <= expandedBoard.fcost){
							//delete expandedBoard
						}else{
							//delete board from queue
							//push currentboard onto queue
							queue.push(expandedBoard);
							currentQLength++;
							maxQLength = std::max(maxQLength, currentQlength);
						}
				}
			}else{
				//delete expandedBoard;
			}
		}
		//delete vector container
		//delete expandedNodes; 

		//Move to next node
		delete currentBoard;

		if(queue.empty()){
			cout << "Error: Empty queue, no solution found"
			return "Failed";
		}
		currentBoard = top(queue);
		pop(queue);
		currentQLength--;
	}

	//Free memory
	while(!queue.empty()){
		expandedBoard = queue.back();
		queue.pop_back();
		delete expandedBoard
	}
	delete queue;

	path = getMoveHistoryString(currentBoard);

//***********************************************************************************************************
	actualRunningTime = ((float)(clock() - startTime)/CLOCKS_PER_SEC);
	             
	return path;
}



///////////////////////////////////////////////////////////////////////////////////////////
//
// Search Algorithm:  A* without the ExpandedList
//
////////////////////////////////////////////////////////////////////////////////////////////
string aStar(string const initialState, string const goalState, int &numOfStateExpansions, int& maxQLength, float &actualRunningTime, heuristicFunction heuristic){
   string path;
	clock_t startTime;
	
	startTime = clock();
	
//***********************************************************************************************************
// BEGIN_DUMMY_CODES - YOU SHOULD DELETE THIS BLOCK OF CODES AND REPLACE WITH YOUR ALGORITHM IMPLEMENTATION 
	//
	// run search algorithm here
	//
	//
	numOfStateExpansions = 777; //this is for testing only
	maxQLength=333; //this is for testing only
	
	
	long i,j;
	
	j=0;
	while(j < 99999){
	   j++;
		i=0;
		while( i < 99){
		   i++;
	   }	
	}
	path = "LUULD";
	//
// END_DUMMY_CODES
//***********************************************************************************************************
	actualRunningTime = ((float)(clock() - startTime)/CLOCKS_PER_SEC);
	             
	return path;
}



///////////////////////////////////////////////////////////////////////////////////////////
//
// Search Algorithm:  A* using the Strict ExpandedList
//
////////////////////////////////////////////////////////////////////////////////////////////
string aStar_Exp_List(string const initialState, string const goalState, int &numOfStateExpansions, int& maxQLength, float &actualRunningTime, heuristicFunction heuristic){
   string path;
	clock_t startTime;
	
	startTime = clock();
	
//***********************************************************************************************************
// BEGIN_DUMMY_CODES - YOU SHOULD DELETE THIS BLOCK OF CODES AND REPLACE WITH YOUR ALGORITHM IMPLEMENTATION 
	//
	// run search algorithm here
	//
	//
	numOfStateExpansions = 777; //this is for testing only
	maxQLength=333; //this is for testing only
	
	
	long i,j;
	
	j=0;
	while(j < 99999){
	   j++;
		i=0;
		while( i < 99){
		   i++;
	   }	
	}
	path = "LUULD";
	//
// END_DUMMY_CODES
//***********************************************************************************************************
	actualRunningTime = ((float)(clock() - startTime)/CLOCKS_PER_SEC);
	             
	return path;
}
