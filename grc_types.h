#ifndef geomtypes_h
#define geomtypes_h

#include "grc_config.h"

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4201)  // nonstandard extension used : nameless struct/union
#endif



typedef REAL real2[2];
typedef REAL real3[3];
typedef REAL real4[4];

typedef struct _mat2 {
	real2 row1;
	real2 row2;
} mat2;

typedef struct _mat3 {
	real3 row1;
	real3 row2;
	real3 row3;
} mat3;

typedef struct _mat4 {
	real4 row1;
	real4 row2;
	real4 row3;
	real4 row4;
} mat4;

#ifdef _MSC_VER
#pragma warning(pop)
#endif

#endif
