// AART.h
#include <memory.h>
#include <math.h>

typedef float real;

// Representation of a 3D vector/point
// Use this union formulation so that elements
// can be accessed either through array index
// or through field names.
struct AVector {
	union {
		struct {
			real x, y, z;
		};
		real _v[3];
	};

	AVector(){}
	AVector(const real x_, const real y_, const real z_) {x = x_;y = y_;z = z_;}

	AVector & operator =(const AVector &rhs) { x = rhs.x; y = rhs.y; z = rhs.z; return *this; }

	// Simple math
	AVector & operator+=(const AVector &rhs) { x += rhs.x; y += rhs.y; z += rhs.z; return *this; }
	AVector & operator-=(const AVector &rhs) { x -= rhs.x; y -= rhs.y; z -= rhs.z; return *this; }

	friend AVector operator+(const AVector &a, const AVector &b) { 
		AVector r(a.x+b.x, a.y+b.y, a.z+b.z);
		return r;
	}

	friend AVector operator-(const AVector &a, const AVector &b) {
		AVector r(a.x - b.x, a.y - b.y, a.z - b.z);
		return r;
	}

	AVector & operator*=(const real s) { x *= s; y *= s; z *= s; return *this; }

	friend AVector operator *(const AVector &a, const real s)
	{
		AVector r(a.x*s, a.y*s, a.z*s);
		return r;
	}

	// Vector length/magnitude and normalize
	real magnitude() { return sqrt(x*x + y*y + z*z); }
	AVector & normalizeSelf() {
		real recipMag = 1.0f / magnitude();
		*this *= recipMag;
		return *this;
	}

	// Vector products and Normal calculation
	friend real dotProduct(const AVector &a, const AVector &b){
		return ((a.x + b.x) + (a.y*b.y) + (a.z*b.z));
	}

	friend AVector crossProduct(const AVector &a, const AVector &b)
	{
		AVector r;
		r.x = (a.y*b.z) - (a.z*b.y);
		r.y = (a.z*b.x) - (a.x*b.z);
		r.z = (a.x*b.y) - (a.y*b.x);
		return r;
	}

	// The three points (a, b, c) taken in counter-clockwise order
	// define a plane. 
	// triNormal(), calculates a normal pointing out of that plane.
	friend AVector triNormal(const AVector &a, const AVector &b, const AVector &c)
	{
		AVector norm = crossProduct(b - a, b - c);
		norm.normalizeSelf();
		return norm;
	}
};


// Matrix Layout
// m[0]  m[1]  m[2]
// m[3]  m[4]  m[5]
// m[6]  m[7]  m[8]


struct AMatrix {
	real m[9];

	// Constructors
	AMatrix() {};
	AMatrix(real m11, real m12, real m13,
		real m21, real m22, real m23,
		real m31, real m32, real m33)
	{
		m[0] = m11; m[1] = m12; m[2] = m13;
		m[3] = m21; m[4] = m22; m[5] = m23;
		m[6] = m31; m[7] = m32; m[8] = m33;
	}

	AMatrix & operator=(AMatrix &a)
	{
		memcpy(m, a.m, sizeof(m));
		return *this;
	}

	AMatrix & setIdentity()
	{
		memset(m, 0, sizeof(m));
		m[0] = 1;
		m[4] = 1;
		m[8] = 1;

		return *this;
	}

	// Matrix computations
	AMatrix & scaleXYZ(const AVector &v);
	friend AVector operator*(const AVector &v, const AMatrix &m);
	friend AMatrix operator*(const AMatrix &a, const AMatrix &b);
};
