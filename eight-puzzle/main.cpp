//
//  main.cpp
//  eight-puzzle
//
//  Created by Benjamin Rood on 9/04/15.
//

#include <iomanip>
#include "algorithm.h"

using namespace std;

void AnimateSolution(string const initialState, string const goalState, string path){
	
	int step=1;
	
	cout << endl << "--------------------------------------------------------------------" << endl;
	
	if (path==""){
		cout << endl << "Nothing to animate." << endl;
	} else {
		cout << endl << "Animating solution..." << endl;
		cout << "Plan of action = " << path << endl;
	}
	
	Board_ptr B = std::make_shared<Board>(stringToBoardArrayRepresentation(initialState));
	string strState;
	
	strState = getStateString(B);
	//displayBoard(strState);
	
	cout << "--------------------------------------------------------------------" << endl;
	
	for(int i=0; i < path.length(); i++){
		
		cout << endl << "Step #" << step << ")  ";
		switch(path[i]){
				
			case 'U': B = std::make_shared<Board>(*B, UP); cout << "[UP]" << endl;
				break;
			case 'D': B = std::make_shared<Board>(*B, DOWN); cout << "[DOWN]" << endl;
				break;
			case 'L': B = std::make_shared<Board>(*B, LEFT); cout << "[LEFT]" << endl;
				break;
			case 'R': B = std::make_shared<Board>(*B, RIGHT); cout << "[RIGHT]" << endl;
				break;
		}
		strState = getStateString(B);
		//displayBoard(strState);
		
		step++;
	}
	
	
	cout << endl << "Animation done." << endl;
	cout << "--------------------------------------------------------------------" << endl;
	
	//getch();
}

int main(int argc, const char * argv[])
{
	string path;

	if(argc < 5){
		cout << "SYNTAX: main.exe <TYPE_OF_RUN = batch_run/single_run> ALGORITHM_NAME \"INITIAL STATE\" \"GOAL STATE\" " << endl;
		exit(0);
	}

	string typeOfRun(argv[1]);
	string algorithmSelected(argv[2]);
	string initialState(argv[3]);
	string goalState(argv[4]);


	size_t pathLength=0;
	//int depth=0;
	int numOfStateExpansions=0;
	int maxQLength=0;
	float actualRunningTime=0.0f;
	
	
	
	//=========================================================================================================

	if(typeOfRun == "single_run") {
		cout << endl << "============================================<< EXPERIMENT RESULTS >>============================================" << endl;
	}

	//=========================================================================================================
	//Run algorithm
	if(algorithmSelected == "PDS_Visited_List" ){
		path = PDS_Visited_List(initialState,  goalState, numOfStateExpansions, maxQLength, actualRunningTime, 500);

	} else if(algorithmSelected == "bestFirstSearch_Visited_List" ){
		path = bestFirstSearch_Visited_List(initialState, goalState, numOfStateExpansions, maxQLength, actualRunningTime);

	}  else if(algorithmSelected == "uniformCost_Exp_List" ){
		path = uniformCost_Exp_List(initialState, goalState, numOfStateExpansions, maxQLength, actualRunningTime);

	}  else if(algorithmSelected == "aStar_Misplaced" ){
		path = aStar(initialState, goalState, numOfStateExpansions, maxQLength, actualRunningTime, misplacedTiles);

	}  else if(algorithmSelected == "aStar_Manhattan" ){
		path = aStar(initialState, goalState, numOfStateExpansions, maxQLength, actualRunningTime, manhattanDistance);

	}  else if(algorithmSelected == "aStar_Exp_List_Misplaced" ){
		path = aStar_Exp_List(initialState, goalState, numOfStateExpansions, maxQLength, actualRunningTime, misplacedTiles);

	}  else if(algorithmSelected == "aStar_Exp_List_Manhattan" ){
		path = aStar_Exp_List(initialState, goalState, numOfStateExpansions, maxQLength, actualRunningTime, manhattanDistance);
	}

	pathLength = path.size();

	//Display name of algorithm
	if(algorithmSelected == "PDS_Visited_List" ){
		cout << setw(28) << std::left << "1) PDS_Visited_List";

	} else if(algorithmSelected == "bestFirstSearch_Visited_List" ){
		cout << setw(28) << std::left << "2) Best_First_Visited_List";
	} else if(algorithmSelected == "BFS_Visited_List" ){
		cout << setw(28) << std::left << "2) BFS_Visited_List";

	}  else if(algorithmSelected == "uniformCost_Exp_List" ){
		cout << setw(28) << std::left << "3) UC_Exp_List";

	}  else if(algorithmSelected == "aStar_Misplaced" ){
		cout << setw(28) << std::left << "4) ASTAR_Misplaced";

	}  else if(algorithmSelected == "aStar_Manhattan" ){
		cout << setw(28) << std::left << "5) ASTAR_Manhattan";

	}  else if(algorithmSelected == "aStar_Exp_List_Misplaced" ){
		cout << setw(28) << std::left << "6) ASTAR_Exp_Misplaced";

	}  else if(algorithmSelected == "aStar_Exp_List_Manhattan" ){
		cout << setw(28) << std::left << "7) ASTAR_Exp_Manhattan";

	}


	if(typeOfRun == "batch_run"){

		cout  << setprecision(6) << std::setfill(' ')   << std::fixed << std::right << ' ' << setw(10) << pathLength;
		cout  << setprecision(6) << std::setfill(' ')   << std::fixed << std::right << ' ' <<  setw(10) << numOfStateExpansions;
		cout  << setprecision(6) << std::setfill(' ')   << std::fixed << std::right << ' ' <<  setw(15) << maxQLength;
		cout  << setprecision(6) << std::setfill(' ')   << std::fixed << std::right << ' ' <<  setw(15) << actualRunningTime << endl;

	} else if(typeOfRun == "single_run"){
		cout << setprecision(6) << setw(25) << std::setfill(' ') <<  std::right << endl << endl << "Initial State:" << std::fixed << ' ' << setw(12) << initialState << endl;
		cout << setprecision(6) << setw(25) << std::setfill(' ') <<  std::right << "Goal State:" << std::fixed << ' ' << setw(12) << goalState << endl;
		cout << setprecision(6) << setw(25) << std::setfill(' ') <<  std::right << endl <<  "Path Length:" << std::fixed << ' ' << setw(12) << pathLength << endl;
		cout << setprecision(6) << setw(25) << std::setfill(' ') <<  std::right << "Num Of State Expansions:" << std::fixed << ' ' << setw(12) <<  numOfStateExpansions << endl;
		cout << setprecision(6) << setw(25) << std::setfill(' ') <<  std::right << "Max Q Length:" << std::fixed << ' ' << setw(12) << maxQLength << endl;
		cout << setprecision(6) << setw(25) << std::setfill(' ') <<  std::right << "Actual Running Time:" << std::fixed << ' ' << setprecision(6) << setw(12) <<  actualRunningTime << endl;
		cout << "================================================================================================================" << endl << endl;
		
		if(path != "") {
			AnimateSolution(initialState, goalState, path);
		}

	}
	
	
	return 0;
}

