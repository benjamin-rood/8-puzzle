
#include <queue>
#include <vector>
#include "algorithm.h"
#include <bitset>

const uint32_t domain_size = 362880;	//	has to be a const variable for it to be used in std::bitset<size of bit array>
					//	makes sense for this to be a global variable

std::bitset<domain_size> visitedList;	//	our Bit Set visited list! sizeof(visitedList) = 45KB
					//	all bits are by default 0 (false) until set true.






	



//using namespace std;


// Search Algorithm:  Progressive Deepening Search with Visited List
string PDS_Visited_List(string const initialState, string const goalState, int &numOfStateExpansions, int& maxQLength, float &actualRunningTime, int ultimateMaxDepth){
	string path;
	clock_t startTime;	
	startTime = clock();
//***********************************************************************************************************
	//see alg from slides (slide #52)

	//variables
	int currentDepthLimit = 4; 
		//start at depth = 4, will double limit every time the search fails
	int currentQueueLength = 0;
	Puzzle* currentPuzzle = nullptr;
	std::stack<Puzzle*>* expandedNodes;
	Puzzle* expandedPuzzle = NULL;
	//DFS queue is a stack 
	std::stack<Puzzle> queueStack;
	
	//initialise values
	numOfStateExpansions = 0;	
	maxQLength = 0;
	visitedList.reset();

    int boardArray[9];
    for(int i = 0; i < 9; i++){
        boardArray[i] = (int)initialState[i] - (int)'0'; 
    }
  
 	currentPuzzle = new Puzzle(boardArray, "");
		
	//initialise queue
	queueStack.push(*currentPuzzle);//push s into queue
	//add s to visited list
	visitedList.set(currentPuzzle->getHash());

	while(!currentPuzzle->isSolved()){
		//test for depth limit
		if(currentPuzzle->getPathLength() < currentDepthLimit){
			expandedNodes = currentPuzzle->spawnMoves();
			numOfStateExpansions++;
			while(!expandedNodes->empty())
	  		{
	    		expandedPuzzle = expandedNodes->top();
	    		expandedNodes->pop();
	  	
	  			//test for match in visited list
				if(!visitedList[expandedPuzzle->getHash()]){
					//add to queue
					queueStack.push(*expandedPuzzle);
					currentQueueLength++;
					maxQLength = std::max(maxQLength, currentQueueLength);					
					//add to visited list
					//visitedList.add(expandedPuzzle)
					visitedList.set(expandedPuzzle->getHash());
				}else{
					delete expandedPuzzle;
				}
			}
			//delete vector container
			//delete expandedNodes; 
		}
		//Move to next node
		delete currentPuzzle;

		if(queueStack.empty()){
			//failed for this depth, double the depth
			currentDepthLimit += currentDepthLimit;

			//test new depth against global depth limit
			if(currentDepthLimit > ultimateMaxDepth) {
				std::cout << "Error: DFS reached Max depth limit with no solution" << std::endl;
				return "Failed";
			}
	
			//reset to begin at new depth
			currentPuzzle = new Puzzle(boardArray,"");
			queueStack.push(*currentPuzzle);
			visitedList.reset();
			//currentQueueLength = 0; shouldn't need this as queue is already empty

		}else{
			currentPuzzle = &queueStack.top(); 
			queueStack.pop(); 		//remove from vector
			currentQueueLength--;
		}
	}

	//Free memory
	while(!queueStack.empty()){
		expandedPuzzle = &queueStack.top();
		queueStack.pop();
		delete expandedPuzzle;
	}

	path = currentPuzzle->getPath();
	delete currentPuzzle;
//***********************************************************************************************************
	actualRunningTime = ((float)(clock() - startTime)/CLOCKS_PER_SEC);            
	return path;
}

// Search Algorithm:  Uniform Cost Search using the Strict ExpandedList
string bestFirstSearch_Visited_List(string const initialState, string const goalState, int &numOfStateExpansions, int& maxQLength, float &actualRunningTime){
   string path;
	clock_t startTime;
	startTime = clock();
//***********************************************************************************************************
	//see alg from slides (slide #56)

	//variables
	int currentQueueLength = 0;
	Puzzle* currentPuzzle = nullptr;
	std::stack<Puzzle*>* expandedNodes;
	Puzzle* expandedPuzzle = NULL;
	//running Queue as a vextor initally as simpler for random delete.
	std::vector<Puzzle*> queueVector;
	//std::priority_queue<Puzzle, std::vector<Puzzle> queuePriority;


	//initialise values
	numOfStateExpansions = 0;	
	maxQLength = 0;
	visitedList.reset();
		
	int boardArray[9];
    for(int i = 0; i < 9; i++){
        boardArray[i] = (int)initialState[i] - (int)'0'; 
    }
  
 	currentPuzzle = new Puzzle(boardArray,"");
 
	//initialise queue
	//push s into queue
	visitedList.set(currentPuzzle->getHash());


	while(!currentPuzzle->isSolved()){
		expandedNodes = currentPuzzle->spawnMoves();

		numOfStateExpansions++;
		while(!expandedNodes->empty())
  		{
    		expandedPuzzle = expandedNodes->top();
    		expandedNodes->pop();
  	
  			//test for match in visited list
			if(!visitedList[expandedPuzzle->getHash()]){
				//add to queue
				queueVector.push_back(expandedPuzzle);
				currentQueueLength++;
				maxQLength = std::max(maxQLength, currentQueueLength);
				//add to visited list
				//visitedList.add(expandedPuzzle);
				visitedList.set(currentPuzzle->getHash());
			}else{
				delete expandedPuzzle;
			}
		}
		//delete vector container
		//delete expandedNodes; 

		//Move to next node
		delete currentPuzzle;

		if(queueVector.empty()){
			std::cout << "Error: BFS has empty queue, no solution found" << std::endl;
			return "Failed";
		}
		Puzzle* best = queueVector[0];
		int bestVal = best->getManhattanCost();
		int index = 0;
		for(int i = 1; i < queueVector.size(); i++){
			if(bestVal > queueVector[i]->getManhattanCost()){
				best = queueVector[i];
				bestVal = best->getManhattanCost();
				index = i;
			}
		}

		currentPuzzle = best; //best based on manhattan distance
		queueVector[index] = queueVector.back();
		queueVector.pop_back();
		currentQueueLength--;
	}

	//Free memory
	while(!queueVector.empty()){
		expandedPuzzle = queueVector.back();
		queueVector.pop_back();
		delete expandedPuzzle;
	}


	path = currentPuzzle->getPath();
	delete currentPuzzle;
//***********************************************************************************************************
	actualRunningTime = ((float)(clock() - startTime)/CLOCKS_PER_SEC);
	return path;
}

// Search Algorithm:  Uniform Cost Search using the Strict ExpandedList
string uniformCost_Exp_List(string const initialState, string const goalState, int &numOfStateExpansions, int& maxQLength, float &actualRunningTime){
	string path;
	clock_t startTime;	
	startTime = clock();
	
//***********************************************************************************************************
	//see alg from slides (slide #109)

	//variables
	int currentQueueLength = 0;
	Puzzle* currentPuzzle = nullptr;
	std::stack<Puzzle*>* expandedNodes;
	int index = 0;
	bool queueCollisionFlag = false;

	Puzzle* expandedPuzzle = NULL;
	//running Queue as a vextor initally as simpler for random delete.
	std::vector<Puzzle*> queueVector;
	//std::priority_queue<Puzzle, std::vector<Puzzle> queuePriority;

	//initialise values
	numOfStateExpansions = 0;	
	maxQLength = 0;
	visitedList.reset();
		
	int boardArray[9];
    for(int i = 0; i < 9; i++){
        boardArray[i] = (int)initialState[i] - (int)'0'; 
    }
  
 	currentPuzzle = new Puzzle(boardArray, "");
 	//visitedList.set(currentPuzzle->getHash());
	//initialise queue


	while(!currentPuzzle->isSolved()){
		if(!visitedList[currentPuzzle->getHash()]){
			//add this node to expanded list
			expandedNodes = currentPuzzle->spawnMoves();
			visitedList.set(currentPuzzle->getHash());
			numOfStateExpansions++;
			while(!expandedNodes->empty())
	  		{
	    		expandedPuzzle = expandedNodes->top();
	    		expandedNodes->pop();
	  	
	  			//test for match in expanded list
				if(!visitedList[expandedPuzzle->getHash()]){
					//check if there is a path collision in the queue
					for (int i = 0; i < queueVector.size(); i++){ //Puzzles in queue
						if(expandedPuzzle->getHash() == queueVector[i]->getHash()){ //compare hashes only, no fcost inclusion
							//replace one already in queue only if lower fcost
							queueCollisionFlag = true;
							if(queueVector[i]->getPathCost() > expandedPuzzle->getPathCost()){
								//replace Puzzle from queue with this lower cost one
								delete queueVector[i];
								queueVector[i] = expandedPuzzle;
															
							}else{
								delete expandedPuzzle;
							}
						}
					}
					if(queueCollisionFlag == false){
						queueVector.push_back(expandedPuzzle);
						currentQueueLength++;
						maxQLength = std::max(maxQLength, currentQueueLength);
					}
					queueCollisionFlag = false;
				}else{
					delete expandedPuzzle;
				}
			}
			//delete vector container
		}	//delete expandedNodes; 

		//Move to next node
		delete currentPuzzle;

		if(queueVector.empty()){
			std::cout << "Error: UCS has an empty queue, no solution found" << std::endl;
			return "Failed";
		}
		Puzzle* best = queueVector[0];
		int bestVal = best->getPathCost();
		index = 0;
		for(int i = 1; i < queueVector.size(); i++){
			if(bestVal > queueVector[i]->getPathCost()){
				best = queueVector[i];
				bestVal = best->getPathCost();
				index = i;
			}
		}

		currentPuzzle = best; //best based on path length
		cout << "new current path = " << currentPuzzle->getPath() << endl;  
		queueVector[index] = queueVector.back();
		queueVector.pop_back();
		currentQueueLength--;
	}

	//Free memory
	while(!queueVector.empty()){
		expandedPuzzle = queueVector.back();
		queueVector.pop_back();
		delete expandedPuzzle;
	}
	

	path = currentPuzzle->getPath();
	delete currentPuzzle;
//***********************************************************************************************************
	actualRunningTime = ((float)(clock() - startTime)/CLOCKS_PER_SEC);
	             
	return path;
}

/*

// Search Algorithm:  A* without the ExpandedList
string aStar(string const initialState, string const goalState, int &numOfStateExpansions, int& maxQLength, float &actualRunningTime, heuristicFunction heuristic){
   string path;
	clock_t startTime;
	
	startTime = clock();
	
//***********************************************************************************************************
	//see alg from slides (slide #109)

	//variables
	int currentQueueLength = 0;
	Puzzle* currentPuzzle = nullptr;
	std::stack<Puzzle*>* expandedNodes;
	Puzzle* expandedPuzzle = NULL;
	//running Queue as a vextor initally as simpler for random delete.
	std::vector<Puzzle> queueVector;
	//std::priority_queue<Puzzle, std::vector<Puzzle> queuePriority;

	//initialise values
	numOfStateExpansions = 0;	
	maxQLength = 0;
	visitedList.reset();
		
	int boardArray[9];
    for(int i = 0; i < 9; i++){
        boardArray[i] = (int)initialState[i] - (int)'0'; 
    }
  
 	currentPuzzle = new Puzzle(boardArray);
	//initialise queue
	//push s into queue
	//add s to expanded list

	while(!currentPuzzle->isSolved()){
		expandedNodes = currentPuzzle->spawnMoves();
		numOfStateExpansions++;
		while(!expandedNodes->empty())
  		{
    		expandedPuzzle = expandedNodes->top();
    		expandedNodes->pop();
  	
  			//test for match in expanded list
			if(!findInList(expandedPuzzle->getHash())){
				//check if there is a path collision in the queue
				for (auto& PuzzleFromQueue : queue){ //Puzzles in queue
					if(expandedPuzzle->getHash() == PuzzleFromQueue->getHash()) //compare hashes only, no fcost inclusion
						//replace one already in queue only if lower fcost
						if(PuzzleFromQueue.getFCost() > expandedPuzzle.getFCost()){
							//delete Puzzle from queue
							//push currentPuzzle onto queue
							queue.push_back(*expandedPuzzle);
							currentQueueLength++;
							maxQLength = std::max(maxQLength, currentQueueLength);
							
						}else{
							//delete expandedPuzzle;
						}
				}
			}else{
				//delete expandedPuzzle;
			}
		}
		//delete vector container
		//delete expandedNodes; 

		//Move to next node
		delete currentPuzzle;

		if(queue.empty()){
			std::cout << "Error: UCS has an empty queue, no solution found" << std::endl;
			return "Failed";
		}
		currentPuzzle = top(queue); //best based on path length
		pop(queue);   //remove it from vector
		currentQueueLength--;
	}

	//Free memory
	while(!queue.empty()){
		expandedPuzzle = queue.back();
		queue.pop_back();
		delete expandedPuzzle;
	}
	

	path = getMoveHistoryString(currentPuzzle);

//***********************************************************************************************************
	actualRunningTime = ((float)(clock() - startTime)/CLOCKS_PER_SEC);
	             
	return path;
}



// Search Algorithm:  A* using the Strict ExpandedList
string aStar_Exp_List(string const initialState, string const goalState, int &numOfStateExpansions, int& maxQLength, float &actualRunningTime, heuristicFunction heuristic){
   string path;
	clock_t startTime;
	
	startTime = clock();
	
//***********************************************************************************************************

//***********************************************************************************************************
	actualRunningTime = ((float)(clock() - startTime)/CLOCKS_PER_SEC);
	             
	return path;
}
*/

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
