#include "student.h"

// Eingabeoperator ">>"
std::istream& operator>> ( std::istream& s, Student& a)
{
    s >> a.Vorname >> a.Nachname >> a.MatNr >> a.Note;
    return s;
}

// Ausgabeoperator "<<"
std::ostream& operator<< ( std::ostream& s, const Student& a)
{
    return s << a.Vorname << ' ' << a.Nachname << '\t' << a.MatNr << '\t' << a.Note;
}

// Vergleichsoperator "<"
bool operator< ( const Student& a, const Student& b)
{
    if(a.Nachname < b.Nachname)
		return true;
	if(a.Nachname > b.Nachname)
		return false;
	return a.Vorname < b.Vorname;
}

// Vergleichsoperator "<="
bool operator<= ( const Student& a, const Student& b)
{
    return a < b || a == b;
}

// Vergleichsoperatoren "==" bzw. "!="
bool operator== ( const Student& a, const Student& b)
{
    return (a.Vorname == b.Vorname) && (a.Nachname == b.Nachname) && (a.MatNr == b.MatNr);
}

bool operator!= ( const Student& a, const Student& b)
{
    return (a.Vorname != b.Vorname) || (a.Nachname != b.Nachname) || (a.MatNr != b.MatNr);
}


