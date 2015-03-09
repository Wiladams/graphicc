#include "pipeline_opengl.h"
#include "linearalgebra.h"

#include <string.h>
#include <math.h>

// convenience
// Multiply rows against transformation matrix
// assume columnar vectors on the right of the matrix
void ogl_transform_rows(const size_t nrows, REAL *res, const REAL *inpts, const mat4 &tmat)
{
	for (size_t idx = 0; idx < nrows; idx++)
	{
		mat4_mul_col4(&res[idx * 4], tmat, &inpts[idx * 4]);
	}
}

// translation
void ogl_translate(mat4 &c, const REAL dx, const REAL dy, const REAL dz)
{
	memset(&c, 0, sizeof(mat4));
	c.m11 = 1;
	c.m22 = 1;
	c.m33 = 1;
	c.m44 = 1;

	c.m41 = dx;
	c.m42 = dy;
	c.m43 = dz;
}

// Create a scaling matrix using all 3 axis
// scales along each of the axis from the origin
void ogl_scale(mat4 &c, const real3 sxyz)
{
	memset(&c, 0, sizeof(mat4));

	c.m11 = sxyz[0];
	c.m22 = sxyz[1];
	c.m33 = sxyz[2];
	c.m44 = 1;
}

// rotate around x axis
void ogl_rotatex(mat4 &c, const REAL radians)
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
void ogl_rotatey(mat4 &c, const REAL radians)
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
void ogl_rotatez(mat4 &c, const REAL radians)
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
// rotate around an axis with its center
// at the origin
void ogl_rotate_around_axis(mat4 &c, const real3 n, const REAL radians)
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




void ogl_set_rotation(mat4 &c, const mat3 &rot)
{
	c.m11 = rot.m11; 
	c.m12 = rot.m12;
	c.m13 = rot.m13;

	c.m21 = rot.m21;
	c.m22 = rot.m22;
	c.m23 = rot.m23;

	c.m31 = rot.m31;
	c.m32 = rot.m32;
	c.m33 = rot.m33;

}

void ogl_lookat(mat4 &mat, const real3 eyexyz, const real3 atxyz, const real3 upxyz)
{
	REAL * m = (REAL*)&mat;
	REAL *xaxis = &m[0];
	REAL *up = &m[4];
	REAL *at = &m[8];
	real3 tmpreal3;

	// Compute our new look at vector, which will be
	//   the new negative Z axis of our transformed object.
	real3_sub(tmpreal3, atxyz, eyexyz);
	real3_normalize(at, tmpreal3);

	// Make a useable copy of the current up vector.
	up[0] = upxyz[0]; up[1] = upxyz[1]; up[2] = upxyz[2];

	// Cross product of the new look at vector and the current
	//   up vector will produce a vector which is the new
	//   positive X axis of our transformed object.
	real3_cross(tmpreal3, at, up);
	real3_normalize(xaxis, tmpreal3);

	// Calculate the new up vector, which will be the
	//   positive Y axis of our transformed object. Note
	//   that it will lie in the same plane as the new
	//   look at vector and the old up vector.
	real3_cross(up, xaxis, at);

	// Account for the fact that the geometry will be defined to
	//   point along the negative Z axis.
	//scale(at, -1.f);
	real3_mul_scalar(at, at, -1);

	// Fill out the rest of the 4x4 matrix
	m[3] = 0.f;     // xaxis is m[0..2]
	m[7] = 0.f;     // up is m[4..6]
	m[11] = 0.f;    // -at is m[8..10]
	m[12] = eyexyz[0]; m[13] = eyexyz[1]; m[14] = eyexyz[2];
	m[15] = 1.0f;
}

/*
void ogl_lookat(mat4 &c, const real3 eye, const real3 lookAt, const real3 up)
{
	real3 tmp1Real3;

	
	real3 viewDir;
	real3 upN;
	real3 viewSide;
	real3 viewUp;
	mat4 M;

	real3_sub(tmp1Real3, lookAt, eye);
	real3_normalize(viewDir, tmp1Real3);

	real3_normalize(upN, up);
	real3_cross(viewSide, viewDir, upN);
	real3_cross(viewUp, viewSide, viewDir);

	M.m11 = viewSide[0]; M.m12 = viewUp[0]; M.m13 = -viewDir[0]; M.m14 = 0;
	M.m21 = viewSide[1]; M.m22 = viewUp[1]; M.m23 = -viewDir[1]; M.m24 = 0;
	M.m31 = viewSide[2]; M.m32 = viewUp[2]; M.m33 = -viewDir[2]; M.m34 = 0;

	// final translation
	M.m41 = -eye[0]; M.m42 = -eye[1]; M.m43 = -eye[2]; M.m44 = 1;
}
*/



// Render pipeline transformation matrices
void ogl_perspective(mat4 &c, const REAL zoomx, const REAL zoomy, const REAL near, const REAL far)
{
	memset(&c, 0, sizeof(mat4));


	c.m11 = zoomx;
	c.m22 = zoomy;
	c.m33 = -(far + near) / (far - near);
	c.m43 = (-2 * near*far) / (far - near);
	c.m34 = -1;
}


void ogl_orthographic(mat4 &c, const REAL zoomx, const REAL zoomy, const REAL near, const REAL far)
{
	memset(&c, 0, sizeof(mat4));


	c.m11 = zoomx;
	c.m22 = zoomy;
	c.m33 = -(2) / (far - near);
	c.m43 = -((far+near) / (far - near));
	c.m44 = 1;
}

// receives normalized coords
// x := -1 (left), 1 (right)
// y := -1 (bottom), 1 (top)
//
void ogl_map_to_window(REAL &screenx, REAL &screeny, 
	const REAL clipx, const REAL clipy, const REAL clipw, 
	const REAL winResx, const REAL winResy, 
	const REAL winCenterx, const REAL winCentery)
{
	screenx = ((clipx*winResx) / (2 * clipw)) + winCenterx;
	screeny = ((clipy*winResy) / (2 * clipw)) + winCentery;
}




