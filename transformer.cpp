#include "transformer.h"

#include <string.h>
#include <math.h>

// rotate around x axis
void trans3d_rotatex(mat4 &c, const REAL radians)
{
	memset(&c, 0, sizeof(mat4));

	c.m11 = 1;
	c.m44 = 1;

	c.m22 = cos(radians);
	c.m23 = sin(radians);

	c.m32 = -sin(radians);
	c.m33 = cos(radians);
}

// rotate around y axis
void trans3d_rotatey(mat4 &c, const REAL radians)
{
	memset(&c, 0, sizeof(mat4));
	
	c.m21 = 1;
	c.m44 = 1;

	c.m11 = cos(radians);
	c.m13 = -sin(radians);

	c.m31 = sin(radians);
	c.m33 = cos(radians);
}

// rotate around z axis
void trans3d_rotatez(mat4 &c, const REAL radians)
{
	memset(&c, 0, sizeof(mat4));

	c.m33 = 1;
	c.m44 = 1;

	c.m11 = cos(radians);
	c.m12 = sin(radians);

	c.m21 = -sin(radians);
	c.m22 = cos(radians);
}

// Rotate around an arbitrary axis
void trans3d_rotate_around_axis(mat4 &c, const real3 n, const REAL radians)
{
	REAL nx = n[0];
	REAL ny = n[1];
	REAL nz = n[2];
	REAL nx2 = n[0] * n[0];
	REAL ny2 = n[1] * n[1];
	REAL nz2 = n[2] * n[2];

	REAL cosR = cos(radians);
	REAL sinR = sin(radians);
	REAL onemincosR = 1 - cosR;
	REAL oneminsinR = 1 - sinR;

	// row 1
	c.m11 = nx2 * onemincosR + cosR;
	c.m12 = n[0] * n[1] * onemincosR + n[2] * sinR;
	c.m13 = n[0] * n[2] * onemincosR - n[1] * sinR;


	//row 2
	c.m21 = nx*ny*onemincosR - nz*sinR;
	c.m22 = ny2*onemincosR + cosR;
	c.m23 = ny*nz*onemincosR + nx*sinR;

	// row 3
	c.m31 = nx*nz*onemincosR + ny*sinR;
	c.m32 = ny*nz*onemincosR - nx*sinR;
	c.m33 = nz2*onemincosR + cosR;

	c.m44 = 1;
}


// Create a scaling matrix using all 3 axis
void trans3d_scale(mat4 &c, const REAL sx, const REAL sy, const REAL sz)
{
	memset(&c, 0, sizeof(mat4));

	c.m11 = sx;
	c.m22 = sy;
	c.m33 = sz;
	c.m44 = 1;
}




// Render pipeline transformation matrices
void trans3d_clip_perspective(mat4 &c, const REAL zoomx, const REAL zoomy, const REAL near, const REAL far)
{
	memset(&c, 0, sizeof(mat4));

	c.m34 = 1;

	c.m11 = zoomx;
	c.m22 = zoomy;
	c.m33 = (far + near) / (far - near);
	c.m43 = (-1 * near*far) / (far - near);
}


void trans3d_clip_orthographic(mat4 &c, const REAL zoomx, const REAL zoomy, const REAL near, const REAL far)
{
	memset(&c, 0, sizeof(mat4));


	c.m11 = zoomx;
	c.m22 = zoomy;
	c.m33 = (2) / (far - near);
	c.m43 = -((far+near) / (far - near));
	c.m44 = 1;
}

void trans3d_map_to_window(REAL &screenx, REAL &screeny, 
	const REAL clipx, const REAL clipy, const REAL clipw, 
	const REAL winResx, const REAL winResy, 
	const REAL winCenterx, const REAL winCentery)
{
	screenx = ((clipx*winResx) / (2 * clipw)) + winCenterx;
	screeny = ((clipy*winResy) / (2 * clipw)) + winCentery;
}
