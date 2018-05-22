#ifndef VGKI_H
#define VGKI_H

#include "4gfeld.h"
#include "4gspieler.h"
#include "4gspiel.h"
#include <random>
#include <vector>
#include <memory>
#include <ctime>
#include <cstdlib>
#include <omp.h>

namespace VG
{
// Hilfsfunktionen
int minmax(const std::vector<int>& v, bool max = false)
	{
		assert(v.size() != 0); 
		int m = v[0];
		for(uint i = 1; i < v.size(); i++)
			if((max ? v[i] > m : v[i] < m))
				m = v[i];
		return m;		
	}

std::vector<int> minmaxIndices(const std::vector<int>& v, bool max = false)
	{
		assert(v.size() != 0);
		std::vector<int> indices; indices.push_back(0);
		for(uint i = 1; i < v.size(); i++) {
			if((max ? v[i] > v[indices[0]] : v[i] < v[indices[0]])) {
				indices.clear(); indices.push_back(i);
			} else if( v[i] == v[indices[0]] )
				indices.push_back(i);
		}
		return indices;
	}

/*
 * Eine Klasse die einen Knoten im Spielbaum darstellt
 * Ordnet dem Feld einen Wert zu
 */
class SpielBaumKnoten
{
friend class KI;
protected:
	Spielbrett S;
	int Wert;
	int spalte;		// was war der letzte Zug?
	Feld gezogen;	// und von wem
	std::vector<std::shared_ptr<SpielBaumKnoten> > Kinder;	// moegliche weitere Zuege
public:	
	// Initialisiert einen Spielbaumknoten auf Basis des uebergeben Feldes
	SpielBaumKnoten(const Spielbrett& S, Feld naechsterZug) : S(S), Wert(0), spalte (-1), gezogen(VG::Spiel::NaechsterSpieler(naechsterZug))
	{
	}
	// Initialisiert einen Spielbaumknoten auf Basis des uebergeben Feldes
	// und fuehrt den uebergebenen Zug aus!
	SpielBaumKnoten(const Spielbrett& S, int spalte, Feld gezogen) : S(S), Wert(0), spalte(spalte), gezogen(gezogen)
	{
		assert(this->S.Setze(spalte, gezogen));
	}
	
	// Implementiert eine heuristische Bewertung des Feldes fuer Spieler f
	int Schaetzen(Feld f) const
	{
		int w = 0;
		if(S.Gewinner() == f)
			return pow(2, 20);
		const int n = S.Hoehe(), m = S.Spalten();
			
		Feld gegner = VG::Spiel::NaechsterSpieler(f);
		int l, k;	// l = Anzahl der von f belegten Felder, k Anzahl der freien oder von f belegten Felder
		for(int i = 0; i < n; i++) {	// Teste horizontale 
			l = 0; k = 0;
			for(int j = 0; j < m; j++) {
				if(S(i, j) == f) {	// wenn von f belegt 
					l++; k++;
				} else if(S(i, j) == gegner) {	// wenn vom gegner belegt, setze k,l zurueck
					l = 0; k =0;
				} else 				// freies Feld
					k++;
				if(k >= 4 && l > 0)  {		// Vier freie Felder, d.h. f koennte gewinnen
					w += pow(2,l); k = 0;	// Je mehr steine l dort, desto besser
				}
			}
		}
		for(int j = 0; j < m; j++) {	// Teste vertikale 
			l = 0; k = 0;
			for(int i = 0; i < n; i++) {
				if(S(i, j) == f) {
					l++; k++;
				} else if(S(i, j) == gegner) {
					l = 0; k =0;
				} else
					k++;
				if(k >= 4&& l > 0)  {
					w += pow(2,l); k = 0;
				}
			}
		}
		for(int j = 0; j < m; j++) {	// Teste diagonale / 
			l = 0; k = 0;
			for(int i = 0; i < n; i++) {
				if(i+j >= m) break;
				if(S(i, j+i) == f) {
					l++; k++;
				} else if(S(i, j+i) == gegner) {
					l = 0; k =0;
				} else
					k++;
				if(k >= 4 && l > 0)  {
					w += pow(2,l); k = 0;
				}
			}
		}
		for(int j = 0; j < m; j++) {	// Teste diagonale /* \*/
			l = 0; k = 0;
			for(int i = 0; i < n; i++) {
				if(j-i <= 0) break;
				if(S(i, j-i) == f) {
					l++;; k++;
				} else if(S(i, j-i) == gegner) {
					l = 0; k =0;
				} else
					k++;
				if(k >= 4 && l > 0)  {
					w += pow(2,l); k = 0;
				}
			}
		}
		return w;
	}
	
	// Vergroessert den Spielbaum
	void Wachsen(int Tiefe)
	{
		if(Tiefe <= 0 || S.Voll() || S.Gewinner() != leer) { // ist Blatt
			if(Wert == 0)
				Wert = Schaetzen(gezogen) - Schaetzen(VG::Spiel::NaechsterSpieler(gezogen));	// symmetrische Bewertung
			return;
		}
		if(Kinder.size() == 0) {	// Finde moegliche Zuege
			Feld naechsterZug = VG::Spiel::NaechsterSpieler(gezogen);
			for(uint j = 0; j < S.Spalten(); j++) {
				if(!S.SpalteVoll(j)) {
					Kinder.push_back(std::make_shared<SpielBaumKnoten>(S, j, naechsterZug));
				}
			}
		}
		// Rekursiver aufruf
		#pragma omp parallel for 	// <- ist ein bisschen geschummelt
		for(uint i = 0; i < Kinder.size(); i++)
			Kinder[i]->Wachsen(Tiefe - 1);
	}
	
	// Minimax algorithmus die einen Knoten bewertet
	int MiniMax(Feld spieler)
	{
		if(Kinder.size() == 0) {	// Blatt
			if(spieler == gezogen)
				return Wert;
			else
				return -Wert;
		}
		std::vector<int> w;
		for(auto it = Kinder.begin(); it != Kinder.end(); ++it) 
			w.push_back((*it)->MiniMax(spieler));
		
		int m = minmax(w, spieler == Spiel::NaechsterSpieler(gezogen));	// finde maximum oder minimum, wie benoetigt
		return m;
	}
};

/*
 * Implementiert eine KI, die einen Spielbaum mit Tiefe t aufbaut
 * und per MiniMax und einer schlechten Heuristik den besten Zug findet
 */
class KI : public Spieler
{
protected:
	std::shared_ptr<SpielBaumKnoten> Wurzel;
	int tiefe;
public:

	KI(int tiefe = 3, std::string Name = "KI") : Spieler(Name), tiefe(tiefe)
	{
		
	}
	// Finde besten Zug
	int Zug(const Spielbrett& S)
	{
		if(!Wurzel)
			Wurzel = std::make_shared<SpielBaumKnoten>(S, this->Farbe);
		Wurzel->Wachsen(tiefe);
		if(Wurzel->Kinder.size() == 0)
			return -1;
		std::vector<int> zuegeWerte, moeglicheZuege;
		for(auto it = Wurzel->Kinder.begin(); it != Wurzel->Kinder.end(); ++it) 
			zuegeWerte.push_back((*it)->MiniMax( Farbe));
		moeglicheZuege = minmaxIndices(zuegeWerte, true);
		return Wurzel->Kinder.at(moeglicheZuege.at(std::rand() % moeglicheZuege.size()))->spalte;
	}
	// Update Spielbaum
	void Gezogen(const Spielbrett& S, const Spieler& sp, uint j)
	{
		for(auto it = Wurzel->Kinder.begin(); it != Wurzel->Kinder.end(); ++it) {
			if((*it)->spalte == (int)j) {
				Wurzel = (*it); return;
			} 
		}
		Wurzel = std::make_shared<SpielBaumKnoten>(S, VG::Spiel::NaechsterSpieler(sp.Farbe));
	}
	// Neuer Spielbaum
	void NeuesSpiel(const Spielbrett& S, Feld farbe)
	{
		std::srand(std::time(nullptr));
		Wurzel = std::make_shared<SpielBaumKnoten>(S, Feld::gelb);
		Wurzel->Wachsen(tiefe);
	}
};




};	


#endif
