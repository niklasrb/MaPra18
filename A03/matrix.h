/***********************************************
*  Name        : matrix.h                      *
*  Verwendung  : Header zu Matrixklasse        *
*  Autor       : N Becker,  					*
*  Datum       : 7.5.18            				*
***********************************************/

#ifndef _MATRIX_H                         // vektor.h nicht doppelt benutzen
#define _MATRIX_H

#include <iostream>
#include <vector>
#include <string>

class Matrix   
{
protected:
	size_t m, n;	
	std::vector<double> coeff;
		
public:
	explicit Matrix(size_t m, size_t n);
	Matrix();
	~Matrix();
	Matrix(const Matrix&);
	
	double& operator()(size_t, size_t);
	double operator()(size_t, size_t) const;
	
	Matrix& operator =(const Matrix&);
	Matrix& operator +=(const Matrix&);
	Matrix& operator -=(const Matrix&);
	Matrix& operator *=(const double);
	Matrix& operator /=(const double);
	
	Matrix& ReDim(size_t, size_t);
	std::pair<size_t, size_t> Dim() const;
	size_t Zeilen() const;
	size_t Spalten() const;
	
	double NormFrobenius() const;

	static void MatFehler(const std::string& str);
public:	
	friend Matrix   operator +  (const Matrix&, const Matrix&); // Addition
    friend Matrix   operator -  (const Matrix&, const Matrix&); // Subtraktion
    friend Matrix   operator -  (const Matrix&);                // Vorzeichen

    friend Matrix   operator *  (const double,  const Matrix&); // Vielfache
    friend Matrix   operator *  (const Matrix&, const double);
    friend Matrix   operator /  (const Matrix&, const double);
    
    friend Matrix 	operator *  (const Matrix&, const Matrix&); // Matrix Matrix product
    

    friend bool     operator == (const Matrix&, const Matrix&); // Vergleich
    friend bool     operator != (const Matrix&, const Matrix&);

    friend std::istream& operator >> (std::istream&, Matrix&);       // Eingabe
    friend std::ostream& operator << (std::ostream&, const Matrix&); // Ausgabe
	
};



#endif
