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
REAL realn_dot(const int dim, const real3 a, const real3 b)
{
	REAL c = 0;
	for (int i = 0; i < dim; i++)
	{
		c += a[i] * b[0];
	}

	return c;
}

REAL realn_mag(const int dim, const real3 a)
{
	REAL lsquared = realn_dot(dim, a, a);
	return sqrt(lsquared);
}

void realn_normalize(const int dim, real3 c, const real3 a)
{
	REAL mag = realn_mag(dim, a);
	realn_div_scalar(dim, c, a, mag);
}





// Specific to real3
void real3_set(real3 c, REAL x, REAL y, REAL z)
{
	c[0] = x;
	c[1] = y;
	c[2] = z;
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
// 2 X 2 Matrix
void real2x2_neg(real2x2 c, const real2x2 a)
{
	c.row1[0] = -a.row1[0];
	c.row1[1] = -a.row1[1];

	c.row2[0] = -a.row2[0];
	c.row2[1] = -a.row2[1];
}

REAL real2x2_det(const real2x2 a)
{
	return (a.row1[0] * a.row2[1]) - (a.row1[1] * a.row2[0]);
}

// 3 X 3 Matrix

//
//	a  b  c
//  d  e  f
//  g  h  i
//


void real3x3_neg(real3x3 c, const real3x3 a)
{
	// Unrolled
	// row 1
	c.row1[0] = -a.row1[0];
	c.row1[1] = -a.row1[1];
	c.row1[2] = -a.row1[2];

	// row 2
	c.row2[0] = -a.row2[0];
	c.row2[1] = -a.row2[1];
	c.row2[2] = -a.row2[2];

	// row3
	c.row3[0] = -a.row3[0];
	c.row3[1] = -a.row3[1];
	c.row3[2] = -a.row3[2];
}

void real3x3_add(real3x3 c, const real3x3 a, const real3x3 b)
{
	// row1
	for (int column = 0; column < 3; column++)
	{
		c.row1[column] = a.row1[column] + b.row1[column];
	}

	// row2
	for (int column = 0; column < 3; column++)
	{
		c.row2[column] = a.row2[column] + b.row2[column];
	}

	// row3
	for (int column = 0; column < 3; column++)
	{
		c.row3[column] = a.row3[column] + b.row3[column];
	}
}

void real3x3_sub(real3x3 c, const real3x3 a, const real3x3 b)
{
	// row1
	for (int column = 0; column < 3; column++)
	{
		c.row1[column] = a.row1[column] - b.row1[column];
	}

	// row2
	for (int column = 0; column < 3; column++)
	{
		c.row2[column] = a.row2[column] -b.row2[column];
	}

	// row3
	for (int column = 0; column < 3; column++)
	{
		c.row3[column] = a.row3[column] -b.row3[column];
	}

	// Another way, if function calls are cheap
	//real3x3_neg(c, b);
	//real3x3_add(c, a, c);

}

void real3x3_mul_scalar(real3x3 c, const real3x3 a, const REAL scalar)
{
	// row1
	for (int column = 0; column < 3; column++) 
	{
		c.row1[column] = a.row1[column] * scalar;
	}

	// row2
	for (int column = 0; column < 3; column++)
	{
		c.row2[column] = a.row2[column] * scalar;
	}

	// row3
	for (int column = 0; column < 3; column++)
	{
		c.row3[column] = a.row3[column] * scalar;
	}
}

void real3x3_div_scalar(real3x3 c, const real3x3 a, const REAL scalar)
{
	REAL fac = 1 / scalar;
	real3x3_mul_scalar(c, a, fac);
}

//
//	a  b  c
//  d  e  f
//  g  h  i
//
REAL real3x3_det(const real3x3 a)
{
	// (aei +bfg + cdh) - (ceg + bdi + afh)
	REAL part1 = a.row1[0] * a.row2[1] * a.row3[2];
	part1 += a.row1[1] * a.row2[2] * a.row3[0];
	part1 += a.row1[2] * a.row2[0] * a.row3[1];

	REAL part2 = a.row1[2] * a.row2[1] * a.row3[0];
	part2 += a.row1[1] * a.row2[0] * a.row3[2];
	part2 += a.row1[0] * a.row2[2] * a.row3[1];

	return part1 - part2;
}