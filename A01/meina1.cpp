/***********************************************************
*  Name       : meina1.cpp                                 *
*  Verwendung : Loesung der ersten Praktikumsaufgabe,      *
*               Nullstellen eines quadratischen Polynoms   *
*  Autor      :                                            *
*  Datum      :                                            *
*  Sprache    : C++                                        *
***********************************************************/

// Einbinden der Praktikums-Umgebung. Falls Sie die Ein-/Ausgabe zuerst
// nicht ueber die Praktikumsumgebung machen wollen, sollten Sie auch noch
// #include <iostream> einbinden.

#include "unit.h"
#include <iostream>
#include <cmath>


void BestimmeNullstellen(double a, double b, double c);

#define DEBUG

#ifdef DEBUG
#define Ergebnis(...) Ergebnis(__VA_ARGS__); std::cout << __VA_ARGS__ << std::endl;
#endif

// ===== Hauptprogramm =====

int main()
{
	// Hier kommt nun Ihr Programm; viel Erfolg!
	double a, b, c;
	for(unsigned int i = 0; i < AnzahlBeispiele; i++) {
		Start(i, a, b, c);
		BestimmeNullstellen(a, b, c);
	}
	
	return 0;
}

void BestimmeNullstellen(const double& a, const double& b, const double& c)
{
	double p, q, radikand, wurzel, x1, x2;
	bool cmplx = false;
	
	if(a == 0) {			// Betrachte Spezialfall a = 0
		if(b == 0 && c == 0) {
			Ergebnis(Unendlich);
			return;
		}
		if(b == 0 && c != 0) {
			Ergebnis(0);
			return;
		}
		Ergebnis(1, false, -c/b);
		return;
	}
	// Implementiere p-q Formel
	p = b/a; q = c/a;
	if(std::abs(p) > 2*std::sqrt(DBL_MAX)) 	// falls p^2 zu gross wird
		radikand = 1./4. - q/p/p;
	else
		radikand = p/2*p/2 - q;
		
	if(radikand < 0) {						// sind die Loesungen komplex?
		radikand = - radikand;
		cmplx = true;
	}
	
	wurzel = std::sqrt(radikand);			// ziehe die Wurzel
	if(std::abs(p) > 2*std::sqrt(DBL_MAX))
		wurzel *= std::abs(p);
	
	if(cmplx) {								// falls das Ergebnis komplex ist, sind wir fertig
		Ergebnis(2, true, -p/2., wurzel);
		return;
	}
	
	if(p >= 0) {							// Versuche Ausloeschung zu vermeiden
		x2 = -p/2 - wurzel;
		if(q == 0) 
			x1 = x2;
		else 
			x1 = q/x2;
	} else {
		x1 = -p/2 + wurzel;
		if(q == 0) 
			x2 = x1;
		else 
			x2 = q/x2;
	}
	Ergebnis(2, false, x1, x2);
	return;	
}
