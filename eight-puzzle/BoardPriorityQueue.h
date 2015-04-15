//
//  BoardPriorityQueue.h
//  eight-puzzle
//
//  Created by Benjamin Rood on 14/04/15.
//  Copyright (c) 2015 Dave & Ben. All rights reserved.
//

#ifndef eight_puzzle_BoardPriorityQueue_h
#define eight_puzzle_BoardPriorityQueue_h

#include <algorithm>
#include <vector>
#include "board_obj.h"

size_t left_child_index( size_t parent_index ) { return (parent_index*2)+1; }
size_t right_child_index( size_t parent_index ) { return (parent_index*2)+2; }
size_t parent_index( size_t child_index ) { return (child_index-1)/2; }


class BoardPriorityQueue {
private:
	std::vector<Board> data;	//	can change that to dynamic
public:
	BoardPriorityQueue() {}
	~BoardPriorityQueue() {}    //	destructor
	void insert( Board& B );
	void sort();
	bool remove( Board& B );
	void print_heap();
	
	size_t size() { return data.size(); }
	
	const Board& front() {
		return data.front();
	}
	
	const Board& back() {
		return data.back();
	}
	
	// range methods...
	const Board* begin() {
		return &data[0];
	}
	const Board* end() {
		return &data[data.size()];
	}
	
};

void BoardPriorityQueue::insert( Board& B ) {
	data.push_back(B);
	size_t child = data.size()-1;
	size_t parent = 0;
	bool swapping = true;
	while ( swapping ) {
		swapping = false;
		parent = parent_index(child);
		if ( data[child] > data[parent] ) {
			std::swap( data[child], data[parent] );
			swapping = true;
			child = parent;
		}
	}
}

bool BoardPriorityQueue::remove( Board& B ) {
	if (data.size() == 0) return false;
	
	size_t index = 0;
	for (; index != data.size(); ++index) {
		auto& A = data[index];
		if (A == B)
			break;
	}
	
	std::swap( data[index], data.back() );
	data.pop_back();
	
	// now fix the heap
	
	size_t parent = index;
	size_t leftChild = left_child_index( parent );
	size_t rightChild = right_child_index( parent );
	
	bool swapping = true;
	while ( ( data[parent] < data[leftChild] || data[parent] < data[rightChild] ) && swapping )
	{
		swapping = false;
		
		if ( data[rightChild] < data[leftChild] ) {	//	follow left
			std::swap( data[leftChild], data[parent] );
			parent = leftChild;
			swapping = true;
		}
		else {	//	follow right
			std::swap( data[rightChild], data[parent] );
			parent = rightChild;
			swapping = true;
		}
		
		leftChild = left_child_index( parent );
		rightChild = right_child_index( parent );
		
		if ( leftChild >= data.size() ) break;
		else {
			if ( rightChild >= data.size() ) {
				if ( data[parent] < data[leftChild] ) {
					std::swap( data[leftChild], data[parent] );
					break;
				}
			}
		}
		
	}
	return true;
}

#endif
