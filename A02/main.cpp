
#include "unit.h"
#include "heap.h"
#include <iostream>




void Test(void sortAlgorithm(uint*, size_t), const size_t gewuenschteLaenge);

void Bubblesort(uint* feld, size_t n);
void SelectionSort(uint* feld, size_t n);
void InsertionSort(uint* feld, size_t n);
void Quicksort(uint* feld, size_t n);
void QuicksortMedian3(uint* feld, size_t n);
void MergeSort(uint* feld, size_t n);
void HeapSort(uint* feld, size_t n);

int main(int argc, char **argv)
{
	size_t laenge = 1000;
	
	std::cout << "---BubbleSort" << std::endl;
	Test(&Bubblesort, laenge);
	std::cout << "---SelectionSort" << std::endl;
	Test(&SelectionSort, laenge);
	std::cout << "---InsertionSort" << std::endl;
	Test(&InsertionSort, laenge);
	std::cout << "---QuickSort" << std::endl;
	Test(&Quicksort, laenge);
	std::cout << "---QuickSort Median3" << std::endl;
	Test(&QuicksortMedian3, laenge);
	std::cout << "---MergeSort" << std::endl;
	Test(&MergeSort, laenge);
	std::cout << "---HeapSort" << std::endl;
	Test(&HeapSort, laenge);
	
	return 0;
}

void Test(void sortAlgorithm(uint*, size_t), const size_t gewuenschteLaenge)
{
	size_t laenge = gewuenschteLaenge;
	uint* feld;
	for(uint i = 1; i <= AnzahlBeispiele; i++, laenge = gewuenschteLaenge) {
		start(i, laenge, feld);
		sortAlgorithm(feld, laenge);
		std::cout << "Laenge = " << laenge << std::endl;
		ergebnis(feld);
	}
}

void Bubblesort(uint* feld, size_t n)
{
	for(size_t i = n-1; i > 0; i--) {
		for(size_t j = 0; j < i; j++) {
			if(feld[j] > feld[j+1])
				tausche(feld, j, j+1);
		}
	}
}

void SelectionSort(uint* feld, size_t n)
{
	size_t min_index;
	for(size_t i = 0; i < n-1; i++) {
		min_index = i;
		for(size_t j = i+1; j < n; j++) {
			if(feld[j] < feld[min_index])
				min_index = j;
		}
		if(i != min_index)
			tausche(feld, i, min_index);
	}
}

void InsertionSort(uint* feld, size_t n)
{
	size_t j;
	for(size_t i = 1; i < n; i++) {
		j = i;
		while(j > 0 && feld[j] < feld[j-1]) {
			tausche(feld, j, j-1);
			j--;
		} 
	}
}

size_t DivideAndConquer(uint* feld, size_t n) 
{
	size_t i, j, pivot;
	i = 0; j = n-2;
	pivot = n-1;
	while(i < j) {
		while(feld[i] < feld[pivot] && i < pivot)
			i++;
		while(feld[j] >= feld[pivot] && j > 0)
			j--;
		if(i < j)
			tausche(feld, i, j);
	}
	if(feld[i] > feld[pivot])
		tausche(feld, i, pivot);
	return i;
}



void Quicksort(uint* feld, size_t n, bool median3 = false)
{
	if(n <= 1) return;
	size_t pivot = n-1;
	if(median3 && n > 2) {
		if((feld[0] <= feld[n/2] && feld[n/2] <= feld[n-1]) || (feld[n-1] <= feld[n/2] && feld[n/2] <= feld[0]))
			pivot = n/2;
		if((feld[n/2] <= feld[0] && feld[0] <= feld[n-1]) || (feld[n-1] <= feld[0] && feld[0] <= feld[n/2]))
			pivot = 0;
		tausche(feld, pivot, n-1);
	}
	size_t i = DivideAndConquer(feld, n);
	Quicksort(feld, i, median3);
	Quicksort(feld + i+1, n-i-1, median3);
}

void Quicksort(uint* feld, size_t n)		{	Quicksort(feld, n, false);	}
void QuicksortMedian3(uint* feld, size_t n)	{	Quicksort(feld, n, true);	}

void MergeSort(uint* feld, size_t n)
{
	if(n <= 1) return;
	size_t p = n/2;
	uint* f1 = new uint[p];
	uint* f2 = new uint[n-p];
	for(size_t i = 0; i < p; i++)
		f1[i] = feld[i];
	for(size_t i = 0; i < n-p; i++)
		f2[i] = feld[p+i];
	MergeSort(f1, p);
	MergeSort(f2, n-p);
	// Merge
	size_t i1 = 0, i2 = 0;
	for(size_t i = 0; i < n; i++) {
		if((f1[i1] <= f2[i2] && i1 < p) || i2 >= n-p ) {
			feld[i] = f1[i1];
			i1++;
		} else  {
			feld[i] = f2[i2];
			i2++;
		}
	}
	//for(size_t i = 0; i < n; i++)
	//	std::cout << feld[i] << (i < n-1 ? '\t' : '\n');
	
	delete [] f1; delete [] f2;
}

void HeapSort(uint* feld, size_t n)
{
	heap H(feld, n);
	for(size_t i = n; i > 0; i--)
		feld[i-1] = H.extractMax();
}
