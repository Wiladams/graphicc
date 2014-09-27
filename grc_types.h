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

typedef REAL real3x3[3][3];
typedef REAL real4x4[4][4];

#ifdef _MSC_VER
#pragma warning(pop)
#endif

#endif
