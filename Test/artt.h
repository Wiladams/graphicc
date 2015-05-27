// AART.h
/*
	Inspired by interfaces found in the book: 
	Real-Time Animation Toolkit in C++
	By: Rex E. Bradford

	Basically setting up a classic software 3D Rendering pipeline
	Some more modern C++ constructs are used than what the book
	presents.
	Ex: Using unions for certain data structures
	Liberal usage of 'const'
	caml casing of function names

	The Perspective Viewing Pipeline
	1. Rotate polygon to its desired orientation in world coordinates.
	2. Translate polygon from local coordinates to world coordinates.
	3. Translate polygon from world coordinates to view coordinates.
	4. Rotate polygon about camera's orientation
	5. Scale polygon based on viewport aspect ratio and field-of-view
	6. Project polygon's view coordinates onto screen coordinates
	7. Draw the projected polygon.

	This is a much simpler rendering pipleline compared to say OpenGL
	It does not require the usage of homogeneous coordinates.
	It does not support partial polygon drawing though, so clipping
	is an all or nothing affair.
*/
#include <memory.h>
#include <math.h>
#include <stdint.h>
#include "graphicc.h"
#include "raster_rgba.h"

#define CBlack RGBA(0,0,0,255)

struct APolyVertex {
	real x;
	real y;
};

static const int  MAX_POLY_VERTICES = 8;

typedef enum POLY_SHADING_TYPE {
	FLAT = 0,
	TMAP = 1
};


struct APolygon {
	union {
		uint32_t colorNative;
		// ABitmap *tmap;
	};
	uint16_t	shading;	// What kind of shading
	short		numVerts;
	APolyVertex *pVerts;
	uint8_t		vertIndex[MAX_POLY_VERTICES];
	//ATmapCoord  *pTmapCoords;		// Texture map coordinates
};

// Representation of a 3D vector/point
// Use this union formulation so that elements
// can be accessed either through array index
// or through field names.
struct AVector {
	union {
		struct {
			real x, y, z;
		};
		real _v[3];
	};

	AVector(){}
	AVector(const real x_, const real y_, const real z_) {x = x_;y = y_;z = z_;}

	AVector & operator =(const AVector &rhs) { x = rhs.x; y = rhs.y; z = rhs.z; return *this; }

	// Simple math
	AVector & operator+=(const AVector &rhs) { x += rhs.x; y += rhs.y; z += rhs.z; return *this; }
	AVector & operator-=(const AVector &rhs) { x -= rhs.x; y -= rhs.y; z -= rhs.z; return *this; }

	friend AVector operator+(const AVector &a, const AVector &b) { 
		AVector r(a.x+b.x, a.y+b.y, a.z+b.z);
		return r;
	}

	friend AVector operator-(const AVector &a, const AVector &b) {
		AVector r(a.x - b.x, a.y - b.y, a.z - b.z);
		return r;
	}

	AVector & operator*=(const real s) { x *= s; y *= s; z *= s; return *this; }

	friend AVector operator *(const AVector &a, const real s)
	{
		AVector r(a.x*s, a.y*s, a.z*s);
		return r;
	}

	// Vector length/magnitude and normalize
	real magnitude() { return sqrt(x*x + y*y + z*z); }
	AVector & normalizeSelf() {
		real recipMag = 1.0f / magnitude();
		*this *= recipMag;
		return *this;
	}

	// Vector products and Normal calculation
	friend real dotProduct(const AVector &a, const AVector &b){
		return ((a.x + b.x) + (a.y*b.y) + (a.z*b.z));
	}

	friend AVector crossProduct(const AVector &a, const AVector &b)
	{
		AVector r;
		r.x = (a.y*b.z) - (a.z*b.y);
		r.y = (a.z*b.x) - (a.x*b.z);
		r.z = (a.x*b.y) - (a.y*b.x);
		return r;
	}

	// The three points (a, b, c) taken in counter-clockwise order
	// define a plane. 
	// triNormal(), calculates a normal pointing out of that plane.
	friend AVector triNormal(const AVector &a, const AVector &b, const AVector &c)
	{
		AVector norm = crossProduct(b - a, b - c);
		norm.normalizeSelf();
		return norm;
	}
};


// Matrix Layout
// m[0]  m[1]  m[2]
// m[3]  m[4]  m[5]
// m[6]  m[7]  m[8]


struct AMatrix {
	real m[9];

	// Constructors
	AMatrix() {};
	AMatrix(real m11, real m12, real m13,
		real m21, real m22, real m23,
		real m31, real m32, real m33)
	{
		m[0] = m11; m[1] = m12; m[2] = m13;
		m[3] = m21; m[4] = m22; m[5] = m23;
		m[6] = m31; m[7] = m32; m[8] = m33;
	}

	AMatrix & operator=(AMatrix &a)
	{
		memcpy(m, a.m, sizeof(m));
		return *this;
	}

	AMatrix & setIdentity()
	{
		memset(m, 0, sizeof(m));
		m[0] = 1;
		m[4] = 1;
		m[8] = 1;

		return *this;
	}

	// Matrix computations
	AMatrix & scaleXYZ(const AVector &v);
	friend AVector operator*(const AVector &v, const AMatrix &m);
	friend AMatrix operator*(const AMatrix &a, const AMatrix &b);
};


static const real FOV_90 = 1.5708;

struct ACamera {
	AVector loc;		// Camera location
	AVector dir;		// Camera pitch, head, bank
	real fov;
	int viewportW;
	int viewportH;
	real viewportAspect;
	AMatrix m;			// matrix to transform world->view coordinates

	void CalcViewMatrix();

	ACamera(){}
	ACamera(const AVector &loc_, const AVector &dir_, int vpw, int vph, real fov_ = FOV_90)
	{
		setViewport(vpw, vph, fov_, false);
		setLocAndDir(loc_, dir_);
	}

	AVector getLocation() { return loc; }
	AVector getDirection() { return dir; }
	real getFOV() { return fov; }
	int getViewportWidth() { return viewportW; }
	int getViewportHeight() { return viewportH; }
	real getViewportAspect() { return viewportAspect; }

	// Set Camera properties
	void setLocation(const AVector &loc_) { loc = loc_; CalcViewMatrix(); }
	void setDirection(const AVector &dir_) { dir = dir_; CalcViewMatrix(); }
	
	void setLocAndDir(const AVector &loc_, const AVector &dir_) {
		loc = loc_; dir = dir_;
		CalcViewMatrix();
	}

	void setViewport(const int w, const int h, real fov_, bool recalc = true)
	{
		viewportW = w; viewportH = h; fov = fov_;
		viewportAspect = float(h) / float(w);
		if (recalc) CalcViewMatrix();
	}
	void setFOV(real fov_) { fov = fov_; CalcViewMatrix(); }

	// Transform and project points
	void worldToView(const AVector &ptsrc, AVector &ptdest);
	bool project(const AVector &ptsrc, APolyVertex &pvdest);

	// Check if transformed poly (screen coords) is facing forward
	static bool isFacing(APolyVertex &v1, APolyVertex &v2, APolyVertex &v3);
};


struct ACanvas {
	pb_rgba *bm;
	pb_rect clipRect;
	uint32_t colorValue;

	ACanvas(pb_rgba *pbm);
	
	virtual ~ACanvas(){};

	void setClipRect(const pb_rect &rect){ clipRect.x = rect.x; clipRect.y = rect.y; clipRect.width = rect.width; clipRect.height = rect.height; }
	void getClipRect(int &left, int &top, int &width, int &height) {
		left = clipRect.x; top = clipRect.y;
		width = clipRect.width; height = clipRect.height;
	}

	void drawHorizontalLine(const int y, const int left, const int right)
	{
		raster_rgba_hline(bm, left, y, right - left, colorValue);
	}

	void drawVerticalLine(const int x, const int top, const int bottom)
	{
		raster_rgba_vline(bm, x, top, bottom - top, colorValue);
	}

	void drawLine(const int x1, const int y1, const int x2, const int y2)
	{
		raster_rgba_line(bm, x1, y1, x2, y2, colorValue);
	}

	void drawRect(const pb_rect &rect)
	{
		raster_rgba_rect_fill(bm, rect.x, rect.y, rect.width, rect.height, colorValue);
	}

	void drawBox(const pb_rect &rect)
	{
		drawHorizontalLine(rect.y, rect.x, rect.x + rect.width);
		drawHorizontalLine(rect.y+rect.height, rect.x, rect.x + rect.width);

		// sides
	}

	void drawBitmap(pb_rgba *bms, const int x, const int y)
	{
		raster_rgba_blit(bm, x, y, bms);
	}
};