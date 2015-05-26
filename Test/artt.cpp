#include "artt.h"

AMatrix & AMatrix::scaleXYZ(const AVector &v)
{
	m[0] *= v.x; m[3] *= v.x; m[6] *= v.x;
	m[1] *= v.y; m[4] *= v.y; m[7] *= v.y;
	m[2] *= v.z; m[5] *= v.z; m[8] *= v.z;

	return *this;
}

AVector operator*(const AVector &v, const AMatrix &m)
{
	return AVector(
		(v.x*m.m[0]) + (v.y*m.m[3]) + (v.z*m.m[6]),
		(v.x*m.m[1]) + (v.y*m.m[4]) + (v.z*m.m[7]),
		(v.x*m.m[2]) + (v.y*m.m[5]) + (v.z*m.m[8]));
}

AMatrix operator*(const AMatrix &a, const AMatrix &b)
{
	AMatrix r;
	r.m[0] = (a.m[0] * b.m[0]) + (a.m[1] * b.m[3]) + (a.m[2] * b.m[6]);
	r.m[1] = (a.m[0] * b.m[1]) + (a.m[1] * b.m[4]) + (a.m[2] * b.m[7]);
	r.m[2] = (a.m[0] * b.m[2]) + (a.m[1] * b.m[5]) + (a.m[2] * b.m[8]);
	r.m[3] = (a.m[3] * b.m[0]) + (a.m[4] * b.m[3]) + (a.m[5] * b.m[6]);
	r.m[4] = (a.m[3] * b.m[1]) + (a.m[4] * b.m[4]) + (a.m[5] * b.m[7]);
	r.m[5] = (a.m[3] * b.m[2]) + (a.m[4] * b.m[5]) + (a.m[5] * b.m[8]);
	r.m[6] = (a.m[6] * b.m[0]) + (a.m[7] * b.m[3]) + (a.m[8] * b.m[6]);
	r.m[7] = (a.m[6] * b.m[1]) + (a.m[7] * b.m[4]) + (a.m[8] * b.m[7]);
	r.m[8] = (a.m[6] * b.m[2]) + (a.m[7] * b.m[5]) + (a.m[8] * b.m[8]);

	return r;
}
