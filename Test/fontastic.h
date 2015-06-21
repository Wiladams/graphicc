#include "truetype.h"

//////////////////////////////////////////////////////////////////////////////
//
// TEXTURE BAKING API
//
// If you use this API, you only have to call two functions ever.
//

typedef struct
{
	unsigned short x0, y0, x1, y1; // coordinates of bbox in bitmap
	float xoff, yoff, xadvance;
} stbtt_bakedchar;

STBTT_DEF int stbtt_BakeFontBitmap(const unsigned char *data, int offset,  // font location (use offset=0 for plain .ttf)
	float pixel_height,                     // height of font in pixels
	unsigned char *pixels, int pw, int ph,  // bitmap to be filled in
	int first_char, int num_chars,          // characters to bake
	stbtt_bakedchar *chardata);             // you allocate this, it's num_chars long
// if return is positive, the first unused row of the bitmap
// if return is negative, returns the negative of the number of characters that fit
// if return is 0, no characters fit and no rows were used
// This uses a very crappy packing.

typedef struct
{
	float x0, y0, s0, t0; // top-left
	float x1, y1, s1, t1; // bottom-right
} stbtt_aligned_quad;

STBTT_DEF void stbtt_GetBakedQuad(stbtt_bakedchar *chardata, int pw, int ph,  // same data as above
	int char_index,             // character to display
	float *xpos, float *ypos,   // pointers to current position in screen pixel space
	stbtt_aligned_quad *q,      // output: quad to draw
	int opengl_fillrule);       // true if opengl fill rule; false if DX9 or earlier
// Call GetBakedQuad with char_index = 'character - first_char', and it
// creates the quad you need to draw and advances the current position.
//
// The coordinate system used assumes y increases downwards.
//
// Characters will extend both above and below the current position;
// see discussion of "BASELINE" above.
//
// It's inefficient; you might want to c&p it and optimize it.



//////////////////////////////////////////////////////////////////////////////
//
// NEW TEXTURE BAKING API
//
// This provides options for packing multiple fonts into one atlas, not
// perfectly but better than nothing.

typedef struct
{
	unsigned short x0, y0, x1, y1; // coordinates of bbox in bitmap
	float xoff, yoff, xadvance;
	float xoff2, yoff2;
} stbtt_packedchar;

typedef struct stbtt_pack_context stbtt_pack_context;

STBTT_DEF int  stbtt_PackBegin(stbtt_pack_context *spc, unsigned char *pixels, int width, int height, int stride_in_bytes, int padding, void *alloc_context);
// Initializes a packing context stored in the passed-in stbtt_pack_context.
// Future calls using this context will pack characters into the bitmap passed
// in here: a 1-channel bitmap that is weight x height. stride_in_bytes is
// the distance from one row to the next (or 0 to mean they are packed tightly
// together). "padding" is // the amount of padding to leave between each
// character (normally you want '1' for bitmaps you'll use as textures with
// bilinear filtering).
//
// Returns 0 on failure, 1 on success.

STBTT_DEF void stbtt_PackEnd(stbtt_pack_context *spc);
// Cleans up the packing context and frees all memory.

#define STBTT_POINT_SIZE(x)   (-(x))

STBTT_DEF int  stbtt_PackFontRange(stbtt_pack_context *spc, unsigned char *fontdata, int font_index, float font_size,
	int first_unicode_char_in_range, int num_chars_in_range, stbtt_packedchar *chardata_for_range);
// Creates character bitmaps from the font_index'th font found in fontdata (use
// font_index=0 if you don't know what that is). It creates num_chars_in_range
// bitmaps for characters with unicode values starting at first_unicode_char_in_range
// and increasing. Data for how to render them is stored in chardata_for_range;
// pass these to stbtt_GetPackedQuad to get back renderable quads.
//
// font_size is the full height of the character from ascender to descender,
// as computed by stbtt_ScaleForPixelHeight. To use a point size as computed
// by stbtt_ScaleForMappingEmToPixels, wrap the point size in STBTT_POINT_SIZE()
// and pass that result as 'font_size':
//       ...,                  20 , ... // font max minus min y is 20 pixels tall
//       ..., STBTT_POINT_SIZE(20), ... // 'M' is 20 pixels tall

typedef struct
{
	float font_size;
	int first_unicode_char_in_range;
	int num_chars_in_range;
	stbtt_packedchar *chardata_for_range; // output
} stbtt_pack_range;

STBTT_DEF int  stbtt_PackFontRanges(stbtt_pack_context *spc, unsigned char *fontdata, int font_index, stbtt_pack_range *ranges, int num_ranges);
// Creates character bitmaps from multiple ranges of characters stored in
// ranges. This will usually create a better-packed bitmap than multiple
// calls to stbtt_PackFontRange.


STBTT_DEF void stbtt_PackSetOversampling(stbtt_pack_context *spc, unsigned int h_oversample, unsigned int v_oversample);
// Oversampling a font increases the quality by allowing higher-quality subpixel
// positioning, and is especially valuable at smaller text sizes.
//
// This function sets the amount of oversampling for all following calls to
// stbtt_PackFontRange(s). The default (no oversampling) is achieved by
// h_oversample=1, v_oversample=1. The total number of pixels required is
// h_oversample*v_oversample larger than the default; for example, 2x2
// oversampling requires 4x the storage of 1x1. For best results, render
// oversampled textures with bilinear filtering. Look at the readme in
// stb/tests/oversample for information about oversampled fonts

STBTT_DEF void stbtt_GetPackedQuad(stbtt_packedchar *chardata, int pw, int ph,  // same data as above
	int char_index,             // character to display
	float *xpos, float *ypos,   // pointers to current position in screen pixel space
	stbtt_aligned_quad *q,      // output: quad to draw
	int align_to_integer);

// this is an opaque structure that you shouldn't mess with which holds
// all the context needed from PackBegin to PackEnd.
struct stbtt_pack_context {
	void *user_allocator_context;
	void *pack_info;
	int   width;
	int   height;
	int   stride_in_bytes;
	int   padding;
	unsigned int   h_oversample, v_oversample;
	unsigned char *pixels;
	void  *nodes;
};
