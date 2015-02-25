

#include "linearalgebra.h"

#include <math.h>
#include <stdlib.h>
#include <string.h>

// general methods
// copy 
// c = a
void realn_assign(const size_t dim, REAL *c, const REAL *a)
{
	for (size_t idx = 0; idx < dim; idx++) {
		c[idx] = a[idx];
	}
}

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

void realn_mul_realn(const size_t dim, REAL *c, const REAL *a, const REAL *b)
{
	for (size_t idx = 0; idx < dim; idx++) {
		c[idx] = a[idx] * b[idx];
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
void real3_cross(real3 &c, const real3 &a, const real3 &b)
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
// general matrix routines

// in-place transpose
void matn_transpose(const size_t dim, REAL **a)
{
	int i, j;
	REAL tmp;

	for (i = 1; i<dim; i++) {
		for (j = 0; j<i; j++) {
			tmp = a[i][j];
			a[i][j] = a[j][i];
			a[j][i] = tmp;
		}
	}
}


// 2 X 2 Matrix
void mat2_mul_scalar(mat2 &c, const mat2 &a, const REAL scalar)
{
	// row 1
	c.m11 = a.m11 * scalar;
	c.m12 = a.m12 * scalar;

	// row 2
	c.m21 = a.m21 * scalar;
	c.m22 = a.m22 * scalar;
}

void mat2_neg(mat2 &c, const mat2 &a)
{
	c.m11 = -a.m11;
	c.m12 = -a.m12;

	c.m21 = -a.m21;
	c.m22 = -a.m22;
}

REAL mat2_determinant(const mat2 &a)
{
	return (a.m11 * a.m22) - (a.m12 * a.m21);
}

int mat2_inverse(mat2 &c, const mat2 &a)
{
	REAL det = (a.m11 * a.m22) - (a.m12 * a.m21);
	if (det == 0) {
		return 0;
	}
	
	REAL oneoverdet = 1.0f / det;
	// temp matrix used to do final calculation
	mat2 mattmp;
	mattmp.m11 = a.m22;
	mattmp.m12 = -a.m12;
	mattmp.m21 = -a.m21;
	mattmp.m22 = a.m11;
	mat2_mul_scalar(c, mattmp, oneoverdet);
}

REAL mat2_trace(const mat2 &a)
{
	return a.m11 + a.m22;
}

// Assume c and a do NOT occupy the same data space
void mat2_trans(mat2 &c, const mat2 &a)
{
	c.m11 = a.m11;
	c.m12 = a.m21;

	c.m21 = a.m12;
	c.m22 = a.m22;
}

void mat2_set_ident(mat2 &c)
{
	// memset to zero
	c.m12 = 0;
	c.m21 = 0;

	// set the ones we need to be one
	c.m11 = 1;
	c.m22 = 1;
}

// c = mat2 * mat2
void mat2_mul_mat2(mat2 &c, const mat2 &a, const mat2 &b)
{
	c.m11 = a.m11*b.m11 + a.m12*b.m21;
	c.m12 = a.m11*b.m12 + a.m12*b.m22;

	c.m21 = a.m21*b.m11 + a.m22*b.m21;
	c.m22 = a.m21*b.m12 + a.m22*b.m22;
}


// 3 X 3 Matrix

//
//	a  b  c
//  d  e  f
//  g  h  i
//


void mat3_neg(mat3 &c, const mat3 &a)
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

void mat3_add(mat3 &c, const mat3 &a, const mat3 &b)
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

void mat3_sub(mat3 &c, const mat3 &a, const mat3 &b)
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
}

void mat3_mul_scalar(mat3 &c, const mat3 &a, const REAL scalar)
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

void mat3_div_scalar(mat3 &c, const mat3 &a, const REAL scalar)
{
	REAL fac = 1 / scalar;
	mat3_mul_scalar(c, a, fac);
}

//
//	a  b  c
//  d  e  f
//  g  h  i
//
REAL mat3_determinant(const mat3 &a)
{
	// (aei +bfg + cdh) - (ceg + bdi + afh)
	REAL part1 = a.m11*a.m22*a.m33 + a.m12*a.m23*a.m31 + a.m13*a.m21*a.m32;
	REAL part2 = a.m13*a.m22*a.m31 + a.m12*a.m21*a.m33 + a.m11*a.m23*a.m32;

	return part1 - part2;
}

// a + e + i
REAL mat3_trace(const mat3 &a)
{
	return (a.m11 + a.m22 + a.m33);
}

// Assume c and a do NOT occupy the same data space
void mat3_transpose(mat3 &c, const mat3 &a)
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

void mat3_set_ident(mat3 &c)
{
	memset(&c, 0, sizeof(mat3));

	c.m11 = 1;
	c.m22 = 1;
	c.m33 = 1;
}

void mat3_set_columns(mat3 &c, const real3 col1, const real3 col2, const real3 col3)
{
	c.m11 = col1[0];
	c.m21 = col1[1];
	c.m31 = col1[2];

	c.m12 = col2[0];
	c.m22 = col2[1];
	c.m32 = col2[2];

	c.m13 = col3[0];
	c.m23 = col3[1];
	c.m33 = col3[2];
}

// c = mat3 * mat3
void mat3_mul_mat3(mat3 &c, const mat3 &a, const mat3 &b)
{
	c.m11 = a.m11*b.m11 + a.m12*b.m21 + a.m13*b.m31;
	c.m12 = a.m11*b.m12 + a.m12*b.m22 + a.m13*b.m32;
	c.m13 = a.m11*b.m13 + a.m12*b.m23 + a.m13*b.m33;

	c.m21 = a.m21*b.m11 + a.m22*b.m21 + a.m23*b.m31;
	c.m22 = a.m21*b.m12 + a.m22*b.m22 + a.m23*b.m32;
	c.m23 = a.m21*b.m13 + a.m22*b.m23 + a.m23*b.m33;

	c.m31 = a.m31*b.m11 + a.m32*b.m21 + a.m33*b.m31;
	c.m32 = a.m31*b.m12 + a.m32*b.m22 + a.m33*b.m32;
	c.m33 = a.m31*b.m13 + a.m32*b.m23 + a.m33*b.m33;
}

// c = vec3 * mat3
void row3_mul_mat3(real3 c, const real3 a, const mat3 &m)
{
	c[0] = a[0] * m.m11 + a[1] * m.m21 + a[2] * m.m31;
	c[1] = a[0] * m.m12 + a[1] * m.m22 + a[2] * m.m32;
	c[2] = a[0] * m.m13 + a[1] * m.m23 + a[2] * m.m33;
}

/*
	Matrix 4x4
*/

void mat4_set_ident(mat4 &c)
{
	memset(&c, 0, sizeof(mat4));

	c.m11 = 1;
	c.m22 = 1;
	c.m33 = 1;
	c.m44 = 1;
}

void mat4_mul_scalar(mat4 &c, const mat4 &a, const REAL scalar)
{
	// row 1
	c.m11 = a.m11 * scalar;
	c.m12 = a.m12 * scalar;
	c.m13 = a.m13 * scalar;
	c.m14 = a.m14 * scalar;

	// row 2
	c.m21 = a.m21 * scalar;
	c.m22 = a.m22 * scalar;
	c.m23 = a.m23 * scalar;
	c.m24 = a.m24 * scalar;

	// row 3
	c.m31 = a.m31 * scalar;
	c.m32 = a.m32 * scalar;
	c.m33 = a.m33 * scalar;
	c.m34 = a.m34 * scalar;

	// row 4
	c.m41 = a.m41 * scalar;
	c.m42 = a.m42 * scalar;
	c.m43 = a.m43 * scalar;
	c.m44 = a.m44 * scalar;
}

void mat4_transpose(mat4 &c, const mat4 &a)
{
	c.m11 = a.m11;
	c.m12 = a.m21;
	c.m13 = a.m31;
	c.m14 = a.m41;

	c.m21 = a.m12;
	c.m22 = a.m22;
	c.m23 = a.m32;
	c.m24 = a.m42;

	c.m31 = a.m13;
	c.m32 = a.m23;
	c.m33 = a.m33;
	c.m34 = a.m43;

	c.m41 = a.m14;
	c.m42 = a.m24;
	c.m43 = a.m34;
	c.m44 = a.m44;
}

REAL mat4_determinant(const mat4 &m) 
{
	double value;
	value =
		m.m14 * m.m23 * m.m32 * m.m41 - m.m13 * m.m24 * m.m32 * m.m41 - m.m14 * m.m22 * m.m33 * m.m41 + m.m12 * m.m24 * m.m33 * m.m41 +
		m.m13 * m.m22 * m.m34 * m.m41 - m.m12 * m.m23 * m.m34 * m.m41 - m.m14 * m.m23 * m.m31 * m.m42 + m.m13 * m.m24 * m.m31 * m.m42 +
		m.m14 * m.m21 * m.m33 * m.m42 - m.m11 * m.m24 * m.m33 * m.m42 - m.m13 * m.m21 * m.m34 * m.m42 + m.m11 * m.m23 * m.m34 * m.m42 +
		m.m14 * m.m22 * m.m31 * m.m43 - m.m12 * m.m24 * m.m31 * m.m43 - m.m14 * m.m21 * m.m32 * m.m43 + m.m11 * m.m24 * m.m32 * m.m43 +
		m.m12 * m.m21 * m.m34 * m.m43 - m.m11 * m.m22 * m.m34 * m.m43 - m.m13 * m.m22 * m.m31 * m.m44 + m.m12 * m.m23 * m.m31 * m.m44 +
		m.m13 * m.m21 * m.m32 * m.m44 - m.m11 * m.m23 * m.m32 * m.m44 - m.m12 * m.m21 * m.m33 * m.m44 + m.m11 * m.m22 * m.m33 * m.m44;
	
	return value;
}

// c = mat4 * mat4
void mat4_mul_mat4(mat4 &c, const mat4 &a, const mat4 &b)
{
	c.m11 = a.m11*b.m11 + a.m12*b.m21 + a.m13*b.m31 + a.m14*b.m41;
	c.m12 = a.m11*b.m12 + a.m12*b.m22 + a.m13*b.m32 + a.m14*b.m42;
	c.m13 = a.m11*b.m13 + a.m12*b.m23 + a.m13*b.m33 + a.m14*b.m43;
	c.m14 = a.m11*b.m14 + a.m12*b.m24 + a.m13*b.m34 + a.m14*b.m44;

	c.m21 = a.m21*b.m11 + a.m22*b.m21 + a.m23*b.m31 + a.m24*b.m41;
	c.m22 = a.m21*b.m12 + a.m22*b.m22 + a.m23*b.m32 + a.m24*b.m42;
	c.m23 = a.m21*b.m13 + a.m22*b.m23 + a.m23*b.m33 + a.m24*b.m43;
	c.m24 = a.m21*b.m14 + a.m22*b.m24 + a.m23*b.m34 + a.m24*b.m44;

	c.m31 = a.m31*b.m11 + a.m32*b.m21 + a.m33*b.m31 + a.m34*b.m41;
	c.m32 = a.m31*b.m12 + a.m32*b.m22 + a.m33*b.m32 + a.m34*b.m42;
	c.m33 = a.m31*b.m13 + a.m32*b.m23 + a.m33*b.m33 + a.m34*b.m43;
	c.m34 = a.m31*b.m14 + a.m32*b.m24 + a.m33*b.m34 + a.m34*b.m44;

	c.m41 = a.m41*b.m11 + a.m42*b.m21 + a.m43*b.m31 + a.m44*b.m41;
	c.m42 = a.m41*b.m12 + a.m42*b.m22 + a.m43*b.m32 + a.m44*b.m42;
	c.m43 = a.m41*b.m13 + a.m42*b.m23 + a.m43*b.m33 + a.m44*b.m43;
	c.m44 = a.m41*b.m14 + a.m42*b.m24 + a.m43*b.m34 + a.m44*b.m44;

}

// c = vec4 * mat4
void row4_mul_mat4(real4 c, const REAL *a, const mat4 &m)
{
	c[0] = a[0] * m.m11 + a[1] * m.m21 + a[2] * m.m31 + a[3] * m.m41;
	c[1] = a[0] * m.m12 + a[1] * m.m22 + a[2] * m.m32 + a[3] * m.m42;
	c[2] = a[0] * m.m13 + a[1] * m.m23 + a[2] * m.m33 + a[3] * m.m43;
	c[3] = a[0] * m.m14 + a[1] * m.m24 + a[2] * m.m34 + a[3] * m.m44;
}

void mat4_mul_col4(real4 c, const mat4 &m, const REAL *a)
{
	c[0] = m.m11*a[0] + m.m12 *a[1] + m.m13*a[2] + m.m14*a[3];
	c[1] = m.m21*a[0] + m.m22*a[1] + m.m23*a[2] + m.m24*a[3];
	c[2] = m.m31*a[0] + m.m32*a[1] + m.m33*a[2] + m.m34*a[3];
	c[3] = m.m41*a[0] + m.m42*a[1] + m.m43*a[2] + m.m44*a[3];
}