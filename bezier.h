#pragma once

#ifndef BEZIER_H
#define BEZIER_H

#include "graphicc.h"

#ifdef __cplusplus
extern "C" {
#endif

// calculate a factorial for an integer
int intfactorial(const int n);

// computeCoefficients
// n represents the number of control points
// c is the array containing the calculated coefficients
void computeCoefficients(const int n, int * c);

// calculate a specific point on a bezier curve
// u			- a value between 0.0 and 1.0, 0.0 is beginning of curve
//				1.0 is end if curve
// pt			- the structure which is filled in with the calculated point
// nControls	- The number of control points
// controls		- The array of control points
// c			- calculated coefficients
void bezierPoint(const float u, Pt3 *pt, const int nControls, const Pt3 *controls, const int * c);

//
// general bezier curve calculation of arbitrary onts
void bezier(const Pt3 *controls, const int nControls, const int m, Pt3 * curve);

//
// cubic bezier
// controls		- 4 control points
// m			- number of segments in curve
// curve		- array containing calculated points along curve
//
void bezier4(const Pt3 *controls, const int m, Pt3 * curve);

#ifdef __cplusplus
}
#endif

#endif
