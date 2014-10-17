#include "linearalgebra.h"

#include <math.h>
#include <stdlib.h>
#include <string.h>

// general methods
void realn_add(const int dim, REAL * c, const REAL * a, const REAL *b)
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
REAL realn_dot(const int dim, const REAL * a, const REAL * b)
{
	REAL c = 0;
	for (int i = 0; i < dim; i++)
	{
		c += a[i] * b[0];
	}

	return c;
}

REAL realn_mag(const int dim, const REAL * a)
{
	REAL lsquared = realn_dot(dim, a, a);
	return sqrt(lsquared);
}

void realn_normalize(const int dim, REAL * c, const REAL * a)
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
void mat2_neg(mat2 c, const mat2 a)
{
	c.m11 = -a.m11;
	c.m12 = -a.m12;

	c.m21 = -a.m21;
	c.m22 = -a.m22;
}

REAL mat2_det(const mat2 a)
{
	return (a.m11 * a.m22) - (a.m12 * a.m21);
}

REAL mat2_trace(const mat2 a)
{
	return a.m11 + a.m22;
}

// Assume c and a do NOT occupy the same data space
void mat2_trans(mat2 c, mat2 a)
{
	c.m11 = a.m11;
	c.m12 = a.m21;

	c.m21 = a.m12;
	c.m22 = a.m22;
}

void mat2_set_ident(mat2 c)
{
	// memset to zero
	c.m12 = 0;
	c.m21 = 0;

	// set the ones we need to be one
	c.m11 = 1;
	c.m22 = 1;
}


// 3 X 3 Matrix

//
//	a  b  c
//  d  e  f
//  g  h  i
//


void mat3_neg(mat3 c, const mat3 a)
{
	// row 1
	c.m11 = -a.m11;
	c.m12 = -a.m12;
	c.m13 = -a.m13;

	// row 2
	c.m21 = -a.m21;
	c.m22 = -a.m22;
	c.m23 = -a.m23;

	// row 3
	c.m31 = -a.m31;
	c.m32 = -a.m32;
	c.m33 = -a.m33;
}

void mat3_add(mat3 c, const mat3 a, const mat3 b)
{
	// row 1
	c.m11 = a.m11 + b.m11;
	c.m12 = a.m12 + b.m12;
	c.m13 = a.m13 + b.m13;

	// row 2
	c.m21 = a.m21 + b.m21;
	c.m22 = a.m22 + b.m22;
	c.m23 = a.m23 + b.m23;

	// row 3
	c.m31 = a.m31 + b.m31;
	c.m32 = a.m32 + b.m32;
	c.m33 = a.m33 + b.m33;
}

void mat3_sub(mat3 c, const mat3 a, const mat3 b)
{
	// row 1
	c.m11 = a.m11 - b.m11;
	c.m12 = a.m12 - b.m12;
	c.m13 = a.m13 - b.m13;

	// row 2
	c.m21 = a.m21 - b.m21;
	c.m22 = a.m22 - b.m22;
	c.m23 = a.m23 - b.m23;

	// row 3
	c.m31 = a.m31 - b.m31;
	c.m32 = a.m32 - b.m32;
	c.m33 = a.m33 - b.m33;


	// Another way, if function calls are cheap
	//real3x3_neg(c, b);
	//real3x3_add(c, a, c);

}

void mat3_mul_scalar(mat3 c, const mat3 a, const REAL scalar)
{
	// row 1
	c.m11 = a.m11 * scalar;
	c.m12 = a.m12 * scalar;
	c.m13 = a.m13 * scalar;

	// row 2
	c.m21 = a.m21 * scalar;
	c.m22 = a.m22 * scalar;
	c.m23 = a.m23 * scalar;

	// row 3
	c.m31 = a.m31 * scalar;
	c.m32 = a.m32 * scalar;
	c.m33 = a.m33 * scalar;

}

void mat3_div_scalar(mat3 c, const mat3 a, const REAL scalar)
{
	REAL fac = 1 / scalar;
	mat3_mul_scalar(c, a, fac);
}

//
//	a  b  c
//  d  e  f
//  g  h  i
//
REAL mat3_det(const mat3 a)
{
	// (aei +bfg + cdh) - (ceg + bdi + afh)
	REAL part1 = a.m11*a.m22*a.m33 + a.m12*a.m23*a.m31 + a.m13*a.m21*a.m32;
	REAL part2 = a.m13*a.m22*a.m31 + a.m12*a.m21*a.m33 + a.m11*a.m23*a.m32;

	return part1 - part2;
}

REAL mat3_trace(const mat3 a)
{
	return (a.m11 + a.m22 + a.m33);
}

// Assume c and a do NOT occupy the same data space
void mat3_trans(mat3 c, mat3 a)
{
	c.m11 = a.m11;
	c.m12 = a.m21;
	c.m13 = a.m31;

	c.m21 = a.m12;
	c.m22 = a.m22;
	c.m23 = a.m32;

	c.m31 = a.m13;
	c.m32 = a.m23;
	c.m33 = a.m33;
}

void mat3_set_ident(mat3 c)
{
	memset((void *)&c, 0, sizeof(c));

	c.m11 = 1;
	c.m22 = 1;
	c.m33 = 1;
}

// c = vec3 * mat3
void row3_mul_mat3(real3 c, const real3 a, const mat3 m)
{
	c[0] = a[0] * m.m11 + a[1] * m.m21 + a[2] * m.m31;
	c[1] = a[0] * m.m12 + a[1] * m.m22 + a[2] * m.m32;
	c[2] = a[0] * m.m13 + a[1] * m.m23 + a[2] * m.m33;
}

// c = vec4 * mat4
void row4_mul_mat4(real4 c, const real4 a, const mat4 m)
{
	c[0] = a[0] * m.m11 + a[1] * m.m21 + a[2] * m.m31 + a[3]*m.m41;
	c[1] = a[0] * m.m12 + a[1] * m.m22 + a[2] * m.m32+a[3]*m.m42;
	c[2] = a[0] * m.m13 + a[1] * m.m23 + a[2] * m.m33+a[3]*m.m43;
	c[3] = a[0] * m.m14 + a[1] * m.m24 + a[2] * m.m34+a[3]*m.m44;
}


// c = mat2 * mat2
void mat2_mul_mat2(mat2 *c, const mat2 a, const mat2 b)
{
	c->m11 = a.m11*b.m11 + a.m12*b.m21;
	c->m12 = a.m11*b.m12 + a.m12*b.m22;

	c->m21 = a.m21*b.m11 + a.m22*b.m21;
	c->m22 = a.m21*b.m12 + a.m22*b.m22;
}

// c = mat3 * mat3
void mat3_mul_mat3(mat3 *c, const mat3 a, const mat3 b)
{
	c->m11 = a.m11*b.m11 + a.m12*b.m21 + a.m13*b.m31;
	c->m12 = a.m11*b.m12 + a.m12*b.m22 + a.m13*b.m32;
	c->m13 = a.m11*b.m13 + a.m12*b.m23 + a.m13*b.m33;

	c->m21 = a.m21*b.m11 + a.m22*b.m21 + a.m23*b.m31;
	c->m22 = a.m21*b.m12 + a.m22*b.m22 + a.m23*b.m32;
	c->m23 = a.m21*b.m13 + a.m22*b.m23 + a.m23*b.m33;

	c->m31 = a.m31*b.m11 + a.m32*b.m21 + a.m33*b.m31;
	c->m32 = a.m31*b.m12 + a.m32*b.m22 + a.m33*b.m32;
	c->m33 = a.m31*b.m13 + a.m32*b.m23 + a.m33*b.m33;
}

// c = mat4 * mat4
void mat4_mul_mat4(mat4 c, const mat4 a, const mat4 b)
{

}
