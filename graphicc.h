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

#if defined(BUILD_AS_DLL)
  #if defined(GRC_CORE) || defined(GRC_LIB)
    #define GRC_API		__declspec(dllexport)
  #else
    #define GRC_API		__declspec(dllimport)
  #endif
#else
  #define GRC_API		extern
	#ifdef _WIN32
	#pragma comment(lib,"graphicc.lib")
	#endif
#endif



#ifndef graphicc_h
#define graphicc_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#define BGR_DOMINANT 1


typedef float	float32;
typedef double	float64;
typedef float32 real;
typedef float coord;

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

// A decently small value
static const double  EPSILON = 0.0000000001;

inline bool isBigEndian() {int t = 1;return (*(char*)&t == 0);}

inline real DEGREES(const real radians) { return (real)(G_RTOD * radians); }
inline real RADIANS(const real degrees) { return (real)(G_DTOR * degrees); }

// utils
#define min3(a,b,c) __min(__min(a,b),c)
#define max3(a,b,c) __max(__max(a,b),c)

//static inline float MIN2(float a, float b){ return a < b ? a : b; }
//static inline float MAX2(float a, float b){ return a > b ? a : b; }

//static inline float MIN3(float a, float b, float c){ return MIN2(MIN2(a, b), c); }
//static inline float MAX3(float a, float b, float c){ return MAX2(MAX2(a, b), c); }

// map a value (a) from between rlo <= a <= rhi to  shi <= b <= slo
inline double MAP(double a, double rlo, double rhi, double slo, double shi) { return slo + (a - rlo) * (shi - slo) / (rhi - rlo); }
//inline double CLAMP(double a, double rlo, double rhi){ return a < rlo ? rlo : (a>rhi ? rhi : a); }
inline double CLAMP(const double a, const double rlo=0.0, const double rhi=1.0){ return __min(__max(a, rlo), rhi); }

// turn a division by 255 into something 
// much cheaper to calculate
// for values between 0 and 65534
#define div255(num) ((num + (num >> 8)) >> 8)
#define lerp255(bg, fg, a) ((uint8_t)div255((fg*a+bg*(255-a))))

// returns the sign of the value
//  < 0 --> -1
//  > 0 -->  1
// == 0 -->  0
inline int sgn(real val) { return ((0 < val) - (val < 0)); }


#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4201)  // nonstandard extension used : nameless struct/union
#endif

/*
enum pixellayouts {
	rgba
};
*/

#ifdef BGR_DOMINANT
inline uint32_t RGBA(const int r, const int g, const int b, const int a) 
{
	return ((uint32_t)(a << 24 | r << 16 | g << 8 | b));
}

inline uint8_t GET_B(const uint32_t value) {return ((uint32_t)value & 0xff);}
inline uint8_t GET_G(const uint32_t value) {return (((uint32_t)value & 0xff00) >> 8);}
inline uint8_t GET_R(const uint32_t value) {return (((uint32_t)value & 0xff0000) >> 16);}
inline uint8_t GET_A(const uint32_t value) {return (((uint32_t)value & 0xff000000) >> 24);}
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
		uint8_t v_[4];
		uint32_t value;
	};
} pix_rgba;

// pixel buffer rectangle
typedef struct pb_rect_t {
	coord x, y;
	coord width, height;
} pb_rect;

inline bool pb_rect_contains_point(const pb_rect &rct, const coord x, const coord y)
{
	if ((x < rct.x) || (y < rct.y))
		return false;

	if ((x >= rct.x + rct.width) || (y >= rct.y + rct.height))
		return false;

	return true;
}

inline bool pb_rect_contains_rect(const pb_rect &container, const pb_rect &other)
{
	if (!pb_rect_contains_point(container, other.x, other.y))
	{
		return false;
	}

	if (!pb_rect_contains_point(container, other.x + other.width - 1, other.y + other.height - 1))
	{
		return false;
	}

	return true;
}

// return the intersection of rectangles a and b
// if there is no intersection, one or both of width and height
// will be == zero
inline void pb_rect_intersection(pb_rect &c, const pb_rect &a, const pb_rect &b)
{
	coord x = a.x > b.x ? a.x : b.x;
	coord y = a.y > b.y ? a.y : b.y;
	coord right = ((a.x + a.width) < (b.x + b.width)) ? (a.x + a.width) : (b.x + b.width);
	coord bottom = ((a.y + a.height) < (b.y + b.height)) ? (a.y + a.height) : (b.y + b.height);

	coord width = ((right - x) > 0) ? (right - x) : 0;
	coord height = ((bottom - y) > 0) ? (bottom - y) : 0;

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

typedef real real2[2];
typedef real real3[3];
typedef real real4[4];

typedef struct {
	coord x;
	coord y;
	coord z;
} Pt3;




#ifdef _MSC_VER
#pragma warning(pop)
#endif

#endif

/*
A pixel buffer is an array of pixels.

The pixelformat determines the layout of the
individual elements.

data		- pointer to the beginning of the data
bitStride	- number of bits between rows
pixelpitch	- number of pixels between rows
*/

typedef struct _pb_rgba {
	uint8_t *		data;
	unsigned int	pixelpitch;
	int				bitStride;
	int				owndata;
	pb_rect			frame;
} pb_rgba;



#ifdef __cplusplus
extern "C" {
#endif

GRC_API int pb_rgba_init(pb_rgba *pb, const unsigned int width, const unsigned int height);
GRC_API int pb_rgba_free(pb_rgba *pb);

GRC_API int pb_rgba_get_frame(pb_rgba *src, const unsigned int x, const unsigned int y, const unsigned int width, const unsigned int height, pb_rgba *pf);

GRC_API void pb_rgba_cover_pixel(pb_rgba *pb, const unsigned int x, const unsigned int y, const uint32_t value);

#ifdef __cplusplus
}
#endif

inline uint32_t pb_rgba_get_pixel(pb_rgba *pb, const int x, const int y)
{
	return ((uint32_t *)(pb)->data)[(y*(pb)->pixelpitch) + x];
}

inline void pb_rgba_set_pixel(pb_rgba *pb, const int x, const int y, const int32_t value) 
{
	((uint32_t *)(pb)->data)[(y*(pb)->pixelpitch) + x] = value;
}



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
	int scan_glyph(pb_rgba *pb, font_t *font, glyph_t *glyph, const int x, const int y, const uint32_t color);
	int scan_str(pb_rgba *pb, font_t *font, const int x, const int y, const char *chars, const uint32_t color);

#ifdef __cplusplus
}
#endif

// PBM Handling
#ifdef __cplusplus
extern "C" {
#endif

	int PPM_read_binary(const char *filename, pb_rgba *fb);			// will read P6
	int write_PPM_binary(const char *filename, pb_rgba *fb);	// write a P6 file
	int write_PPM_ascii(const char *filename, pb_rgba *fb);		// write a P3 file

#ifdef __cplusplus
}
#endif
