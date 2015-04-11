#include "board_obj.h"



int factorial(int n){
  //recursive factorial function
  return (n==1 || n==0) ? 1: fact(n-1) *n;
}

const uint32_t hashBoardState(const Board& board){
  std::array<uint32_t, boardSize> boardArray = getState(board);
  return hashBoardState(boardArray);
}

const uint32_t hashBoardState(const std::array<uint32_t, boardSize> boardArray){
  //returns a UNIQUE value for every gamestate. 
  // "012345678" (solved) returns 0
  // "012345687" returns 1
  // ... 
  // "876543210" returns 362879 (highest result) 
  // '9' could be replaced with variable,
  // and cleared[] modified to be a smaller or larger array 
  std::array<unint32_t> cleared = {0,1,2,3,4,5,6,7,8};
  int permutation_id = 0;
  int location_count = 0;
  int current_num;
  
  for(int i = 0; i < boardSize; i++){
    current_num = boardArray[i];
    for(int j = 0; j < current_num; j++){
      if(cleared[j] != -1){
       location_count++; 
      }
    }
    cleared[current_num] = -1;
    permutation_id += factorial(boardSize-i-1) * location_count;
    location_count= 0;
  }  
  return permutation_id;
}