
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

#include "student.h"
#include "unit.h"

template<typename T>
void SelectionSort(std::vector<T>& feld);
template<typename T>
void Bubblesort(std::vector<T>& feld);
template<typename T>
void MergeSort(std::vector<T>& feld);
template<typename T>
std::istream& operator >>(std::istream& ifs, std::vector<T>& v);
template<typename T>	
std::ostream& operator <<(std::ostream& os, const std::vector<T>& v);



int main(int argc, char** argv)
{
	std::vector<double> zahlen;
	std::vector<Student> studenten;
	std::vector<std::string> strings;
	char input;
	// Einlesen
	std::fstream fd("doubles.txt", std::ios_base::in);
	fd >> zahlen; fd.close();
	std::fstream fstu("studenten.txt", std::ios_base::in);
	fstu >> studenten; fstu.close();
	std::fstream fstr("strings.txt", std::ios_base::in);
	fstr >> strings; fstr.close();
	
	std::cout << "-- Eingelesen --" << std::endl << "doubles: " << std::endl << zahlen << std::endl;
	std::cout << "studenten: " << std::endl << studenten << std::endl;
	std::cout << "strings: " << strings << std::endl;
	// Sortieren
	do {
		std::cout << "Bitte Sortieralgorithmus auswaehlen (B/A/M): ";
		std::cin >> input;
	} while(input != 'A' && input != 'B' && input != 'M');
	switch(input) {
		case 'A':
			std::cout << "Auswahlsortieren" << std::endl;
			SelectionSort(zahlen);
			SelectionSort(studenten);
			SelectionSort(strings);
			break;
		case 'B':
			std::cout << "Bubblesort" << std::endl;
			Bubblesort(zahlen);
			Bubblesort(studenten);
			Bubblesort(strings); 
			break;
		case 'M':
			std::cout << "Mergesort" << std::endl;
			MergeSort(zahlen);
			MergeSort(studenten);
			MergeSort(strings);
			break;
	}
	// Ausgeben
	std::cout << "-- Sortiert -- " << std::endl << "doubles: " << std::endl << zahlen << std::endl;
	ergebnis(zahlen);
	std::cout << "studenten: " << std::endl << studenten << std::endl;
	ergebnis(studenten);    // crashes with segfault
	std::cout << "strings: " << std::endl <<  strings << std::endl;
	ergebnis(strings);	// linking error
	return 0;
}


template<typename T>
std::istream& operator >>(std::istream& ifs, std::vector<T>& v)
{
	v.clear(); 
	T buf; std::string line;
	while(getline(ifs, line)) {
		if(!line.empty()) {
			std::stringstream ss(line);
			ss >> buf;
			v.push_back(buf);
		}			
	}			
	return ifs;
}

template<typename T>	
std::ostream& operator <<(std::ostream& os, const std::vector<T>& v)
{
	for(uint i = 0; i < v.size(); i++)
		os << v.at(i) << (i < v.size()-1 ? '\n' : ' ');
	return os;
}


template<typename T>
void Bubblesort(std::vector<T>& feld)
{
	for(uint i = feld.size()-1; i > 0; i--) {
		for(uint j = 0; j < i; j++) {
			if( feld[j+1] < feld[j])
				std::swap(feld[j], feld[j+1]);
		}
	}
}

template<typename T>
void SelectionSort(std::vector<T>& feld)
{
	uint min_index;
	for(uint i = 0; i < feld.size()-1; i++) {
		min_index = i;
		for(uint j = i+1; j < feld.size(); j++) {
			if(feld[j] < feld[min_index])
				min_index = j;
		}
		if(i != min_index)
			std::swap(feld[i], feld[min_index]);
	}
}

template<typename T>
void MergeSort(std::vector<T>& feld)
{
	uint n = feld.size();
	if(n <= 1) return;
	uint p = n/2;
	std::vector<T> f1, f2;
	for(uint i = 0; i < p; i++)
		f1.push_back(feld.at(i));
	for(uint i = 0; i < n-p; i++)
		f2.push_back(feld.at(p+i));
	
	MergeSort(f1);
	MergeSort(f2);
	// Merge
	uint i1 = 0, i2 = 0;
	for(uint i = 0; i < n; i++) {
		if(i2 >= n-p) {
			feld.at(i) = f1.at(i1);
			i1++;
		} else if(i1 >= p) {
			feld.at(i) = f2.at(i2);
			i2++;
		} else if(f1.at(i1) <= f2.at(i2)){
			feld.at(i) = f1.at(i1); i1++;
		} else {
			feld.at(i) = f2.at(i2); i2++;
		}
	}
}


