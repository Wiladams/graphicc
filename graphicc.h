/*
Copyright 2015 William A Adams

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http ://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#pragma once

#ifndef graphicc_h
#define graphicc_h

#include <stdint.h>

typedef double REAL;

// Some useful constants
#ifndef M_PI
#define M_PI			3.14159265358979323846264338327950288
#define M_ROOT_PI		1.772453850905516027
#define M_HALF_PI		1.57079632679489661923132169163975144
#define M_QUARTER_PI	0.785398163397448309615660845819875721
#define M_ONE_OVER_PI	0.318309886183790671537767526745028724

#define M_E				2.71828182845904523536
#define M_EULER			0.577215664901532860606

#endif	// M_PI

#define M_GOLDEN_RATIO	1.61803398874989484820458683436563811
#define DEGREES(radians) ((180 / M_PI) * radians)
#define RADIANS(degrees) ((M_PI/180)*degrees)



#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4201)  // nonstandard extension used : nameless struct/union
#endif



typedef REAL real2[2];
typedef REAL real3[3];
typedef REAL real4[4];

typedef struct {
	REAL x;
	REAL y;
	REAL z;
} Pt3;


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

typedef REAL mat4x4[4][4];


enum pixellayouts {
	rgba
};


#ifdef _MSC_VER
#pragma warning(pop)
#endif

#endif
