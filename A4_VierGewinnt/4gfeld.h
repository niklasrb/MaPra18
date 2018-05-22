/*****************************************************************
*  Name       : 4gfeld.h                                         *
*  Verwendung : Schnittstelle zu Praktikumsumgebung (MAPRA),     *
*               Vier-Gewinnt mit Minimax-Algorithmus             *
*  Autor      : Niklas Becker							         *
*  Datum      : Mai 2019                                 *
*  Sprache    : C++                                              *
*****************************************************************/

#ifndef VGFELD_H
#define VGFELD_H

#include <iostream>
#include <iomanip>
#include <vector>
#include <cassert>

namespace VG
{

enum Feld
{ leer, gelb, rot };

std::ostream& operator <<(std::ostream& os, const Feld& f)
	{
		if( f == leer)
			return os << "o";
		else if( f == rot)
			return os << "\033[31mr\033[0m";
		else 
			return os << "\033[33mg\033[0m"; 
	}
/*
 * Stellt ein Spielbrett dar
 * mit Hoehe n und Breite m
 * auf die Felder werden mit (i,j) zugregriffen
 * Stein setzen geht mit Setze()
 * Findet gewinner mit Gewinner()
 */
class Spielbrett
{
protected:
	std::vector<Feld> f;
	uint m, n;
public:
	Spielbrett(uint m, uint n) : m(m), n(n)
	{
		Leeren();
	}
	
	Feld& operator()(uint i, uint j)  { assert(i < Hoehe() && j < Spalten()); return f.at(j + i*Spalten()); }
	Feld operator()(uint i, uint j) const { assert(i < Hoehe() && j < Spalten()); return f.at(j + i*Spalten()); }
	
	bool Setze(uint j, Feld s) 
	{
		if(s == leer) return false;		// Kein leerer Stein
		if(j >= n) return false;		// falsche Spalte
		if((*this)(m-1, j) != leer) return false;	// Spalte voll
		uint i = m-1;
		while((i > 0 ? (*this)(i - 1, j) == leer : false)) i--; 	// Lasse stein fallen
		//std::cout << "Stein faellt auf " << i << ", " << j << std::endl;
		(*this)(i, j) = s;				
		return true;
	}
	
	bool Leer() const
	{
		for(uint i = 0; i < m*n; i++)
			if(f[i] != leer)
				return false;
		return true;
	}
	
	void Leeren()
	{
		f.resize(m*n);
		for(uint i = 0; i < m*n; i++) 
			f[i] = leer;
	}
	bool SpalteVoll(uint j) const
	{
		if((*this)(m-1, j) != leer) return true;	// Spalte voll
		return false;
	}
	
	bool Voll() const
	{
		for(uint i = 0; i < m*n; i++)
			if(f[i] == leer)
				return false;
		return true;
	}

	Feld Gewinner() const 
	{
		if(Gewinner(Feld::rot)) 
			return Feld::rot;
		if(Gewinner(Feld::gelb))
			return Feld::gelb;
		return Feld::leer;
	}
	
	
	uint Spalten() const { return n; }
	uint Hoehe() const { return m; }
	
	void Print() const 
	{
		for(int i = Hoehe()-1; i >= 0; i--) {
			for(uint j = 0; j < Spalten(); j++) {
				std::cout << '\t' << (*this)(i, j);
			}
			std::cout << std::endl;
		}
	}
	
protected: 
	bool Gewinner(Feld s) const 
	{		
		if(s == leer) return false;
		
		uint n;
		for(uint i = 0; i < Hoehe(); i++) {	// Teste horizontale 
			n = 0;
			for(uint j = 0; j < Spalten(); j++) {
				if((*this)(i, j) == s)
					n += 1;
				else
					n = 0;
				if(n >= 4)
					return true;
			}
		}
		for(uint j = 0; j < Spalten(); j++) {	// Teste vertikale 
			n = 0;
			for(uint i = 0; i < Hoehe(); i++) {
				if((*this)(i, j) == s)
					n += 1;
				else
					n = 0;
				if(n >= 4)
					return true;
			}
		}
		for(uint j = 0; j < Spalten(); j++) {	// Teste diagonale /
			n = 0;
			for(uint i = 0; i < Hoehe(); i++) {
				if(j+i >= Spalten()) break;
				if((*this)(i, j+i) == s)
					n += 1;
				else
					n = 0;
				if(n >= 4)
					return true;
			}
		}
		for(uint j = 0; j < Spalten(); j++) {	// Teste diagonale  
			n = 0;
			for(uint i = 0; i < Hoehe(); i++) {
				if(j-i >= Spalten()) break;
				if((*this)(i, j-i) == s)
					n += 1;
				else
					n = 0;
				if(n >= 4)
					return true;
			}
		}
		
		return false;
	}
};



};	// namespace VG


#endif
