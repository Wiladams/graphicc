
#include "bezier.h"


#include <math.h>
#include <stdlib.h>


int intfactorial(const int n)
{
	int c = 1;
	for (int k = 1; k <= n; k++)
		c *= k;
	
	return c;
}

void computeCoefficients(const int n, int * c)
{
	int k, i;

	for (k = 0; k <= n; k++)
	{
		// compute n!/(k!(n-k)!)
		c[k] = 1;
		for (i = n; i >= k + 1; i--)
		{
			c[k] *= i;
		}

		for (i = n - k; i >= 2; i--)
		{
			c[k] /= i;
		}
	}
}

void computePoint(const float u, Pt3 * pt, const int nControls, const Pt3 *controls, const int * c)
{
	int k;
	int n = nControls - 1;
	float blend;

	pt->x = 0.0;	// x
	pt->y = 0.0;	// y
	pt->z = 0.0;	// z
	
	// Add in influence of each control point
	for (k = 0; k < nControls; k++){
		blend = c[k] * powf(u, k) *powf(1 - u, n - k);
		pt->x += controls[k].x * blend;
		pt->y += controls[k].y * blend;
		pt->z += controls[k].z * blend;
	}
}

void bezier(const Pt3 *controls, const int nControls, const int m, Pt3 * curve)
{
	// create space for the coefficients
	int * c = (int *)malloc(nControls * sizeof(int));
	int i;

	computeCoefficients(nControls - 1, c);
	for (i = 0; i <= m; i++) {
		computePoint(i / (float)m, &curve[i], nControls, controls, c);
	}
	free(c);	
}
