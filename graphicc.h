#pragma once 

#ifndef graphicc_h
#define graphicc_h

#include <stdint.h>

typedef double REAL;

// Some useful constants
#define M_PI 3.14159

#define DEGREES(r) ((180 / M_PI) * r)
#define RADIANS(d) ((M_PI/180)*d)


#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4201)  // nonstandard extension used : nameless struct/union
#endif



typedef REAL real2[2];
typedef REAL real3[3];
typedef REAL real4[4];

typedef struct _mat2 {
	REAL m11, m12;
	REAL m21, m22;
} mat2;

typedef struct _mat3 {
	REAL m11, m12, m13;
	REAL m21, m22, m23;
	REAL m31, m32, m33;
} mat3;

typedef struct _mat4 {
	REAL m11, m12, m13, m14;
	REAL m21, m22, m23, m24;
	REAL m31, m32, m33, m34;
	REAL m41, m42, m43, m44;
} mat4;

enum pixellayouts {
	rgba
};


#ifdef _MSC_VER
#pragma warning(pop)
#endif

#endif
