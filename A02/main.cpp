
#include "unit.h"
#include <iostream>

#define uint unsigned int


void Bubblesort(uint* feld, size_t n);

int main(int argc, char **argv)
{
	size_t laenge = 10000;
	uint* feld;
	
	for(uint i = 1; i <= AnzahlBeispiele; i++, laenge = 10000) {
		start(i, laenge, feld);
		Bubblesort(feld, laenge);
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

