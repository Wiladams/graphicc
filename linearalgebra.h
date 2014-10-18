#pragma once

#ifndef linearalgebra_h
#define linearalgebra_h

#include "graphicc.h"

#ifdef __cplusplus
extern "C" {
#endif

// Basic geometric vector type routines
void realn_add(const int dim, REAL * c, const REAL * a, const REAL * b);
void realn_sub(const int dim, REAL * c, const REAL * a, const REAL * b);
void realn_mul_scalar(const int dim, REAL * c, const REAL * a, const REAL scalar);
void realn_div_scalar(const int dim, REAL * c, const REAL * a, const REAL scalar);
void realn_neg(const int dim, REAL * c, const REAL * a);

// Linear Algebra
REAL realn_dot(const int dim, const REAL * a, const REAL * b);
REAL realn_mag(const int dim, const REAL * c);	// magnitude of vector
void realn_normalize(const int dim, REAL * c, const REAL * a);

// Specific to 1x3
void real3_set(real3 c, REAL x, REAL y, REAL z);


void real3_cross(real3 c, const real3 a, const real3 b);
REAL real3_distance(const real3 a, const real3 b);
REAL real3_radians_between_units(const real3 a, const real3 b);
REAL real3_radians_between(const real3 a, const real3 b);


// matrix 2X2
void mat2_neg(mat2 &c, const mat2 &a);
REAL mat2_det(const mat2 &a);
REAL mat2_trace(const mat2 &a);
void mat2_trans(const mat2 &c, const mat2 &a);
void mat2_set_ident(mat2 &c);

// Matrix 3X3
void mat3_add(mat3 &c, const mat3 &a, const mat3 &b);
void mat3_sub(mat3 &c, const mat3 &a, const mat3 &b);
void mat3_mul_scalar(mat3 &c, const mat3 &a, const REAL scalar);
void mat3_div_scalar(mat3 &c, const mat3 &a, const REAL scalar);
REAL mat3_det(const mat3 &a);
REAL mat3_trace(const mat3 &a);
void mat3_trans(mat3 &c, const mat3 &a);
void mat3_set_ident(mat3 &c);

// Some common matrix operations
void row3_mul_mat3(real3 c, const real3 a, const mat3 &m);
void row4_mul_mat4(real4 c, const real4 a, const mat4 &m);

void mat2_mul_mat2(mat2 &c, const mat2 &a, const mat2 &b);
void mat3_mul_mat3(mat3 &c, const mat3 &a, const mat3 &b);
void mat4_mul_mat4(mat4 &c, const mat4 &a, const mat4 &b);

#ifdef __cplusplus
}
#endif


// Simple arithmetic
#define real3_add(c, a, b) realn_add(3, c, a, b)
#define real3_sub(c, a, b) realn_sub(3, c, a, b)
#define real3_mul_scalar(c, a, scalar) realn_mul_scalar(3, c, a, scalar)
#define real3_div_scalar(c, a, scalar) realn_div_scalar(3, c, a, scalar)
#define real3_neg(c, a) realn_neg(3, c, a)

#define  real3_dot(a, b) realn_dot(3, a, b)
#define  real3_mag(c) realn_mag(3, c)
#define  real3_normalize(c, a) realn_normalize(3, c, a)


#endif
