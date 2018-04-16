
#include "unit.h"
#include <iostream>

#define uint unsigned int


void Test(void sortAlgorithm(uint*, size_t), const size_t gewuenschteLaenge);

void Bubblesort(uint* feld, size_t n);
void SelectionSort(uint* feld, size_t n);
void InsertionSort(uint* feld, size_t n);

int main(int argc, char **argv)
{
	size_t laenge = 10000;
	
	//std::cout << "BubbleSort" << std::endl;
	//Test(&Bubblesort, laenge)
	//std::cout << "SelectionSort" << std::endl;
	//Test(&SelectionSort, laenge)
	std::cout << "InsertionSort" << std::endl;
	Test(&InsertionSort, laenge);
	
	
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

