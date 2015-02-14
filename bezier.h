#pragma once

#ifndef BEZIER_H
#define BEZIER_H

#include "graphicc.h"

#ifdef __cplusplus
extern "C" {
#endif

int intfactorial(const int n);

void computeCoefficients(const int n, int * c);
void computePoint(const float u, Pt3 *pt, const int nControls, const Pt3 *controls, const int * c);
void bezier(const Pt3 *controls, const int nControls, const int m, Pt3 * curve);

#ifdef __cplusplus
}
#endif

#endif
