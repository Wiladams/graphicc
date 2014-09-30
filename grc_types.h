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

typedef struct _real2x2 {
	real2 row1;
	real2 row2;
} real2x2;

typedef struct _real3x3 {
	real3 row1;
	real3 row2;
	real3 row3;
} real3x3;

typedef REAL real4x4[4][4];

#ifdef _MSC_VER
#pragma warning(pop)
#endif

#endif
