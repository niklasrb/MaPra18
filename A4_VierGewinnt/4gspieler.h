#ifndef VGSPIELER_H
#define VGSPIELER_H

#include "4gfeld.h"
#include "unit.h"
#include <string>

namespace VG
{

/*
 * Abstrakte Klasse die einer Spieler darstellt
 * Wird dem Spiel uebergeben und muss bei Zug() die Spalte zurueckgeben
 * Wird durch Gezogen() ueber Zuege informiert (auch eigene)
 */
class Spieler
{
public:
	Feld Farbe;
	std::string Name;
	
	Spieler(std::string Name = "") : Name(Name) {}
	
	virtual int Zug(const Spielbrett& S) = 0;
	virtual void Gezogen(const Spielbrett& S, const Spieler& sp, uint j) = 0;
	virtual void NeuesSpiel(const Spielbrett& S, Feld farbe) = 0;
};

/*
 * Spieler am Terminal
 */
class LokalerSpieler : public Spieler
{
public:
	LokalerSpieler(std::string Name = "Spieler") : Spieler(Name)
	{
		
	}

	int Zug(const Spielbrett& S)
	{
		std::cout << "Sie sind an der Reihe, Spieler " << this->Name << " einen " << this->Farbe << "en Stein zu werfen." << std::endl;
		int i;
		do {
			std::cout << "Bitte waehlen eine Spalte (0 - " << S.Spalten()-1 << ") oder geben Sie auf (-1): ";
			std::cin.clear();
			std::cin >> i;
		} while(i >= (int)S.Spalten() || i < -1);
		return i;
	}
	
	void NeuesSpiel(const Spielbrett& S, Feld farbe) { }
	
	void Gezogen(const Spielbrett& S, const Spieler& sp, uint j)
	{
		std::cout << "Spieler " << sp.Name << " hat einen " << sp.Farbe << " Stein in Spalte " << j << " geworfen" << std::endl;
		S.Print();
	}
};

/*
 * Interface zur Praktikumsumgebung
 */
class Praktikumsumgebung : public Spieler
{
	int naechsterZug;
public:
	Praktikumsumgebung(int s = 4, std::string Name = "PU") : Spieler(Name)
	{
		Start(s);
	}
	
	void NeuesSpiel(const Spielbrett& S, Feld farbe)
	{
		if(farbe == gelb)	// ueberge -1 wenn die PU anfaengt
			naechsterZug = NaechsterZug(-1);
	}

	int Zug(const Spielbrett& S)
	{
		if(naechsterZug >= 0 && naechsterZug < (int)S.Spalten())
			return naechsterZug;
		std::cout << "Praktikumsumgebung sagt " << naechsterZug << std::endl;
		return naechsterZug;
	}
	void Gezogen(const Spielbrett& S, const Spieler& sp, uint j)
	{
		if(sp.Farbe != Farbe)	// Teile PU nur den Zug des anderen mit und erhalte neuen Zug
			naechsterZug = NaechsterZug((int) j);
	}
};

/*
 * Interface fuer einen Netzwerkspieler
 */
class NetzwerkSpieler : public Spieler
{
protected:
	bool error;
public:
	NetzwerkSpieler(bool verbinde, unsigned short port, std::string ip = "", std::string Name = "Netzwerk")  : Spieler(Name)
	{
		error = false;
        if(verbinde){
			if (!Verbinde(ip.c_str(), port)) {
				std::cout << "Verbindung fehlgeschlagen." << std::endl;
				error = true;
				return;
			}
		}
		else {
			if (!WarteAufVerbindung(port)) {
				std::cout << "Kein Gegner gefunden.";
				error = true;
				return;
			}
		}
		std::cout << "Verbindung hergestellt" << std::endl;
	}
	
	void NeuesSpiel(const Spielbrett& S, Feld farbe) {}
	
	void VerbindungSchliessen() 
	{
		if(!error)
			SchliesseVerbindung();
	}
	
	~NetzwerkSpieler()
	{
		VerbindungSchliessen();
	}

	int Zug(const Spielbrett& S)
	{
		int e = -1;
		if(!error)
			e = EmpfangeZug();
		if(error || e < 0 || e >= (int)S.Spalten()) {
			std::cout << "Verbindungsfehler: " << e << std::endl;
			return VERBINDUNGSFEHLER;
		}
		return e;
	}
	void Gezogen(const Spielbrett& S, const Spieler& sp, uint j)
	{
		if(!error && sp.Farbe != Farbe)	// Sende nur Zug des anderen Spielers
			error = !SendeZug((int) j );
		if(error)
			std::cout << "Fehler bei SendeZug(" << (int)j << ")" << std::endl;
	}
};

/*
 * Bekommt die Zuege mitgeteilt um sie darstellen zu koennen
 */
class Beobachter
{
public:
	virtual void Gezogen(const Spielbrett& S, const Spieler& sp, uint j)= 0;
};

class LokalerBeobachter : public Beobachter
{
public:
	void Gezogen(const Spielbrett& S, const Spieler& sp, uint j)
	{
		std::cout << "Spieler " << sp.Name << " hat einen " << sp.Farbe << " Stein in Spalte " << j << " geworfen" << std::endl;
		S.Print();
	}
};




};	


#endif
