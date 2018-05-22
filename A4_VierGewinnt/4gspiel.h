#ifndef VGSPIEL_H
#define VGSPIEL_H

#include <memory>
#include <cassert>
#include "4gfeld.h"
#include "4gspieler.h"

namespace VG
{

/*
 * Klasse, die die Spiellogik uebernimmt
 * Erwartet Spieler und Beobachter
 */
class Spiel
{
protected:
	std::vector<std::shared_ptr<Spieler> > spieler;
	std::vector<std::shared_ptr<Beobachter> > beobachter;
	Spielbrett sb;
	
public:
	Spiel(uint m, uint n, const std::shared_ptr<Spieler>& spieler1, const std::shared_ptr<Spieler>& spieler2, const std::vector<std::shared_ptr<Beobachter> >& beobachter =  std::vector<std::shared_ptr<Beobachter> >())
		: beobachter(beobachter), sb(m, n)
	{
		spieler.push_back(spieler1);
		spieler.push_back(spieler2);
	}
	
	static Feld NaechsterSpieler(Feld letzter)
	{
		return (letzter == Feld::gelb ? Feld::rot : Feld::gelb);
	}
	
	// Spiellogik
	Feld Spielen(Feld spieler1Farbe)
	{
		sb.Leeren();
		assert(spieler.size() == 2);	// Initialisiere Spieler und deren Farben
		spieler[0]->Farbe = spieler1Farbe; spieler[0]->NeuesSpiel(sb, spieler1Farbe);
		spieler[1]->Farbe = NaechsterSpieler(spieler1Farbe); spieler[1]->NeuesSpiel(sb, spieler[1]->Farbe);
		int aktiverSpieler = (spieler1Farbe == Feld::gelb ? 0 : 1);	// gelb beginnt
		Feld gewinner;
		int spalte;
		while(!sb.Voll()) {	
			do { // Warte auf gueltigen Zug
				spalte = spieler[aktiverSpieler]->Zug(sb);	
				if(spalte < 0) {				// Negative Spalte bricht ab
					std::cout << "Spieler " << spieler[aktiverSpieler]->Name << " bricht ab mit Code " << spalte << std::endl;
					return NaechsterSpieler(spieler[aktiverSpieler]->Farbe);
				}
			} while(!sb.Setze((uint) spalte, spieler[aktiverSpieler]->Farbe));
			spieler[0]->Gezogen(sb, *spieler[aktiverSpieler], (uint) spalte);	// Teile Zug Spielern und Beobachtern mit
			spieler[1]->Gezogen(sb, *spieler[aktiverSpieler], (uint) spalte);
			for(auto it = beobachter.begin(); it != beobachter.end(); ++it)
				(*it)->Gezogen(sb, *spieler[aktiverSpieler], (uint) spalte);
			gewinner = sb.Gewinner();		// Teste auf Gewinner
			if(gewinner != Feld::leer)
				return gewinner;
			aktiverSpieler = (aktiverSpieler == 0 ? 1 : 0);
		}
		return Feld::leer;
	}
};


};
#endif
