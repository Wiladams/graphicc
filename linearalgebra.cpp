#include "linearalgebra.h"
#include <math.h>

void real3_set3(real3 c, REAL x, REAL y, REAL z)
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
