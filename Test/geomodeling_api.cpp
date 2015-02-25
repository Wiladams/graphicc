#include "geomodeling_api.h"
#include "linearalgebra.h"

void DETVAL(const mat3 &DI, REAL &VAL)
{
	VAL = mat3_determinant(DI);
}

void MATINV(const mat4 &MI, const mat4 &MO)
{
	//mat4_inverse(MO, MI);
}

// MATRNS - 4x4 matrix transpose
void MATRNS(const mat4 &MI, mat4 &MO)
{
	mat4_transpose(MO, MI);
}

// S = V1 * V2
void SPROD(const real3 &V1, const real3 &V2, REAL &S)
{
	S = realn_dot(3, V1, V2);
}

// V3 = V1 X V2
void VPROD(const real3 &V1, const real3 &V2, real3 &V3)
{
	real3_cross(V3, V1, V2);
}
