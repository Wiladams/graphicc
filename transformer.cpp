#include "transformer.h"
#include "linearalgebra.h"

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

void trans3d_set_rotation(mat4 &c, const mat3 &rot)
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

// For the render pipeline
// model -> view
// view > camera
// camera -> perspective
// perspective -> screen
void trans3d_lookat(mat4 &c, const real3 eye, const real3 lookAt, const real3 up)
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

/*
void trans3d_lookat(mat4 &c, const real3 eye, const real3 lookAt, const real3 up)
{
	real3 tmp1Real3;
	real3 tmp2Real3;
	real3 tmp3Real3;

	real3 viewDir;
	real3 viewSide;
	real3 viewUp;

	// viewDir = normalize(lookat - eye)
	// This is the translation portion of the matrix
	real3_sub(tmp1Real3, lookAt, eye);
	real3_normalize(viewDir, tmp1Real3);

	// viewUp = up - up.Dot(viewDir)*viewDir
	real3_dot(tmp1Real3, up, viewDir);
	real3_mul_real3(tmp2Real3, tmp1Real3, viewDir);
	real3_sub(tmp3Real3, up, tmp2Real3);

	// viewUp.normalize();
	real3_normalize(viewUp, tmp3Real3);

	// viewSide = viewDir.Cross(viewUp)
	real3_cross(viewSide, viewDir, viewUp);

	// build transposed rotation matrix
	mat3 rotation;
	// rotate.SetRows(viewSide, viewUp, -viewDir);
	real3_neg(tmp1Real3, viewDir);
	mat3_set_columns(rotation, viewSide, viewUp, tmp1Real3);

	// transform translation
	// eyeInv = -(rotate*eye)
	real3 eyeInv;
	row3_mul_mat3(tmp1Real3, eye, rotation);
	real3_neg(eyeInv, tmp1Real3);

	// build 4x4 matrix
	trans3d_set_rotation(c, rotation);
	c.m41 = eyeInv[0];
	c.m42 = eyeInv[1];
	c.m43 = eyeInv[2];
}
*/

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

// receives normalized coords
// x := -1 (left), 1 (right)
// y := -1 (bottom), 1 (top)
//
void trans3d_map_to_window(REAL &screenx, REAL &screeny, 
	const REAL clipx, const REAL clipy, const REAL clipw, 
	const REAL winResx, const REAL winResy, 
	const REAL winCenterx, const REAL winCentery)
{
	screenx = ((clipx*winResx) / (2 * clipw)) + winCenterx;
	screeny = ((clipy*winResy) / (2 * clipw)) + winCentery;
}

// convenience
void trans3d_transform_rows(const size_t nrows, REAL *res, const REAL *inpts, mat4 &tmat)
{
	for (size_t idx = 0; idx < nrows; idx++)
	{
		row4_mul_mat4(&res[idx * 4], &inpts[idx * 4], tmat);
	}
}


