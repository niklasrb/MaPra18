#ifndef HEAP_H
#define HEAP_H

#include "unit.h"
#include <cmath>
#include <cassert>
#include <algorithm>


struct heap
{
protected:
	uint* h_;
	size_t max;
	size_t size;
	
public:
	static inline size_t left(size_t n)  { return 2*n+1; }
	static inline size_t right(size_t n ) { return 2*n+2; }
	static inline size_t parent(size_t n) { return std::floor((n-1)/2.); }
	
	void heapify(size_t n)
	{
		if(left(n) >= size)	// no children
			return; 
		if(right(n)  >= size) { // one child
			if(h_[left(n)] > h_[n])
				tausche(h_, n, left(n));
			return;
		}	
		// two children
		if(h_[n] > h_[left(n)] && h_[n] > h_[right(n)])	// heap condition fullfilled
			return;
		if(h_[left(n)] >= h_[right(n)]) {	// exchange with left child
			tausche(h_, n, left(n));
			heapify(left(n));
			
		} else {	// exchange with right child
			tausche(h_, n, right(n));
			heapify(right(n));
		}
	}
	
	void reserve(uint newMax)
	{
		assert(size < newMax);
		uint* hnew = new uint[newMax];
		for(size_t i = 0; i < size; i++)
			hnew[i] = h_[i];
		max = newMax;
		delete [] h_;
		h_ = hnew;
	}
	
	void insert(uint v)
	{
		if(size >= max) 
			reserve(2*max);
		size++;
		h_[size-1] = v;
		if(size == 1)
			return;
		size_t p = size-1;
		do {
			p = parent(p);
			heapify(p);
		} while(p >0);
	}
	
	uint extractMax()
	{
		assert(size > 0);
		tausche(h_, 0, size-1);
		uint r = h_[size-1]; size--;
		heapify(0);
		return r;
	}
	
	heap(uint* feld, size_t n) : max(std::max(n, (size_t)1)), size(0)
	{
		h_ = new uint[max];
		for(size_t i = 0; i < n; i++)
			insert(feld[i]);
	}
	
	~heap()
	{
		delete [] h_;
	}
	
	void print() const
	{
		std::cout << "[";
		for(uint i = 0; i < size; i++) std::cout << h_[i] << (i < size -1 ? ", " : "]\n");
	}
	
	
};



#endif // HEAP_H
