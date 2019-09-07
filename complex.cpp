/*
 * file: complex.cpp
 * This is the implementation of our complex number
 * class whose interface resides in the file complex.h
 */

#include "complex.h"

#include <cmath> // for the square root function needed for norm()

complex::complex(double re, double im) //Professor Wrote
{
	//initialize the real and imaginary parts:
	real = re;
	imag = im;
}

complex::complex(const complex& z) //Professor Wrote
{
	//we need to make *this a copy of z:
	real = z.real;
	imag = z.imag;
}

ostream& operator<<(ostream& o, const complex& z) //Professor Wrote
{
	o << "(" << z.real;
	if(z.imag>=0)
		o << " + " << z.imag;
	else
		o << " - " << -z.imag;
	o << "i)";
	return o;
}

istream& operator>>(istream& i, complex& z) //Professor Wrote
{
	return (i>> z.real >> z.imag);
}

complex complex::conj() //Professor Wrote
{
	#if 0
	complex temp;
	temp.real = real;
	temp.imag = -imag;
	return temp;
	#else
	/* NOTE: alternatively, you could use a constructor to make a
	 * new complex number with the right real and imaginary parts,
	 * and return it straight away: */
	return complex(real,-imag);
	#endif
}

complex::complex() //Professor Wrote
{
	real = 0;
	imag = 0;
}

double complex::norm() //Written
{
	/* TODO: write this */
	return sqrt(real*real + imag*imag); //returning the value of the square root of real times real added by imaginary times imaginary
}

complex operator+(const complex& w, const complex& z) //Professor Wrote
{
	complex retVal;
	retVal.real = w.real + z.real;
	retVal.imag = w.imag + z.imag;
	return retVal;
	/* again, this could also be written as: */
	// return complex(w.real+z.real,w.imag+z.imag);
}

complex operator-(const complex& w) //Written
{
	/* TODO: write this */
	/* NOTE: this is unary negation, not subtraction. */
	complex retVal; //declaring
	//Just negating now, similar to what the professor wrote above.
	retVal.real = -w.real;
	retVal.imag = -w.imag;
	return retVal;
}

complex operator-(const complex& w, const complex& z) //Written
{
	/* TODO: write this */
	return (w-z); //Just subtracting
}

complex operator*(const complex& w, const complex& z) //Written
{
	/* TODO: write this */
	complex retVal;
	retVal.real = (w.real * z.real) - (w.imag * z.imag);
	//multiplying and subtract to set as real
	retVal.imag = (w.real * z.imag) + (w.imag * z.real);
	//multiply and add to set as imag
	return retVal;
}

complex operator/(complex& w, complex& z) //Written
{
	/* TODO: write this */
	complex retVal;
	complex zConj = z.conj();
	//returns the complex conjugate of z since it's the demoninator
	retVal = w * zConj;
	size_t demoninator = (z * zConj).real;
	//setting the demoninator
	retVal.real /= demoninator;
	//dividing real by demoninator
	retVal.imag /= demoninator;
	//dividing imag by demoninator
	return retVal;
}

complex operator^(const complex& w, int a) //Professor Wrote
{
	/* NOTE: there are much more efficient ways to do exponentiation,
	 * but it won't make much difference for our application. */
	complex retVal(1,0); //initialize it to 1
	for(int i=0; i<a; i++)
		retVal = retVal*w;
	return retVal;
}
