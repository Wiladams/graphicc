#include "linearalgebra.h"
#include <math.h>

// general methods
void realn_add(const int dim, REAL *c, const REAL *a, const REAL *b)
{
	for (int i = 0; i < dim; i++)
	{
		c[i] = a[i] + b[i];
	}
}

void realn_sub(const int dim, REAL *c, const REAL *a, const REAL *b)
{
	for (int i = 0; i < dim; i++)
	{
		c[i] = a[i] - b[i];
	}
}

void realn_mul_scalar(const int dim, REAL *c, const REAL *a, const REAL scalar)
{
	for (int i = 0; i < dim; i++)
	{
		c[i] = a[i] * scalar;
	}
}

void realn_div_scalar(const int dim, REAL *c, const REAL *a, const REAL scalar)
{
	realn_mul_scalar(dim, c, a, 1 / scalar);
}

void realn_neg(const int dim, REAL *c, const REAL *a)
{
	for (int i = 0; i < dim; i++)
	{
		c[i] = -a[i];
	}
}

// Linear algebra
REAL realn_dot(const real3 a, const real3 b)
{
	REAL c = 0;
	REAL c = a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
	return c;
}



// Specific to real3
void real3_set(real3 c, REAL x, REAL y, REAL z)
{
	c[0] = x;
	c[1] = y;
	c[2] = z;
}

// c = a + b
void real3_add(real3 c, const real3 a, const real3 b)
{
	c[0] = a[0] + b[0];
	c[1] = a[1] + b[1];
	c[2] = a[2] + b[2];
}

// c = a - b
void real3_sub(real3 c, const real3 a, const real3 b)
{
	c[0] = a[0] - b[0];
	c[1] = a[1] - b[1];
	c[2] = a[2] - b[2];
}

// c = c * s
void real3_mul_scalar(real3 c, const real3 a, const REAL s)
{
	c[0] = a[0] * s;
	c[1] = a[1] * s;
	c[2] = a[2] * s;
}

// c = c / s;
void real3_div_scalar(real3 c, const real3 a, const REAL s)
{
	real3_mul_scalar(c, a, 1 / s);
}

// c = -c;
void real3_neg(real3 c, const real3 a)
{
	c[0] = -a[0];
	c[1] = -a[1];
	c[2] = -a[2];
}

// ||c||
REAL real3_mag(const real3 c)
{
	REAL lsquared = real3_dot(c, c);
	return sqrt(lsquared);
}

void real3_normalize(real3 c, const real3 a)
{
	REAL mag = real3_mag(a);
	real3_div_scalar(c, a, mag);
}

// c = a dot b
REAL real3_dot(const real3 a, const real3 b)
{
	REAL c = a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
	return c;
}

// c = a cross b
void real3_cross(real3 c, const real3 a, const real3 b)
{
	c[0] = (a[1] * b[2]) - (a[2] * b[1]);
	c[1] = (a[2] * b[0]) - (a[0] * b[2]);
	c[2] = (a[0] * b[1]) - (a[1] * b[0]);
}

REAL real3_distance(const real3 a, const real3 b)
{
	real3 c;
	real3_sub(c, b, a);
	return real3_mag(c);
}

REAL real3_radians_between_units(const real3 a, const real3 b)
{
	return acos(real3_dot(a, b));
}

REAL real3_radians_between(const real3 a, const real3 b)
{
	real3 aunit;
	real3 bunit;

	real3_normalize(aunit, a);
	real3_normalize(bunit, b);
	
	return real3_radians_between_units(aunit, bunit);
}

/*
	Matrix routines
*/
void real3x3_neg(real3x3 c, const real3x3 a)
{
	for (int row = 0; row < 3; row++) {
		for (int column = 0; column < 3; column++) {
			c[row][column] = -a[row][column];
		}
	}

/*
	// Unrolled
	// row 1
	c[0][0] = -a[0][0];
	c[0][1] = -a[0][1];
	c[0][2] = -a[0][2];

	// row 2
	c[1][0] = -a[1][0];
	c[1][1] = -a[1][1];
	c[1][2] = -a[1][2];

	// row3
	c[2][0] = -a[2][0];
	c[2][1] = -a[2][1];
	c[2][2] = -a[2][2];
*/
}

void real3x3_add(real3x3 c, const real3x3 a, const real3x3 b)
{
	for (int row = 0; row < 3; row++) {
		for (int column = 0; column < 3; column++) {
			c[row][column] = a[row][column] + b[row][column];
		}
	}

/*
	// unrolled
	// row 1
	c[0][0] = a[0][0] + b[0][0];
	c[0][1] = a[0][1] + b[0][1];
	c[0][2] = a[0][2] + b[0][2];

	// row 2
	c[1][0] = a[1][0] + b[1][0];
	c[1][1] = a[1][1] + b[1][1];
	c[1][2] = a[1][2] + b[1][2];
	
	// row3
	c[2][0] = a[2][0] + b[2][0];
	c[2][1] = a[2][1] + b[2][1];
	c[2][2] = a[2][2] + b[2][2];
*/
}

void real3x3_sub(real3x3 c, const real3x3 a, const real3x3 b)
{
	for (int row = 0; row < 3; row++) {
		for (int column = 0; column < 3; column++) {
			c[row][column] = a[row][column] - b[row][column];
		}
	}

/*
	// Unrolled
	// row 1
	c[0][0] = a[0][0] - b[0][0];
	c[0][1] = a[0][1] - b[0][1];
	c[0][2] = a[0][2] - b[0][2];

	// row 2
	c[1][0] = a[1][0] - b[1][0];
	c[1][1] = a[1][1] - b[1][1];
	c[1][2] = a[1][2] - b[1][2];

	// row3
	c[2][0] = a[2][0] - b[2][0];
	c[2][1] = a[2][1] - b[2][1];
	c[2][2] = a[2][2] - b[2][2];
*/
	// Another way, if function calls are cheap
	//real3x3_neg(c, b);
	//real3x3_add(c, a, c);

}

void real3x3_mul_scalar(real3x3 c, const real3x3 a, const REAL scalar)
{
	for (int row = 0; row < 3; row++) {
		for (int column = 0; column < 3; column++) {
			c[row][column] = a[row][column] * scalar;
		}
	}
}

void real3x3_div_scalar(real3x3 c, const real3x3 a, const REAL scalar)
{
	REAL fac = 1 / scalar;
	real3x3_mul_scalar(c, a, fac);
}
