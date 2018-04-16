
#include "unit.h"
#include <iostream>

#define uint unsigned int


void Bubblesort(uint* feld, size_t n);
void SelectionSort(uint* feld, size_t n);

int main(int argc, char **argv)
{
	size_t laenge = 10000;
	uint* feld;
	/*
	std::cout << "BubbleSort" << std::endl;
	for(uint i = 1; i <= AnzahlBeispiele; i++, laenge = 10000) {
		start(i, laenge, feld);
		Bubblesort(feld, laenge);
		std::cout << "Laenge = " << laenge << std::endl;
		ergebnis(feld);
	}*/
	
	std::cout << "SelectionSort" << std::endl;
	for(uint i = 1; i <= AnzahlBeispiele; i++, laenge = 100) {
		start(i, laenge, feld);
		SelectionSort(feld, laenge);
		std::cout << "Laenge = " << laenge << std::endl;
		ergebnis(feld);
	}
	
	return 0;
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

