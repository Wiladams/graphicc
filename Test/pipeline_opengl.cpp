#include "pipeline_opengl.h"
#include "linearalgebra.h"

#include <string.h>
#include <math.h>

// convenience
// Multiply rows against transformation matrix
// assume columnar vectors on the right of the matrix
void ogl_transform_rows(REAL *res, const mat4 &tmat, const REAL *inpts, const size_t nrows)
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

void ogl_lookat(mat4 &mat, const real3 eye, const real3 center, const real3 up)
{
	REAL * m = (REAL*)&mat;

	real3 tmpreal3;
	real3 f;
	real3 upN;

	real3 s;
	//REAL *s = &m[0];

	real3 u;
	//REAL *u = &m[4];

	// Compute our new look at vector, which will be
	//   the new negative Z axis of our transformed object.
	real3_sub(tmpreal3, center, eye);
	real3_normalize(f, tmpreal3);
	real3_normalize(upN, up);

	real3_cross(s, f, upN);
	real3_cross(u, s, f);

	m[0]  = s[0];    m[1]  = s[1];     m[2]  = s[2];     m[3]  = 0;
	m[4]  = u[0];    m[5]  = u[1];     m[6]  = u[2];     m[7]  = 0;
	m[8]  = -f[0];   m[9]  = -f[1];    m[10] = -f[2];    m[11] = 0;
	m[12] = -eye[0]; m[13] = -eye[1];  m[14] = -eye[2];  m[15] = 1;


}

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

mat4 ogl_ortho(REAL left, REAL right, REAL bottom, REAL top, REAL n, REAL f)
{
	mat4 r;
	r.m11 = 2.0f / (right - left);           r.m12 = 0,                               r.m13 = 0;                 r.m14 = 0;
	r.m21 = 0;                               r.m22 = 2.0 / (top - bottom);            r.m23 = 0.0f;              r.m24 = 0.0f;
	r.m31 = 0.0f;                            r.m32 = 0.0f;                            r.m33 = 2.0f / (n - f);    r.m34 = 0.0f;
	r.m41 = (left + right) / (left - right); r.m42 = (bottom + top) / (bottom - top); r.m43 = (n + f) / (f - n); r.m44 = 1.0f;

	return r;
}

// receives normalized coords
// clipx := -1 (left), 1 (right)
// clipy := -1 (bottom), 1 (top)
//
void ogl_map_to_window(REAL &screenx, REAL &screeny, 
	const REAL clipx, const REAL clipy, const REAL clipw, 
	const REAL winResx, const REAL winResy, 
	const REAL winCenterx, const REAL winCentery)
{
	screenx = ((clipx*winResx) / (2 * clipw)) + winCenterx;
	screeny = -((clipy*winResy) / (2 * clipw)) + winCentery;
}

void ogl_create_ndc_to_window(const REAL Ds,
	const REAL winResx, const REAL winResy,
	const REAL Sx, const REAL Sy)
{
	REAL Wshalf = winResx / 2.0f;
	REAL Hshalf = winResy / 2.0f;
	REAL Dshalf = Ds / 2.0f;

	mat4 m = {
		Wshalf, 0, 0, 0,
		0, -Hshalf, 0, 0,
		0, 0, Dshalf, 0,
		Wshalf + Sx, Hshalf + Sy, Dshalf, 1
	};

}



