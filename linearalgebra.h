#pragma once
#ifndef linearalgebra_h
#define linearalgebra_h

#include "grc_types.h"

#ifdef __cplusplus
extern "C" {
#endif

// Basic geometric vector type routines
void realn_add(real3 c, const real3 a, const real3 b);
void realn_sub(real3 c, const real3 a, const real3 b);
void realn_mul_scalar(real3 c, const real3 a, const REAL scalar);
void realn_div_scalar(real3 c, const real3 a, const REAL scalar);
void realn_neg(real3 c, const real3 a);

	
	void real3_set(real3 c, REAL x, REAL y, REAL z);

// Simple arithmetic
void real3_add(real3 c, const real3 a, const real3 b);
void real3_sub(real3 c, const real3 a, const real3 b);
void real3_mul_scalar(real3 c, const real3 a, const REAL scalar);
void real3_div_scalar(real3 c, const real3 a, const REAL scalar);
void real3_neg(real3 c, const real3 a);

// Linear algebra
REAL real3_mag(const real3 c);	// magnitude of vector
void real3_normalize(real3 c, const real3 a);
REAL real3_dot(const real3 a, const real3 b);
void real3_cross(real3 c, const real3 a, const real3 b);
REAL real3_distance(const real3 a, const real3 b);
REAL real3_radians_between_units(const real3 a, const real3 b);
REAL real3_radians_between(const real3 a, const real3 b);

// Basic matrix routines
void real3x3_add(real3x3 c, const real3x3 a, const real3x3 b);
void real3x3_sub(real3x3 c, const real3x3 a, const real3x3 b);
void real3x3_mul_scalar(real3x3 c, const real3x3 a, const REAL scalar);
void real3x3_div_scalar(real3x3 c, const real3x3 a, const REAL scalar);

#ifdef __cplusplus
}
#endif

#endif
