#ifndef HEAP_H
#define HEAP_H

#include "unit.h"
#include <vector>
#include <cmath>
#include <cassert>


struct heap
{
protected:
	std::vector<uint> h_;
	
public:
	static inline size_t left(size_t n)  { return 2*n+1; }
	static inline size_t right(size_t n ) { return 2*n+2; }
	static inline size_t parent(size_t n) { return std::floor((n-1)/2.); }
	
	void heapify(size_t n)
	{
		if(left(n) >= h_.size())	// no children
			return; 
		if(right(n)  >= h_.size()) { // one child
			if(h_.at(left(n)) > h_.at(n))
				tausche(h_.data(), n, left(n));
			return;
		}	
		// two children
		if(h_.at(n) > h_.at(left(n)) && h_.at(n) > h_.at(right(n)))	// heap condition fullfilled
			return;
		if(h_.at(left(n)) >= h_.at(right(n))) {	// exchange with left child
			tausche(h_.data(), n, left(n));
			heapify(left(n));
			
		} else {	// exchange with right child
			tausche(h_.data(), n, right(n));
			heapify(right(n));
		}
	}
	
	void insert(uint v)
	{
		//std::cout << "insert " << v << std::endl;
		h_.push_back(v);
		if(h_.size() == 1)
			return;
		size_t p = h_.size()-1;
		do {
			p = parent(p);
			heapify(p);
		} while(p >0);
			
		//print();
	}
	
	uint extractMax()
	{
		assert(h_.size() > 0);
		tausche(h_.data(), 0, h_.size()-1);
		uint r = h_.back(); h_.pop_back();
		heapify(0);
		return r;
	}
	
	heap(uint* feld, size_t n)
	{
		for(size_t i = 0; i < n; i++)
			insert(feld[i]);
	}
	
	void print() 
	{
		std::cout << "[";
		for(uint i = 0; i < h_.size(); i++) std::cout << h_[i] << (i < h_.size()-1 ? ", " : "]\n");
	}
	
	
};



#endif // HEAP_H
