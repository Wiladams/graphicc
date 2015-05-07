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


#pragma comment(lib,"graphicc.lib")


#ifndef graphicc_h
#define graphicc_h

#include <stdint.h>
#include <string.h>
#include <math.h>

#define BGR_DOMINANT 1


typedef float	float32;
typedef double	float64;
typedef double	REAL;

// There are various sources for high precision numbers.  The well known
// CRC books are one.  The ones used here come variously from the Graphics Gems
// books, as well as the glm library, or direct derivation
// the 'G_' prefix is used to avoid potential conflict with the 'M_' prefix that might
// be used in math.h
#define G_PI     3.141592653589793238462643383279502884197169399375105820975
#define G_E      2.718281828459045235360287471352662497757247093699959574967
#define G_SQRT2  1.414213562373095048801688724209698078569671875376948073177
#define G_SQRT3  1.732050807568877293527446341505872366942805253810380628056
#define G_GOLDEN 1.618033988749894848204586834365638117720309179805762862135

// derived constants
#define G_PITIMES2  (2.0*G_PI)
#define G_PIOVER2   (0.5*G_PI)
#define G_PIOVER4	(0.25*G_PI)
#define G_ONE_OVER_PI	(1.0/G_PI)
//#define G_ONE_OVER_PI	0.318309886183790671537767526745028724
#define G_ROOT_PI		1.772453850905516027

#define G_DTOR      (G_PI/180.0)
#define G_RTOD      (180.0/G_PI)

// macro and constant for base 2 logarithm
#define G_LN2    0.693147180559945309417232121458176568075500134360255254121
#define G_LOG2(val) (log(val)*(1.0/G_LN_2))

#define G_EULER			0.577215664901532860606

// A really small value
#define EPSILON		0.0000000001



inline REAL DEGREES(const REAL radians) {return ((180 / G_PI) * radians);}
inline REAL RADIANS(const REAL degrees) {return ((G_PI / 180)*degrees);}

// map a value (a) from between rlo <= a <= rhi to  shi <= b <= slo
inline double MAP(double a, double rlo, double rhi, double slo, double shi) {return slo + ((double)(a - rlo) / (rhi - rlo)) * (shi - slo);}

// turn a division by 255 into something 
// much cheaper to calculate
// for values between 0 and 65534
#define div255(num) ((num + (num >> 8)) >> 8)
#define lerp255(bg, fg, a) ((uint8_t)div255((fg*a+bg*(255-a))))



#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4201)  // nonstandard extension used : nameless struct/union
#endif


enum pixellayouts {
	rgba
};

#ifdef BGR_DOMINANT
#define RGBA(r,g,b,a) ((uint32_t)(a<<24|r<<16|g<<8|b))
#define GET_B(value) ((uint32_t)value &0xff)
#define GET_G(value) (((uint32_t)value &0xff00) >> 8)
inline uint8_t GET_R(const uint32_t value) {return (((uint32_t)value & 0xff0000) >> 16);}

#define GET_A(value) (((uint32_t)value &0xff000000) >> 24)
#else
#define RGBA(r,g,b,a) ((uint32_t)(a<<24|b<<16|g<<8|r))
#define GET_R(value) ((uint32_t)value &0xff)
#define GET_G(value) (((uint32_t)value &0xff00) >> 8)
#define GET_B(value) (((uint32_t)value &0xff0000) >> 16)
#define GET_A(value) (((uint32_t)value &0xff000000) >> 24)
#endif

// pixel buffer color
typedef struct {
	union {
		struct {
			uint8_t r, g, b, a;
		};
		uint32_t value;
	};
} pix_rgba;

// pixel buffer rectangle
typedef struct _pb_rect {
	int x, y;
	int width, height;
} pb_rect;

inline int pb_rect_contains_point(const pb_rect &rct, const int x, const int y)
{
	if ((x < rct.x) || (y < rct.y))
		return 0;

	if ((x >= rct.x + rct.width) || (y >= rct.y + rct.height))
		return 0;

	return 1;
}

inline int pb_rect_contains_rect(const pb_rect &container, const pb_rect &other)
{
	if (!pb_rect_contains_point(container, other.x, other.y))
	{
		return 0;
	}

	if (!pb_rect_contains_point(container, other.x + other.width - 1, other.y + other.height - 1))
	{
		return 0;
	}

	return 1;
}

// return the intersection of rectangles a and b
// if there is no intersection, one or both of width and height
// will be == zero
inline void pb_rect_intersection(pb_rect &c, const pb_rect &a, const pb_rect &b)
{
	int x = a.x > b.x ? a.x : b.x;
	int y = a.y > b.y ? a.y : b.y;
	int right = ((a.x + a.width - 1) < (b.x + b.width - 1)) ? (a.x + a.width - 1) : (b.x + b.width - 1);
	int bottom = ((a.y + a.height - 1) < (b.y + b.height - 1)) ? (a.y + a.height - 1) : (b.y + b.height - 1);

	int width = ((right - x) > 0) ? (right - x) : 0;
	int height = ((bottom - y) > 0) ? (bottom - y) : 0;

	c.x = x;
	c.y = y;
	c.width = width;
	c.height = height;
}

// returns whether two rectangles intersect
// if the intersection would result in a rectangle with zero
// width or height, the answer is 'false'
inline bool pb_rect_intersect(const pb_rect &a, const pb_rect &b)
{
	return (a.x < (b.x + b.width - 1)) &&
		(b.x < (a.x + a.width - 1)) &&
		(a.y < (b.y + b.height - 1)) &&
		(b.y < (a.y + a.height - 1));
}

inline void pb_rect_clear(pb_rect &rect)
{
	memset(&rect, 0, sizeof(pb_rect));
}

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

#ifdef _MSC_VER
#pragma warning(pop)
#endif

#endif

/*
A pixel buffer is an array of pixels.

The pixelformat determines the layout of the
individual elements.

data - pointer to the beginning of the data
pformat - some indication of the pixel layout
width - number of pixels wide
height - number of pixels high
pixelpitch - number of pixels between rows
*/

typedef struct _pb_rgba {
	uint8_t *		data;
	unsigned int		pixelpitch;
	int					owndata;
	pb_rect				frame;
} pb_rgba;


#ifdef __cplusplus
extern "C" {
#endif

	int pb_rgba_init(pb_rgba *pb, const unsigned int width, const unsigned int height);
	int pb_rgba_free(pb_rgba *pb);

	int pb_rgba_get_frame(pb_rgba *pb, const unsigned int x, const unsigned int y, const unsigned int width, const unsigned int height, pb_rgba *pf);

	void pb_rgba_cover_pixel(pb_rgba *pb, const unsigned int x, const unsigned int y, const uint32_t value);

#ifdef __cplusplus
}
#endif

#define pb_rgba_get_pixel(pb, x, y, value) *value = ((uint32_t *)(pb)->data)[(y*(pb)->pixelpitch)+x]
#define pb_rgba_set_pixel(pb, x, y, value) ((uint32_t *)(pb)->data)[(y*(pb)->pixelpitch)+x] = value




// Font information
typedef uint8_t cover_type;    //----cover_type
enum cover_scale_e
{
	cover_none = 0,                 //----cover_none 
	cover_shift = 8,                 //----cover_shift
	cover_size = 1 << cover_shift,  //----cover_size 
	cover_mask = cover_size - 1,    //----cover_mask 
	cover_full = cover_mask         //----cover_full 
};

inline bool isBigEndian() {
	int t = 1;
	return (*(char*)&t == 0);
}


typedef struct _font {
	uint8_t	height;			// height in pixels of all characters
	uint8_t baseline;		// baseline offset of character
	uint8_t start_char;		// ordinal of first character in the set
	uint8_t num_chars;		// number of characters in the font

	bool bigendian;			// endianness of current machine
	uint8_t *charbits;		// pointer to where the bits for the chars start

} font_t;

// Rectangle representing the boundary of a glyph
struct glyph_rect
{
	int x1, y1, x2, y2;
	double dx, dy;
};

typedef struct _glyph {
	size_t width;
	size_t byte_width;

	uint8_t *data;
} glyph_t;


#ifdef __cplusplus
extern "C" {
#endif

	void font_t_init(font_t *f, const uint8_t *data);


	void glyph_t_prepare(const font_t *font, const glyph_t *ginfo, struct glyph_rect* r, double x, double y, bool flip);
	void glyph_t_init(const font_t *f, glyph_t *ginfo, const unsigned int glyph);
	size_t font_t_glyph_width(const font_t *f, const unsigned int glyph);
	size_t font_t_str_width(const font_t *f, const char * str);
	//void font_str_box(const font_t *f, const char * str, int *x, int *y, int *swidth, int *sheight);

	void glyph_t_span(const font_t *f, glyph_t *g, unsigned i, cover_type *m_span);
	int scan_glyph(pb_rgba *pb, font_t *font, glyph_t *glyph, const int x, const int y, const int color);
	int scan_str(pb_rgba *pb, font_t *font, const int x, const int y, const char *chars, const int color);

#ifdef __cplusplus
}
#endif

// PBM Handling
#ifdef __cplusplus
extern "C" {
#endif

	int read_PPM(const char *filename, pb_rgba *fb);			// will read P3 or P6
	int write_PPM_binary(const char *filename, pb_rgba *fb);	// write a P6 file
	int write_PPM_ascii(const char *filename, pb_rgba *fb);		// write a P3 file

#ifdef __cplusplus
}
#endif
