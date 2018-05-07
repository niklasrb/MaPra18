
#include "matrix.h"
#include "vektor.h"
#include "unit.h"
#include <iostream>
#include <cstdlib>

Vektor Jakobi(const Matrix& A, const Vektor& b, const Vektor& x0, double& eps, int& maxiter);
Vektor GaussSeidel(const Matrix& A, const Vektor& b, const Vektor& x0, double& eps, int& maxiter);
Vektor ConjugateGradient(const Matrix& A, const Vektor& b, const Vektor& x0, double& eps, int& maxiter);

int main(int argc, char** argv)
{
	int ex = 0;
	if(argc > 1) ex = atoi(argv[1]);
	
	Matrix A;
	Vektor x0, b, x;
	double tol;
	int maxiter;
	
	Start(ex, A, x0, b, tol, maxiter);
	x.ReDim(x0.Laenge());
	
	std::cout << "Problem: " << std::endl << A << std::endl << b << std::endl 
						<< "maxiter = " << maxiter << std::endl << "tol = " << tol << std::endl;
	// Jakobi
	x = Jakobi(A, b, x0, tol, maxiter);
	std::cout << "Jakobi: " << x << std::endl << "it: " << maxiter << "  - err: " << tol << std::endl;
	Ergebnis(x, maxiter, 0);
	// GaussSeidel
	Start(ex, A, x0, b, tol, maxiter);
	x = GaussSeidel(A, b, x0, tol, maxiter);
	std::cout << "GaussSeidel: " << x << std::endl << "it: " << maxiter << "  - err: " << tol << std::endl;
	Ergebnis(x, maxiter, 1);
	// Conjugate Gradient
	Start(ex, A, x0, b, tol, maxiter);
	x = ConjugateGradient(A, b, x0, tol, maxiter);
	std::cout << "ConjugateGradient: " << x << std::endl << "it: " << maxiter << "  - err: " << tol << std::endl;
	Ergebnis(x, maxiter, 2);
	
	
	return 0;
}

Vektor Jakobi(const Matrix& A, const Vektor& b, const Vektor& x0, double& eps, int& maxiter)
{
	int iter = 0;
	double err = (A*x0 - b).Norm2(), s;
	Vektor xold(x0), xnew(x0.Laenge());

	while(iter < maxiter && err > eps) {
		for(size_t i = 0; i < x0.Laenge(); i++) {
			s = 0;
			for(size_t j = 0; j < A.Spalten(); j++) {
				if(i != j) s += A(i, j)*xold(j);
			}
			xnew(i) = (b(i) - s)/A(i, i);
		}
		err = (A*xnew - b).Norm2();
		xold = xnew;
		iter++;
	}
	eps = err; maxiter = iter;
	return xnew;
}

Vektor GaussSeidel(const Matrix& A, const Vektor& b, const Vektor& x0, double& eps, int& maxiter)
{
	int iter = 0;
	double err = (A*x0 - b).Norm2(), s;
	Vektor xold(x0), xnew(x0.Laenge());
	
	while(iter < maxiter && err > eps) {
		for(size_t i = 0; i < x0.Laenge(); i++) {
			s = 0;
			for(size_t j = 0; j < i; j++) 
				s += A(i, j)*xnew(j);
			for(size_t j = i+1; j < A.Spalten(); j++)
				s += A(i, j)*xold(j);
			
			xnew(i) = (b(i) - s)/A(i, i);
		}
		err = (A*xnew - b).Norm2();
		xold = xnew;
		iter++;
	}
	eps = err; maxiter = iter;
	return xnew;
}

Vektor ConjugateGradient(const Matrix& A, const Vektor& b, const Vektor& x0, double& eps, int& maxiter)
{
	Vektor x(x0), p(b - A*x0);
	Vektor r(p), q(p);
	double gammaNew, gammaOld = r*r, alpha;
	int iter = 0;
	double err = (A*x0 - b).Norm2();
	
	while(iter < maxiter && err > eps) {
		q = A*p;
		alpha = gammaOld / (q*p);
		x += alpha*p;
		r -= alpha*q;
		gammaNew = r*r;
		p = r + (gammaNew/gammaOld)*p;
		err = r.Norm2();
		gammaOld = gammaNew;
		iter++;
	}
	eps = err; maxiter = iter;
	return x;
}
