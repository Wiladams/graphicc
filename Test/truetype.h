// Original
// stb_truetype.h - v1.05 - public domain
// authored from 2009-2014 by Sean Barrett / RAD Game Tools

//
// USAGE
//
//   Simple 3D API (don't ship this, but it's fine for tools and quick start)
//           stbtt_BakeFontBitmap()               -- bake a font to a bitmap for use as texture
//           stbtt_GetBakedQuad()                 -- compute quad to draw for a given char
//
//   Improved 3D API (more shippable):
//           #include "stb_rect_pack.h"           -- optional, but you really want it
//           stbtt_PackBegin()
//           stbtt_PackSetOversample()            -- for improved quality on small fonts
//           stbtt_PackFontRanges()
//           stbtt_PackEnd()
//           stbtt_GetPackedQuad()
//
//   "Load" a font file from a memory buffer (you have to keep the buffer loaded)
//           stbtt_InitFont()
//           stbtt_GetFontOffsetForIndex()        -- use for TTC font collections
//
//   Render a unicode codepoint to a bitmap
//           stbtt_GetCodepointBitmap()           -- allocates and returns a bitmap
//           stbtt_MakeCodepointBitmap()          -- renders into bitmap you provide
//           stbtt_GetCodepointBitmapBox()        -- how big the bitmap must be
//
//   Character advance/positioning
//           stbtt_GetCodepointHMetrics()
//           stbtt_GetFontVMetrics()
//           stbtt_GetCodepointKernAdvance()
//


#ifndef __STB_INCLUDE_STB_TRUETYPE_H__
#define __STB_INCLUDE_STB_TRUETYPE_H__


#include "graphicc.h"


#ifdef STBTT_STATIC
#define STBTT_DEF static
#else
#define STBTT_DEF extern
#endif

// #define your own STBTT_sort() to override this to avoid qsort
#ifndef STBTT_sort
#include <stdlib.h>
#define STBTT_sort(data,num_items,item_size,compare_func)   qsort(data,num_items,item_size,compare_func)
#endif

// #define your own STBTT_ifloor/STBTT_iceil() to avoid math.h
#ifndef STBTT_ifloor
#include <math.h>
#define STBTT_ifloor(x)   ((int) floor(x))
#define STBTT_iceil(x)    ((int) ceil(x))
#endif

#ifndef STBTT_sqrt
#include <math.h>
#define STBTT_sqrt(x)      sqrt(x)
#endif

// #define your own functions "STBTT_malloc" / "STBTT_free" to avoid malloc.h
#ifndef STBTT_malloc
#include <stdlib.h>
#define STBTT_malloc(x,u)  ((void)(u),malloc(x))
#define STBTT_free(x,u)    ((void)(u),free(x))
#endif

#ifndef STBTT_assert
#include <assert.h>
#define STBTT_assert(x)    assert(x)
#endif

#ifndef STBTT_strlen
#include <string.h>
#define STBTT_strlen(x)    strlen(x)
#endif

#ifndef STBTT_memcpy
#include <memory.h>
#define STBTT_memcpy       memcpy
#define STBTT_memset       memset
#endif

// #define your own (u)stbtt_int8/16/32 before including to override this
#ifndef stbtt_uint8
typedef unsigned char   stbtt_uint8;
typedef signed   char   stbtt_int8;
typedef unsigned short  stbtt_uint16;
typedef signed   short  stbtt_int16;
typedef unsigned int    stbtt_uint32;
typedef signed   int    stbtt_int32;
#endif

typedef char stbtt__check_size32[sizeof(stbtt_int32) == 4 ? 1 : -1];
typedef char stbtt__check_size16[sizeof(stbtt_int16) == 2 ? 1 : -1];

//////////////////////////////////////////////////////////////////////////
//
// accessors to parse data from file
//

// on platforms that don't allow misaligned reads, if we want to allow
// truetype fonts that aren't padded to alignment, define ALLOW_UNALIGNED_TRUETYPE

#define ttBYTE(p)     (* (stbtt_uint8 *) (p))
#define ttCHAR(p)     (* (stbtt_int8 *) (p))
#define ttFixed(p)    ttLONG(p)

#if defined(STB_TRUETYPE_BIGENDIAN) && !defined(ALLOW_UNALIGNED_TRUETYPE)

#define ttUSHORT(p)   (* (stbtt_uint16 *) (p))
#define ttSHORT(p)    (* (stbtt_int16 *) (p))
#define ttULONG(p)    (* (stbtt_uint32 *) (p))
#define ttLONG(p)     (* (stbtt_int32 *) (p))

#else

static stbtt_uint16 ttUSHORT(const stbtt_uint8 *p) { return p[0] * 256 + p[1]; }
static stbtt_int16 ttSHORT(const stbtt_uint8 *p)   { return p[0] * 256 + p[1]; }
static stbtt_uint32 ttULONG(const stbtt_uint8 *p)  { return (p[0] << 24) + (p[1] << 16) + (p[2] << 8) + p[3]; }
static stbtt_int32 ttLONG(const stbtt_uint8 *p)    { return (p[0] << 24) + (p[1] << 16) + (p[2] << 8) + p[3]; }

#endif

#define stbtt_tag4(p,c0,c1,c2,c3) ((p)[0] == (c0) && (p)[1] == (c1) && (p)[2] == (c2) && (p)[3] == (c3))
#define stbtt_tag(p,str)           stbtt_tag4(p,str[0],str[1],str[2],str[3])

#ifndef STBTT_MAX_OVERSAMPLE
#define STBTT_MAX_OVERSAMPLE   8
#endif


#ifdef __cplusplus
extern "C" {
#endif





	//////////////////////////////////////////////////////////////////////////////
	//
	// FONT LOADING
	//
	//
	struct ttfcollection {
		char *filename;
		uint8_t *data;
		size_t dataSize;
	};

	STBTT_DEF bool ttfcollection_open(const char *filename, struct ttfcollection &fset);
	STBTT_DEF int stbtt__isfont(const stbtt_uint8 *font);
	STBTT_DEF stbtt_uint32 stbtt__find_table(stbtt_uint8 *data, stbtt_uint32 fontstart, const char *tag);

	STBTT_DEF int stbtt_GetFontOffsetForIndex(const unsigned char *data, int index);
	// Each .ttf/.ttc file may have more than one font. Each font has a sequential
	// index number starting from 0. Call this function to get the font offset for
	// a given index; it returns -1 if the index is out of range. A regular .ttf
	// file will only define one font and it always be at offset 0, so it will
	// return '0' for index 0, and -1 for all other indices. You can just skip
	// this step if you know it's that kind of font.


	// The following structure is defined publically so you can declare one on
	// the stack or as a global or etc, but you should treat it as opaque.
	typedef struct stbtt_fontinfo
	{
		void           * userdata;
		unsigned char  * data;              // pointer to .ttf file
		int              fontstart;         // offset of start of font

		int numGlyphs;                     // number of glyphs, needed for range checking

		// table locations as offset from start of .ttf
		int loca, head, glyf, hhea, hmtx, kern; 
		int index_map;                     // a cmap mapping for our chosen character encoding
		int indexToLocFormat;              // format needed to map from glyph index to glyph
	} stbtt_fontinfo;

	STBTT_DEF int stbtt_InitFont(stbtt_fontinfo *info, const unsigned char *data, int offset);
	// Given an offset into the file that defines a font, this function builds
	// the necessary cached info for the rest of the system. You must allocate
	// the stbtt_fontinfo yourself, and stbtt_InitFont will fill it out. You don't
	// need to do anything special to free it, because the contents are pure
	// value data with no additional data structures. Returns 0 on failure.


	//////////////////////////////////////////////////////////////////////////////
	//
	// CHARACTER TO GLYPH-INDEX CONVERSIOn

	STBTT_DEF int stbtt_FindGlyphIndex(const stbtt_fontinfo *info, int unicode_codepoint);
	// If you're going to perform multiple operations on the same character
	// and you want a speed-up, call this function with the character you're
	// going to process, then use glyph-based functions instead of the
	// codepoint-based functions.


	//////////////////////////////////////////////////////////////////////////////
	//
	// CHARACTER PROPERTIES
	//

	STBTT_DEF float stbtt_ScaleForPixelHeight(const stbtt_fontinfo *info, float pixels);
	// computes a scale factor to produce a font whose "height" is 'pixels' tall.
	// Height is measured as the distance from the highest ascender to the lowest
	// descender; in other words, it's equivalent to calling stbtt_GetFontVMetrics
	// and computing:
	//       scale = pixels / (ascent - descent)
	// so if you prefer to measure height by the ascent only, use a similar calculation.

	STBTT_DEF float stbtt_ScaleForMappingEmToPixels(const stbtt_fontinfo *info, float pixels);
	// computes a scale factor to produce a font whose EM size is mapped to
	// 'pixels' tall. This is probably what traditional APIs compute, but
	// I'm not positive.

	STBTT_DEF void stbtt_GetFontVMetrics(const stbtt_fontinfo *info, int *ascent, int *descent, int *lineGap);
	// ascent is the coordinate above the baseline the font extends; descent
	// is the coordinate below the baseline the font extends (i.e. it is typically negative)
	// lineGap is the spacing between one row's descent and the next row's ascent...
	// so you should advance the vertical position by "*ascent - *descent + *lineGap"
	//   these are expressed in unscaled coordinates, so you must multiply by
	//   the scale factor for a given size

	STBTT_DEF void stbtt_GetFontBoundingBox(const stbtt_fontinfo *info, int *x0, int *y0, int *x1, int *y1);
	// the bounding box around all possible characters

	STBTT_DEF void stbtt_GetCodepointHMetrics(const stbtt_fontinfo *info, int codepoint, int *advanceWidth, int *leftSideBearing);
	// leftSideBearing is the offset from the current horizontal position to the left edge of the character
	// advanceWidth is the offset from the current horizontal position to the next horizontal position
	//   these are expressed in unscaled coordinates

	STBTT_DEF int  stbtt_GetCodepointKernAdvance(const stbtt_fontinfo *info, int ch1, int ch2);
	// an additional amount to add to the 'advance' value between ch1 and ch2

	STBTT_DEF int stbtt_GetCodepointBox(const stbtt_fontinfo *info, int codepoint, int *x0, int *y0, int *x1, int *y1);
	// Gets the bounding box of the visible part of the glyph, in unscaled coordinates

	STBTT_DEF void stbtt_GetGlyphHMetrics(const stbtt_fontinfo *info, int glyph_index, int *advanceWidth, int *leftSideBearing);
	STBTT_DEF int  stbtt_GetGlyphKernAdvance(const stbtt_fontinfo *info, int glyph1, int glyph2);
	STBTT_DEF int  stbtt_GetGlyphBox(const stbtt_fontinfo *info, int glyph_index, int *x0, int *y0, int *x1, int *y1);
	// as above, but takes one or more glyph indices for greater efficiency


	//////////////////////////////////////////////////////////////////////////////
	//
	// GLYPH SHAPES (you probably don't need these, but they have to go before
	// the bitmaps for C declaration-order reasons)
	//

#ifndef STBTT_vmove // you can predefine these to use different values (but why?)
	enum {
		STBTT_vmove = 1,
		STBTT_vline,
		STBTT_vcurve
	};
#endif

typedef struct
{
		float x, y;
} stbtt__point;

#ifndef stbtt_vertex // you can predefine this to use different values
	// (we share this with other code at RAD)
#define stbtt_vertex_type short // can't use stbtt_int16 because that's not visible in the header file
	typedef struct
	{
		stbtt_vertex_type x, y, cx, cy;
		unsigned char type, padding;
	} stbtt_vertex;
#endif

	STBTT_DEF int stbtt_IsGlyphEmpty(const stbtt_fontinfo *info, int glyph_index);
	// returns non-zero if nothing is drawn for this glyph

	STBTT_DEF stbtt__point *stbtt_FlattenCurves(stbtt_vertex *vertices, int num_verts, float objspace_flatness, int **contour_lengths, int *num_contours, void *userdata);
	STBTT_DEF int stbtt_GetCodepointShape(const stbtt_fontinfo *info, int unicode_codepoint, stbtt_vertex **vertices);
	STBTT_DEF int stbtt_GetGlyphShape(const stbtt_fontinfo *info, int glyph_index, stbtt_vertex **vertices);
	// returns # of vertices and fills *vertices with the pointer to them
	//   these are expressed in "unscaled" coordinates
	//
	// The shape is a series of countours. Each one starts with
	// a STBTT_moveto, then consists of a series of mixed
	// STBTT_lineto and STBTT_curveto segments. A lineto
	// draws a line from previous endpoint to its x,y; a curveto
	// draws a quadratic bezier from previous endpoint to
	// its x,y, using cx,cy as the bezier control point.

	STBTT_DEF void stbtt_FreeShape(const stbtt_fontinfo *info, stbtt_vertex *vertices);
	// frees the data allocated above

	//////////////////////////////////////////////////////////////////////////////
	//
	// BITMAP RENDERING
	//

	STBTT_DEF void stbtt_FreeBitmap(unsigned char *bitmap, void *userdata);
	// frees the bitmap allocated below

	STBTT_DEF unsigned char *stbtt_GetCodepointBitmap(const stbtt_fontinfo *info, float scale_x, float scale_y, int codepoint, int *width, int *height, int *xoff, int *yoff);
	// allocates a large-enough single-channel 8bpp bitmap and renders the
	// specified character/glyph at the specified scale into it, with
	// antialiasing. 0 is no coverage (transparent), 255 is fully covered (opaque).
	// *width & *height are filled out with the width & height of the bitmap,
	// which is stored left-to-right, top-to-bottom.
	//
	// xoff/yoff are the offset it pixel space from the glyph origin to the top-left of the bitmap

	STBTT_DEF unsigned char *stbtt_GetCodepointBitmapSubpixel(const stbtt_fontinfo *info, float scale_x, float scale_y, float shift_x, float shift_y, int codepoint, int *width, int *height, int *xoff, int *yoff);
	// the same as stbtt_GetCodepoitnBitmap, but you can specify a subpixel
	// shift for the character

	STBTT_DEF void stbtt_MakeCodepointBitmap(const stbtt_fontinfo *info, unsigned char *output, int out_w, int out_h, int out_stride, float scale_x, float scale_y, int codepoint);
	// the same as stbtt_GetCodepointBitmap, but you pass in storage for the bitmap
	// in the form of 'output', with row spacing of 'out_stride' bytes. the bitmap
	// is clipped to out_w/out_h bytes. Call stbtt_GetCodepointBitmapBox to get the
	// width and height and positioning info for it first.

	STBTT_DEF void stbtt_MakeCodepointBitmapSubpixel(const stbtt_fontinfo *info, unsigned char *output, int out_w, int out_h, int out_stride, float scale_x, float scale_y, float shift_x, float shift_y, int codepoint);
	// same as stbtt_MakeCodepointBitmap, but you can specify a subpixel
	// shift for the character

	STBTT_DEF void stbtt_GetCodepointBitmapBox(const stbtt_fontinfo *font, int codepoint, float scale_x, float scale_y, int *ix0, int *iy0, int *ix1, int *iy1);
	// get the bbox of the bitmap centered around the glyph origin; so the
	// bitmap width is ix1-ix0, height is iy1-iy0, and location to place
	// the bitmap top left is (leftSideBearing*scale,iy0).
	// (Note that the bitmap uses y-increases-down, but the shape uses
	// y-increases-up, so CodepointBitmapBox and CodepointBox are inverted.)

	STBTT_DEF void stbtt_GetCodepointBitmapBoxSubpixel(const stbtt_fontinfo *font, int codepoint, float scale_x, float scale_y, float shift_x, float shift_y, int *ix0, int *iy0, int *ix1, int *iy1);
	// same as stbtt_GetCodepointBitmapBox, but you can specify a subpixel
	// shift for the character

	// the following functions are equivalent to the above functions, but operate
	// on glyph indices instead of Unicode codepoints (for efficiency)
	STBTT_DEF unsigned char *stbtt_GetGlyphBitmap(const stbtt_fontinfo *info, float scale_x, float scale_y, int glyph, int *width, int *height, int *xoff, int *yoff);
	STBTT_DEF unsigned char *stbtt_GetGlyphBitmapSubpixel(const stbtt_fontinfo *info, float scale_x, float scale_y, float shift_x, float shift_y, int glyph, int *width, int *height, int *xoff, int *yoff);
	STBTT_DEF void stbtt_MakeGlyphBitmap(const stbtt_fontinfo *info, unsigned char *output, int out_w, int out_h, int out_stride, float scale_x, float scale_y, int glyph);
	STBTT_DEF void stbtt_MakeGlyphBitmapSubpixel(const stbtt_fontinfo *info, unsigned char *output, int out_w, int out_h, int out_stride, float scale_x, float scale_y, float shift_x, float shift_y, int glyph);
	STBTT_DEF void stbtt_GetGlyphBitmapBox(const stbtt_fontinfo *font, int glyph, float scale_x, float scale_y, int *ix0, int *iy0, int *ix1, int *iy1);
	STBTT_DEF void stbtt_GetGlyphBitmapBoxSubpixel(const stbtt_fontinfo *font, int glyph, float scale_x, float scale_y, float shift_x, float shift_y, int *ix0, int *iy0, int *ix1, int *iy1);


	// @TODO: don't expose this structure
	typedef struct
	{
		int w, h, stride;
		unsigned char *pixels;
	} stbtt__bitmap;

	STBTT_DEF void stbtt_Rasterize(stbtt__bitmap *result, float flatness_in_pixels, stbtt_vertex *vertices, int num_verts, float scale_x, float scale_y, float shift_x, float shift_y, int x_off, int y_off, int invert, void *userdata);

	//////////////////////////////////////////////////////////////////////////////
	//
	// Finding the right font...
	//
	// You should really just solve this offline, keep your own tables
	// of what font is what, and don't try to get it out of the .ttf file.
	// That's because getting it out of the .ttf file is really hard, because
	// the names in the file can appear in many possible encodings, in many
	// possible languages, and e.g. if you need a case-insensitive comparison,
	// the details of that depend on the encoding & language in a complex way
	// (actually underspecified in truetype, but also gigantic).
	//
	// But you can use the provided functions in two possible ways:
	//     stbtt_FindMatchingFont() will use *case-sensitive* comparisons on
	//             unicode-encoded names to try to find the font you want;
	//             you can run this before calling stbtt_InitFont()
	//
	//     stbtt_GetFontNameString() lets you get any of the various strings
	//             from the file yourself and do your own comparisons on them.
	//             You have to have called stbtt_InitFont() first.


	STBTT_DEF int stbtt_FindMatchingFont(const unsigned char *fontdata, const char *name, int flags);
	// returns the offset (not index) of the font that matches, or -1 if none
	//   if you use STBTT_MACSTYLE_DONTCARE, use a font name like "Arial Bold".
	//   if you use any other flag, use a font name like "Arial"; this checks
	//     the 'macStyle' header field; i don't know if fonts set this consistently
#define STBTT_MACSTYLE_DONTCARE     0
#define STBTT_MACSTYLE_BOLD         1
#define STBTT_MACSTYLE_ITALIC       2
#define STBTT_MACSTYLE_UNDERSCORE   4
#define STBTT_MACSTYLE_NONE         8   // <= not same as 0, this makes us check the bitfield is 0

	STBTT_DEF int stbtt_CompareUTF8toUTF16_bigendian(const char *s1, int len1, const char *s2, int len2);
	// returns 1/0 whether the first string interpreted as utf8 is identical to
	// the second string interpreted as big-endian utf16... useful for strings from next func

	STBTT_DEF const char *stbtt_GetFontNameString(const stbtt_fontinfo *font, int *length, int platformID, int encodingID, int languageID, int nameID);
	// returns the string (which may be big-endian double byte, e.g. for unicode)
	// and puts the length in bytes in *length.
	//
	// some of the values for the IDs are below; for more see the truetype spec:
	//     http://developer.apple.com/textfonts/TTRefMan/RM06/Chap6name.html
	//     http://www.microsoft.com/typography/otspec/name.htm

	enum { // platformID
		STBTT_PLATFORM_ID_UNICODE = 0,
		STBTT_PLATFORM_ID_MAC = 1,
		STBTT_PLATFORM_ID_ISO = 2,
		STBTT_PLATFORM_ID_MICROSOFT = 3
	};

	enum { // encodingID for STBTT_PLATFORM_ID_UNICODE
		STBTT_UNICODE_EID_UNICODE_1_0 = 0,
		STBTT_UNICODE_EID_UNICODE_1_1 = 1,
		STBTT_UNICODE_EID_ISO_10646 = 2,
		STBTT_UNICODE_EID_UNICODE_2_0_BMP = 3,
		STBTT_UNICODE_EID_UNICODE_2_0_FULL = 4
	};

	enum { // encodingID for STBTT_PLATFORM_ID_MICROSOFT
		STBTT_MS_EID_SYMBOL = 0,
		STBTT_MS_EID_UNICODE_BMP = 1,
		STBTT_MS_EID_SHIFTJIS = 2,
		STBTT_MS_EID_UNICODE_FULL = 10
	};

	enum { // encodingID for STBTT_PLATFORM_ID_MAC; same as Script Manager codes
		STBTT_MAC_EID_ROMAN = 0, STBTT_MAC_EID_ARABIC = 4,
		STBTT_MAC_EID_JAPANESE = 1, STBTT_MAC_EID_HEBREW = 5,
		STBTT_MAC_EID_CHINESE_TRAD = 2, STBTT_MAC_EID_GREEK = 6,
		STBTT_MAC_EID_KOREAN = 3, STBTT_MAC_EID_RUSSIAN = 7
	};

	enum { // languageID for STBTT_PLATFORM_ID_MICROSOFT; same as LCID...
		// problematic because there are e.g. 16 english LCIDs and 16 arabic LCIDs
		STBTT_MS_LANG_ENGLISH = 0x0409, STBTT_MS_LANG_ITALIAN = 0x0410,
		STBTT_MS_LANG_CHINESE = 0x0804, STBTT_MS_LANG_JAPANESE = 0x0411,
		STBTT_MS_LANG_DUTCH = 0x0413, STBTT_MS_LANG_KOREAN = 0x0412,
		STBTT_MS_LANG_FRENCH = 0x040c, STBTT_MS_LANG_RUSSIAN = 0x0419,
		STBTT_MS_LANG_GERMAN = 0x0407, STBTT_MS_LANG_SPANISH = 0x0409,
		STBTT_MS_LANG_HEBREW = 0x040d, STBTT_MS_LANG_SWEDISH = 0x041D
	};

	enum { // languageID for STBTT_PLATFORM_ID_MAC
		STBTT_MAC_LANG_ENGLISH = 0, STBTT_MAC_LANG_JAPANESE = 11,
		STBTT_MAC_LANG_ARABIC = 12, STBTT_MAC_LANG_KOREAN = 23,
		STBTT_MAC_LANG_DUTCH = 4, STBTT_MAC_LANG_RUSSIAN = 32,
		STBTT_MAC_LANG_FRENCH = 1, STBTT_MAC_LANG_SPANISH = 6,
		STBTT_MAC_LANG_GERMAN = 2, STBTT_MAC_LANG_SWEDISH = 5,
		STBTT_MAC_LANG_HEBREW = 10, STBTT_MAC_LANG_CHINESE_SIMPLIFIED = 33,
		STBTT_MAC_LANG_ITALIAN = 3, STBTT_MAC_LANG_CHINESE_TRAD = 19
	};

#ifdef __cplusplus
}
#endif

#endif // __STB_INCLUDE_STB_TRUETYPE_H__





