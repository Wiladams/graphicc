#pragma once
#ifndef linearalgebra_h
#define linearalgebra_h

#include "grc_types.h"

#ifdef __cplusplus
extern "C" {
#endif

// Basic geometric vector type routines
void realn_add(const int dim, real3 c, const real3 a, const real3 b);
void realn_sub(const int dim, real3 c, const real3 a, const real3 b);
void realn_mul_scalar(const int dim, real3 c, const real3 a, const REAL scalar);
void realn_div_scalar(const int dim, real3 c, const real3 a, const REAL scalar);
void realn_neg(const int dim, real3 c, const real3 a);

// Linear Algebra
REAL realn_dot(const int dim, const real3 a, const real3 b);
REAL realn_mag(const int dim, const real3 c);	// magnitude of vector
void realn_normalize(const int dim, real3 c, const real3 a);

// Specific to 1x3
void real3_set(real3 c, REAL x, REAL y, REAL z);

// Simple arithmetic
#define real3_add(c, a, b) realn_add(3, c, a, b)
#define real3_sub(c, a, b) realn_sub(3, c, a, b)
#define real3_mul_scalar(c, a, scalar) realn_mul_scalar(3, c, a, scalar)
#define real3_div_scalar(c, a, scalar) realn_div_scalar(3, c, a, scalar)
#define real3_neg(c, a) realn_neg(3, c, a)

#define  real3_dot(a, b) realn_dot(3, a, b)
#define  real3_mag(c) realn_mag(3, c)
#define  real3_normalize(c, a) realn_normalize(3, c, a)

void real3_cross(real3 c, const real3 a, const real3 b);
REAL real3_distance(const real3 a, const real3 b);
REAL real3_radians_between_units(const real3 a, const real3 b);
REAL real3_radians_between(const real3 a, const real3 b);


// matrix 2X2
void real2x2_neg(real2x2 c, const real2x2 a);
REAL real2x2_det(const real2x2 a);

// Matrix 3X3
void real3x3_add(real3x3 c, const real3x3 a, const real3x3 b);
void real3x3_sub(real3x3 c, const real3x3 a, const real3x3 b);
void real3x3_mul_scalar(real3x3 c, const real3x3 a, const REAL scalar);
void real3x3_div_scalar(real3x3 c, const real3x3 a, const REAL scalar);
REAL real3x3_det(const real3x3 a);

#ifdef __cplusplus
}
#endif

#endif
