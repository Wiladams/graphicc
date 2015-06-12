#include "geomodeling_api.h"
#include "linearalgebra.h"


void DETVAL(const mat3 &DI, real &VAL)
{
	VAL = mat3_determinant(DI);
}

void MATINV(const mat4 &MI, mat4 &MO)
{
	mat4_inverse(MO, MI);
}

// MATRNS - 4x4 matrix transpose
void MATRNS(const mat4 &MI, mat4 &MO)
{
	mat4_transpose(MO, MI);
}

// S = V1 * V2
void SPROD(const real3 &V1, const real3 &V2, real &S)
{
	S = realn_dot(3, V1, V2);
}

// VANG		Compute the angle between two vectors
void VANG(const real3 &V1, const real3 &V2, real &A)
{
	A = real3_radians_between(V1, V2);
}

// VMAG		Compute the magnitude of a vector
// MAG = magnitude(V)
void VMAG(const real3 V, real MAG)
{
	MAG = realn_mag(3, V);
}


// V3 = V1 X V2
// vector product, cross product
void VPROD(const real3 &V1, const real3 &V2, real3 &V3)
{
	real3_cross(V3, V1, V2);
}

// V3 = V1 + V2
void VSUM(const real3 &V1, const real3 &V2, real3 &V3)
{
	realn_add(3, V3, V1, V2);
}

// Calculate Unit vector for a vector
// U = unit(V)
void VUNIT(const real3 &V, real3 &U)
{
	realn_normalize(3, U, V);
}
