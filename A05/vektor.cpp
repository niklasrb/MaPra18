/***********************************************
*  Name       : vektor.cpp                     *
*  Verwendung : Vektorklasse                   *
*  Autor      :                                *
*  Datum      :                                *
***********************************************/

#include "vektor.h"
#include <iomanip>
#include <cmath>
#include <cstdlib>

// =======================
//      Konstruktoren
// =======================


// ----- Konstruktor -----

Vektor::Vektor(size_t l)
{
#ifndef NDEBUG
  if (l <= 0)
    VekFehler("Nur Vektoren mit positiver Laenge!");
#endif

  this->ReDim(l);
}


// ----- Kopierkonstruktor -----

Vektor::Vektor(const Vektor& x):Vek(x.Vek){ }


// ===========================================
//      Vektorelement schreiben und lesen
// ===========================================


// ----- Schreib- und Lesezugriff auf Vektorelemente -----

double& Vektor::operator () (size_t i) {
#ifndef NDEBUG
  if (i >= Vek.size())
    VekFehler("Ungueltiger Index!");
#endif

  return Vek[i];
}


// ----- Lesezugriff auf Elemente konstanter Vektoren -----

double Vektor::operator () (size_t i) const {
#ifndef NDEBUG
  if (i >= Vek.size())
    VekFehler("Ungueltiger Index!");
#endif

  return Vek[i];
}

// =====================
//      Zuweisungen
// =====================

// ----- Zuweisungsoperator "=" ----

Vektor& Vektor::operator = (const Vektor& x)
{
#ifndef NDEBUG
  if (Vek.size() != x.Laenge())
    VekFehler("Inkompatible Dimensionen fuer 'Vektor = Vektor'!");
#endif

  for (size_t i = 0; i < Vek.size(); i++)
    (*this) (i) = x(i);

  //oder: this->Vek=x.Vek;

  return *this;
}


// ----- Zuweisungsoperator mit Addition "+=" ----

Vektor& Vektor::operator += (const Vektor& x)
{
  #ifndef NDEBUG
  if (Vek.size() != x.Laenge())
    VekFehler("Inkompatible Dimensionen fuer 'Vektor += Vektor'!");
#endif

  for (size_t i = 0; i < Vek.size(); i++)
    (*this) (i) += x(i);

  return *this;
}


// ----- Zuweisungsoperator mit Subtraktion "-=" ----

Vektor& Vektor::operator -= (const Vektor& x)
{
  #ifndef NDEBUG
  if (Vek.size() != x.Laenge())
    VekFehler("Inkompatible Dimensionen fuer 'Vektor -= Vektor'!");
#endif

  for (size_t i = 0; i < Vek.size(); i++)
    (*this) (i) -= x(i);

  return *this;
}


// ----- Zuweisungsoperator mit Multiplikation "*=" ----

Vektor& Vektor::operator *= (const double c)
{
  for(size_t i = 0; i < Vek.size(); i++)
	(*this)(i) *= c;
	
	return *this;
}


// ----- Zuweisungsoperator mit Division "/=" ----

Vektor& Vektor::operator /= (const double c)
{
  for(size_t i = 0; i < Vek.size(); i++)
	(*this)(i) /= c;
	
	return *this;
}


// ==============================
//      Vektorlaenge aendern
// ==============================


// ----- Vektorlaenge aendern -----

Vektor& Vektor::ReDim (size_t l)
{
#ifndef NDEBUG
  if (l <= 0)
    VekFehler("Nur Vektoren mit positiver Laenge!");
#endif

  Vek.clear();
  Vek.resize(l,0);

  return *this;
}


// ======================
//      Vektornormen
// ======================


// ----- Euklidische Norm -----

double Vektor::Norm2() const
{
	double s = 0;
	for(size_t i = 0; i < Vek.size(); i++)
		s += pow((*this)(i), 2);
	return sqrt(s);	
}


// ----- Maximum-Norm -----

double Vektor::NormMax() const
{
	double m = 0;
	for(size_t i = 0; i < Vek.size(); i++) {
		if(std::abs((*this)(i)) > m)
			m = std::abs((*this)(i));
	}
	return m;
}


// ==================================
//      arithmetische Operatoren
// ==================================


// ----- Addition "+" -----

Vektor operator + (const Vektor& x, const Vektor& y)
{
#ifndef NDEBUG
  if (x.Laenge() != y.Laenge())
    Vektor::VekFehler("Inkompatible Dimensionen fuer 'Vektor + Vektor'!");
#endif

  Vektor z = x;
  return z += y;
}


// ----- Subtraktion "-" -----

Vektor operator - (const Vektor& x, const Vektor& y)
{
#ifndef NDEBUG
  if (x.Laenge() != y.Laenge())
    Vektor::VekFehler("Inkompatible Dimensionen fuer 'Vektor + Vektor'!");
#endif

  Vektor z = x;
  return z -= y;
}


// ----- Vorzeichen wechseln "-" -----

Vektor operator - (const Vektor& x)
{
  Vektor z = x;
  return z *= -1.;
}


// ----- Skalarprodukt -----

double dot (const Vektor& x, const Vektor& y)
{
	#ifndef NDEBUG
	if (x.Laenge() != y.Laenge())
		Vektor::VekFehler("Inkompatible Dimensionen fuer 'Vektor * Vektor'!");
	#endif
	double s = 0;
	for(size_t i = 0; i < x.Laenge(); i++)
		s += x(i)*y(i);
	return s;
}

double operator *  (const Vektor& x, const Vektor& y)
{
	return dot(x, y);
}


// ----- Multiplikation Skalar*Vektor "*" -----

Vektor operator * (const double c, const Vektor& x)
{
	Vektor z = x;
	return z*= c;
}


// ----- Multiplikation Vektor*Skalar "*" -----

Vektor operator * (const Vektor& x, const double c)
{
	return c*x;
}


// ----- Division Vektor/Skalar "/" -----

Vektor operator / (const Vektor& x, const double c)
{
	return (1./c)*x;
}


// ==============================
//      Vergleichsoperatoren
// ==============================


// ----- Test auf Gleichheit "==" -----

bool operator == (const Vektor& x, const Vektor& y)
{
  if (x.Laenge() !=y.Laenge()) return false;

  for (size_t i = 0, xsize = x.Laenge(); i < xsize; i++)
    if (x(i)!=y(i)) return false;

  return true;
}


// ----- Test auf Ungleichheit "!=" -----

bool operator != (const Vektor& x, const Vektor& y)
{
	return !(x == y);
}


// ==========================
//      Ein- und Ausgabe
// ==========================


// ----- Ausgabe "<<" -----

std::ostream& operator << (std::ostream& s, const Vektor& x)
{
  s << std::setiosflags(std::ios::right);
  s << "# Laenge: " << x.Laenge() << " [";
  for (size_t i = 0, xsize = x.Laenge(); i < xsize; i++)
    s << x(i) << (i < xsize-1 ? ", " : "]");

  return s << std::endl;
}


// ----- Eingabe ">>" -----

std::istream& operator >> (std::istream& s, Vektor& x)
{
  for (size_t i = 0, xsize = x.Laenge(); i < xsize; i++) {
	s >> x(i);
  }
  return s;
}


// ==========================
//      Fehlerbehandlung
// ==========================


// ----- Ausgabe der Fehlermeldung "str" -----

void Vektor::VekFehler (const std::string& str)
{
  std::cerr << "\nVektorfehler: " << str << '\n' << std::endl;
  exit(1);
}
