/***********************************************
*  Name       : matrix.cpp                     *
*  Verwendung : Matrixklasse                   *
*  Autor      : Niklas Becker                  *
*  Datum      :                                *
***********************************************/

#include "vektor.h"
#include "matrix.h"
#include <iomanip>
#include <cmath>
#include <utility>
#include <cstdlib>


// =======================
//      Konstruktoren
// =======================


// ----- Konstruktor -----

Matrix::Matrix(size_t m, size_t n) : m(m), n(n)
{
	#ifndef NDEBUG
	if(m <= 0 || n <= 0)
		MatFehler("Nur Matrizen mit positiven Dimensionen!");
	#endif
	this->ReDim(m, n);
}

// ----- Konstruktor -----

Matrix::Matrix() : Matrix(1, 1)
{
}

// ----- Kopierkonstruktor -----

Matrix::Matrix(const Matrix& M) : m(M.m), n(M.n), coeff(M.coeff) { }


// ----- Destruktor -----
Matrix::~Matrix() {}

// ===========================================
//      Matrixelement schreiben und lesen
// ===========================================


// ----- Schreib- und Lesezugriff auf Matrixelemente -----

double& Matrix::operator () (size_t i, size_t j) {
#ifndef NDEBUG
  if (i >= m || j >= n)
    MatFehler("Ungueltiger Index!");
   //std::cout << "( " << i << ", " << j << ") == " << n*i+j << std::endl;
#endif

  return coeff.at(n*i + j);
}


// ----- Lesezugriff auf Elemente konstanter Matrizen -----

double Matrix::operator () (size_t i, size_t j) const {
#ifndef NDEBUG
  if (i >= m || j >= n)
    MatFehler("Ungueltiger Index!");
    //std::cout << "( " << i << ", " << j << ") == " << n*i+j << std::endl;
#endif
  return coeff.at(n*i + j);
}

// =====================
//      Zuweisungen
// =====================

// ----- Zuweisungsoperator "=" ----

Matrix& Matrix::operator = (const Matrix& A)
{
#ifndef NDEBUG
  if (this->Dim() != A.Dim())
    MatFehler("Inkompatible Dimensionen fuer 'Matrix = Matrix'!");
#endif

  for (size_t i = 0; i < m; i++) {
	  for(size_t j = 0; j < n; j++) {
		  (*this)(i, j) = A(i, j);
	  }
  }
  return *this;
}


// ----- Zuweisungsoperator mit Addition "+=" ----

Matrix& Matrix::operator += (const Matrix& A)
{
#ifndef NDEBUG
  if (this->Dim() != A.Dim())
    MatFehler("Inkompatible Dimensionen fuer 'Matrix = Matrix'!");
#endif

  for (size_t i = 0; i < m; i++) {
	  for(size_t j = 0; j < n; j++) {
		  (*this)(i, j) += A(i, j);
	  }
  }
  return *this;
}


// ----- Zuweisungsoperator mit Subtraktion "-=" ----

Matrix& Matrix::operator -= (const Matrix& A)
{
#ifndef NDEBUG
  if (this->Dim() != A.Dim())
    MatFehler("Inkompatible Dimensionen fuer 'Matrix = Matrix'!");
#endif

  for (size_t i = 0; i < m; i++) {
	  for(size_t j = 0; j < n; j++) {
		  (*this)(i, j) -= A(i, j);
	  }
  }
  return *this;
}


// ----- Zuweisungsoperator mit Multiplikation "*=" ----

Matrix& Matrix::operator *= (const double c)
{
  for (size_t i = 0; i < m; i++) {
	  for(size_t j = 0; j < n; j++) {
		  (*this)(i, j) *= c;
	  }
  }
  return *this;
}


// ----- Zuweisungsoperator mit Division "/=" ----

Matrix& Matrix::operator /= (const double c)
{
  for (size_t i = 0; i < m; i++) {
	  for(size_t j = 0; j < n; j++) {
		  (*this)(i, j) /= c;
	  }
  }
  return *this;
}


// ==============================
//      Matrixdimensionen
// ==============================


// ----- Matrixdimensionen aendern -----

Matrix& Matrix::ReDim (size_t m, size_t n)
{
#ifndef NDEBUG
  if (m <= 0 || n<= 0)
    MatFehler("Nur Matrizen mit positiven Dimensionen!");
#endif
  coeff.clear();
  coeff.resize(m*n, 0);
  this->m = m; this->n = n;
  return *this;
}

std::pair<size_t, size_t> Matrix::Dim() const
{
	return std::make_pair(m, n);
}

size_t Matrix::Zeilen() const
{
	return m;
}

size_t Matrix::Spalten() const
{
	return n;
}


// ======================
//      Matrixnormen
// ======================


// ----- Frobenius Norm -----

double Matrix::NormFrobenius() const
{
	double s = 0;
	for(size_t i = 0; i < m; i++) {
		for(size_t j = 0; j < n; j++)
			s += pow((*this)(i, j), 2);
	}	
	return sqrt(s);	
}



// ==================================
//      arithmetische Operatoren
// ==================================


// ----- Addition "+" -----

Matrix operator + (const Matrix& A, const Matrix& B)
{
#ifndef NDEBUG
  if (A.Dim() != B.Dim())
    Matrix::MatFehler("Inkompatible Dimensionen fuer 'Vektor + Vektor'!");
#endif
  Matrix C = A;
  return C += B;
}


// ----- Subtraktion "-" -----

Matrix operator - (const Matrix& A, const Matrix& B)
{
#ifndef NDEBUG
  if (A.Dim() != B.Dim())
    Matrix::MatFehler("Inkompatible Dimensionen fuer 'Matrix + Matrix'!");
#endif
  Matrix C = A;
  return C -= B;
}


// ----- Vorzeichen wechseln "-" -----

Matrix operator - (const Matrix& A)
{
  Matrix B = A;
  return B *= -1.;
}


// ----- Multiplikation Skalar*Matrix "*" -----
Matrix operator *  (const double c,  const Matrix& A)
{
	Matrix B = A;
	return B*= c;
}

// ----- Multiplikation Matrix*Skalar "*" -----
Matrix   operator *  (const Matrix& A, const double c)
{
	return c*A;
}
// ----- Division Matrix/Skalar "*" -----
Matrix   operator /  (const Matrix& A, const double c)
{
	return (1./c)*A;
}

// ----- Multiplikation Matrix*Matrix "*" -----
Matrix	operator *  (const Matrix& A, const Matrix& B)
{
	#ifndef NDEBUG
	if(A.Spalten() != B.Zeilen())
		Matrix::MatFehler("Inkompatible Dimensionen fuer 'Matrix * Matrix'!");
	#endif
	Matrix C(A.Zeilen(), B.Spalten());
	for(size_t i = 0; i < A.Zeilen(); i++) {
		for(size_t j = 0; j < B.Spalten(); j++) {
			for(size_t k = 0; k < A.Spalten(); k++) {
				C(i, j) += A(i, k)*B(k, j);
			}
		}
	}
	return C;
}

// ----- Multiplikation Matrix*Vektor "*" -----
Vektor operator *(const Matrix& A, const Vektor& x)
{
	#ifndef NDEBUG
	if(A.Spalten() != x.Laenge())
		Matrix::MatFehler("Inkompatible Dimensionen fuer 'Matrix * Vektor'!");
	#endif
	Vektor b(A.Zeilen());
	for(size_t i = 0; i < A.Zeilen(); i++) {
		for(size_t j = 0; j < A.Spalten(); j++) {
			b(i) += A(i, j)*x(j);
		}
	}
	return b;
}


// ----- Multiplikation Vektor*Matrix "*" -----
Vektor operator *(const Vektor& x, const Matrix& A)
{
	#ifndef NDEBUG
	if(A.Zeilen() != x.Laenge())
		Matrix::MatFehler("Inkompatible Dimensionen fuer 'Vektor * Matrix'!");
	#endif
	Vektor b(A.Spalten());
	for(size_t j = 0; j < A.Spalten(); j++) {
		for(size_t i = 0; i < A.Zeilen(); i++) {
			b(j) += A(i, j)*x(i);
		}
	}
	return b;
}


// ==============================
//      Vergleichsoperatoren
// ==============================


// ----- Test auf Gleichheit "==" -----

bool  operator == (const Matrix& A, const Matrix& B)
{
	if(A.Dim() != B.Dim())
		return false;
	for(size_t i = 0; i < A.Zeilen(); i++) {
		for(size_t j = 0; j < A.Spalten(); j++) {
			if(A(i, j) != B(i, j))
				return false;
		}
	}
	return true;
}

// ----- Test auf Ungleichheit "!=" -----
bool     operator != (const Matrix& A, const Matrix& B)
{
	return !(A == B);
}



// ==========================
//      Ein- und Ausgabe
// ==========================


// ----- Ausgabe "<<" -----


std::ostream& operator << (std::ostream& os, const Matrix& A) // Ausgabe
{
	os << "[ ";
	for(size_t i = 0; i < A.Zeilen(); i++) {
		os << "[ ";
		for(size_t j = 0; j < A.Spalten(); j++) 
			os << A(i, j) << (j < A.Spalten()-1 ? ", " : "]");
		os << (i < A.Zeilen() - 1 ? ",\n" : "]");
	}
	return os << std::endl;
}



// ----- Eingabe ">>" -----

std::istream& operator >> (std::istream& is, Matrix& A)       // Eingabe
{
	for(size_t i = 0; i < A.Zeilen(); i++) {
		for(size_t j = 0; j < A.Spalten(); j++) {
			is >> A(i, j);
		}
	}
	return is;
}


// ==========================
//      Fehlerbehandlung
// ==========================


// ----- Ausgabe der Fehlermeldung "str" -----

void Matrix::MatFehler (const std::string& str)
{
  std::cerr << "\nMatrixfehler: " << str << '\n' << std::endl;
  exit(1);
}

