#pragma once
#ifndef linearalgebra_h
#define linearalgebra_h

#include "geomtypes.h"

#ifdef __cplusplus
extern "C" {
#endif

void real3_set3(real3 c, REAL x, REAL y, REAL z);

// Simple arithmetic
void real3_add(real3 c, const real3 a, const real3 b);
void real3_sub(real3 c, const real3 a, const real3 b);
void real3_scalar_mul(real3 c, const real3 a, const REAL scalar);
void real3_scalar_div(real3 c, const real3 a, const REAL scalar);
void real3_neg(real3 c, const real3 a);

// Vector properties
REAL real3_mag(const real3 c);

// linear algebra
REAL real3_dot(const real3 a, const real3 b);
void real3_cross(real3 c, const real3 a, const real3 b);

#ifdef __cplusplus
}
#endif

#endif
