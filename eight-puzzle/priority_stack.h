//
//  priority_stack.h
//  eight-puzzle
//
//  Created by Benjamin Rood on 14/04/15.
//  Copyright (c) 2015 Dave & Ben. All rights reserved.
//

#ifndef eight_puzzle_priority_stack_h
#define eight_puzzle_priority_stack_h

#include <vector>

void insert( const std::vector<T>& priority_stack, const T& obj ) {
	if ( stack.empty() )	{
		stack.push_back(obj);
		return;
	}
	//binary search to find pointer to location of <= nearest neighbour of obj being inserted
	T* low = &stack.data();
	T* high = &stack.data() + stack.size();
	T* insert_position = &(stack.data() + stack.size())/2;	// initialise to mid-point
	
	while(1) {
		if (*insert_position == obj) break;
	}
}

T& top( const std::vector<T>& priority_stack ) {
	return stack.back();
}

void pop( const std::vector<T>& priority_stack ) {
	priority_stack.pop_back();
}



#endif
