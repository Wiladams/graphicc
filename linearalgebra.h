#pragma once
#ifndef linearalgebra_h
#define linearalgebra_h

#include "grc_types.h"

#ifdef __cplusplus
extern "C" {
#endif

// Basic geometric vector type routines
void real3_set3(real3 c, REAL x, REAL y, REAL z);

// Simple arithmetic
void real3_add(real3 c, const real3 a, const real3 b);
void real3_sub(real3 c, const real3 a, const real3 b);
void real3_mul_scalar(real3 c, const real3 a, const REAL scalar);
void real3_div_scalar(real3 c, const real3 a, const REAL scalar);
void real3_neg(real3 c, const real3 a);

// Vector properties
REAL real3_mag(const real3 c);

// linear algebra
REAL real3_dot(const real3 a, const real3 b);
void real3_cross(real3 c, const real3 a, const real3 b);


// Basic matrix routines

#ifdef __cplusplus
}
#endif

#endif
