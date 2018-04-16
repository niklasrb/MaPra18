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

// ===== Hauptprogramm =====
void BestimmeNullstellen(const double& a, const double& b, const double& c);

int main()
{
	// Hier kommt nun Ihr Programm; viel Erfolg!
	double a, b, c;
	for(int i = 1; i <= AnzahlBeispiele; i++) {
		Start(i, a, b, c);
		std::cout << i << ": a = " << a << ", b = " << b << ", c = " << c << std::endl;
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
		
	if(radikand == 0) {						// Doppelte Nullstelle
		Ergebnis(1, false, -p/2.);
		return;
	}
	
	if(radikand < 0) {						// sind die Loesungen komplex?
		radikand = - radikand;
		cmplx = true;
	}
	
	wurzel = std::sqrt(radikand);			// ziehe die positive Wurzel
	if(std::abs(p) > 2*std::sqrt(DBL_MAX))	// falls noetig, korrigiere
		wurzel *= std::abs(p);
	
	if(cmplx) {								// falls das Ergebnis komplex ist, sind wir fertig
		Ergebnis(2, true, -p/2., wurzel);
		return;
	}
	
	if(p >= 0) {							// Versuche Ausloeschung zu vermeiden
		x2 = -p/2 - wurzel;	// < 0
		if(q == 0) 
			x1 = 0;
		else 
			x1 = q/x2;
	} else {
		x1 = -p/2 + wurzel;	// > 0
		if(q == 0) 
			x2 = 0;
		else 
			x2 = q/x1;
	}
	Ergebnis(2, false, x1, x2);
	return;	
}
