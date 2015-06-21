
#include "fontastic.h"

//////////////////////////////////////////////////////////////////////////////
//
// bitmap baking
//
// This is SUPER-CRAPPY packing to keep source code small

STBTT_DEF int stbtt_BakeFontBitmap(const unsigned char *data, int offset,  // font location (use offset=0 for plain .ttf)
	float pixel_height,                     // height of font in pixels
	unsigned char *pixels, int pw, int ph,  // bitmap to be filled in
	int first_char, int num_chars,          // characters to bake
	stbtt_bakedchar *chardata)
{
	float scale;
	int x, y, bottom_y, i;
	stbtt_fontinfo f;
	if (!stbtt_InitFont(&f, data, offset))
		return -1;
	STBTT_memset(pixels, 0, pw*ph); // background of 0 around pixels
	x = y = 1;
	bottom_y = 1;

	scale = stbtt_ScaleForPixelHeight(&f, pixel_height);

	for (i = 0; i < num_chars; ++i) {
		int advance, lsb, x0, y0, x1, y1, gw, gh;
		int g = stbtt_FindGlyphIndex(&f, first_char + i);
		stbtt_GetGlyphHMetrics(&f, g, &advance, &lsb);
		stbtt_GetGlyphBitmapBox(&f, g, scale, scale, &x0, &y0, &x1, &y1);
		gw = x1 - x0;
		gh = y1 - y0;
		if (x + gw + 1 >= pw)
			y = bottom_y, x = 1; // advance to next row
		if (y + gh + 1 >= ph) // check if it fits vertically AFTER potentially moving to next row
			return -i;
		STBTT_assert(x + gw < pw);
		STBTT_assert(y + gh < ph);
		stbtt_MakeGlyphBitmap(&f, pixels + x + y*pw, gw, gh, pw, scale, scale, g);
		chardata[i].x0 = (stbtt_int16)x;
		chardata[i].y0 = (stbtt_int16)y;
		chardata[i].x1 = (stbtt_int16)(x + gw);
		chardata[i].y1 = (stbtt_int16)(y + gh);
		chardata[i].xadvance = scale * advance;
		chardata[i].xoff = (float)x0;
		chardata[i].yoff = (float)y0;
		x = x + gw + 1;
		if (y + gh + 1 > bottom_y)
			bottom_y = y + gh + 1;
	}
	return bottom_y;
}

STBTT_DEF void stbtt_GetBakedQuad(stbtt_bakedchar *chardata, int pw, int ph, int char_index, float *xpos, float *ypos, stbtt_aligned_quad *q, int opengl_fillrule)
{
	float d3d_bias = opengl_fillrule ? 0 : -0.5f;
	float ipw = 1.0f / pw, iph = 1.0f / ph;
	stbtt_bakedchar *b = chardata + char_index;
	int round_x = STBTT_ifloor((*xpos + b->xoff) + 0.5f);
	int round_y = STBTT_ifloor((*ypos + b->yoff) + 0.5f);

	q->x0 = round_x + d3d_bias;
	q->y0 = round_y + d3d_bias;
	q->x1 = round_x + b->x1 - b->x0 + d3d_bias;
	q->y1 = round_y + b->y1 - b->y0 + d3d_bias;

	q->s0 = b->x0 * ipw;
	q->t0 = b->y0 * iph;
	q->s1 = b->x1 * ipw;
	q->t1 = b->y1 * iph;

	*xpos += b->xadvance;
}

//////////////////////////////////////////////////////////////////////////////
//
// rectangle packing replacement routines if you don't have stb_rect_pack.h
//

#ifndef STB_RECT_PACK_VERSION
#ifdef _MSC_VER
#define STBTT__NOTUSED(v)  (void)(v)
#else
#define STBTT__NOTUSED(v)  (void)sizeof(v)
#endif

typedef int stbrp_coord;

////////////////////////////////////////////////////////////////////////////////////
//                                                                                //
//                                                                                //
// COMPILER WARNING ?!?!?                                                         //
//                                                                                //
//                                                                                //
// if you get a compile warning due to these symbols being defined more than      //
// once, move #include "stb_rect_pack.h" before #include "stb_truetype.h"         //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

typedef struct
{
	int width, height;
	int x, y, bottom_y;
} stbrp_context;

typedef struct
{
	unsigned char x;
} stbrp_node;

typedef struct
{
	stbrp_coord x, y;
	int id, w, h, was_packed;
} stbrp_rect;

static void stbrp_init_target(stbrp_context *con, int pw, int ph, stbrp_node *nodes, int num_nodes)
{
	con->width = pw;
	con->height = ph;
	con->x = 0;
	con->y = 0;
	con->bottom_y = 0;
	STBTT__NOTUSED(nodes);
	STBTT__NOTUSED(num_nodes);
}

static void stbrp_pack_rects(stbrp_context *con, stbrp_rect *rects, int num_rects)
{
	int i;
	for (i = 0; i < num_rects; ++i) {
		if (con->x + rects[i].w > con->width) {
			con->x = 0;
			con->y = con->bottom_y;
		}
		if (con->y + rects[i].h > con->height)
			break;
		rects[i].x = con->x;
		rects[i].y = con->y;
		rects[i].was_packed = 1;
		con->x += rects[i].w;
		if (con->y + rects[i].h > con->bottom_y)
			con->bottom_y = con->y + rects[i].h;
	}
	for (; i < num_rects; ++i)
		rects[i].was_packed = 0;
}
#endif

//////////////////////////////////////////////////////////////////////////////
//
// bitmap baking
//
// This is SUPER-AWESOME (tm Ryan Gordon) packing using stb_rect_pack.h. If
// stb_rect_pack.h isn't available, it uses the BakeFontBitmap strategy.

STBTT_DEF int stbtt_PackBegin(stbtt_pack_context *spc, unsigned char *pixels, int pw, int ph, int stride_in_bytes, int padding, void *alloc_context)
{
	stbrp_context *context = (stbrp_context *)STBTT_malloc(sizeof(*context), alloc_context);
	int            num_nodes = pw - padding;
	stbrp_node    *nodes = (stbrp_node    *)STBTT_malloc(sizeof(*nodes) * num_nodes, alloc_context);

	if (context == NULL || nodes == NULL) {
		if (context != NULL) STBTT_free(context, alloc_context);
		if (nodes != NULL) STBTT_free(nodes, alloc_context);
		return 0;
	}

	spc->user_allocator_context = alloc_context;
	spc->width = pw;
	spc->height = ph;
	spc->pixels = pixels;
	spc->pack_info = context;
	spc->nodes = nodes;
	spc->padding = padding;
	spc->stride_in_bytes = stride_in_bytes != 0 ? stride_in_bytes : pw;
	spc->h_oversample = 1;
	spc->v_oversample = 1;

	stbrp_init_target(context, pw - padding, ph - padding, nodes, num_nodes);

	STBTT_memset(pixels, 0, pw*ph); // background of 0 around pixels

	return 1;
}

STBTT_DEF void stbtt_PackEnd(stbtt_pack_context *spc)
{
	STBTT_free(spc->nodes, spc->user_allocator_context);
	STBTT_free(spc->pack_info, spc->user_allocator_context);
}

STBTT_DEF void stbtt_PackSetOversampling(stbtt_pack_context *spc, unsigned int h_oversample, unsigned int v_oversample)
{
	STBTT_assert(h_oversample <= STBTT_MAX_OVERSAMPLE);
	STBTT_assert(v_oversample <= STBTT_MAX_OVERSAMPLE);
	if (h_oversample <= STBTT_MAX_OVERSAMPLE)
		spc->h_oversample = h_oversample;
	if (v_oversample <= STBTT_MAX_OVERSAMPLE)
		spc->v_oversample = v_oversample;
}

#define STBTT__OVER_MASK  (STBTT_MAX_OVERSAMPLE-1)

static void stbtt__h_prefilter(unsigned char *pixels, int w, int h, int stride_in_bytes, unsigned int kernel_width)
{
	unsigned char buffer[STBTT_MAX_OVERSAMPLE];
	int safe_w = w - kernel_width;
	int j;
	for (j = 0; j < h; ++j) {
		int i;
		unsigned int total;
		STBTT_memset(buffer, 0, kernel_width);

		total = 0;

		// make kernel_width a constant in common cases so compiler can optimize out the divide
		switch (kernel_width) {
		case 2:
			for (i = 0; i <= safe_w; ++i) {
				total += pixels[i] - buffer[i & STBTT__OVER_MASK];
				buffer[(i + kernel_width) & STBTT__OVER_MASK] = pixels[i];
				pixels[i] = (unsigned char)(total / 2);
			}
			break;
		case 3:
			for (i = 0; i <= safe_w; ++i) {
				total += pixels[i] - buffer[i & STBTT__OVER_MASK];
				buffer[(i + kernel_width) & STBTT__OVER_MASK] = pixels[i];
				pixels[i] = (unsigned char)(total / 3);
			}
			break;
		case 4:
			for (i = 0; i <= safe_w; ++i) {
				total += pixels[i] - buffer[i & STBTT__OVER_MASK];
				buffer[(i + kernel_width) & STBTT__OVER_MASK] = pixels[i];
				pixels[i] = (unsigned char)(total / 4);
			}
			break;
		default:
			for (i = 0; i <= safe_w; ++i) {
				total += pixels[i] - buffer[i & STBTT__OVER_MASK];
				buffer[(i + kernel_width) & STBTT__OVER_MASK] = pixels[i];
				pixels[i] = (unsigned char)(total / kernel_width);
			}
			break;
		}

		for (; i < w; ++i) {
			STBTT_assert(pixels[i] == 0);
			total -= buffer[i & STBTT__OVER_MASK];
			pixels[i] = (unsigned char)(total / kernel_width);
		}

		pixels += stride_in_bytes;
	}
}

static void stbtt__v_prefilter(unsigned char *pixels, int w, int h, int stride_in_bytes, unsigned int kernel_width)
{
	unsigned char buffer[STBTT_MAX_OVERSAMPLE];
	int safe_h = h - kernel_width;
	int j;
	for (j = 0; j < w; ++j) {
		int i;
		unsigned int total;
		STBTT_memset(buffer, 0, kernel_width);

		total = 0;

		// make kernel_width a constant in common cases so compiler can optimize out the divide
		switch (kernel_width) {
		case 2:
			for (i = 0; i <= safe_h; ++i) {
				total += pixels[i*stride_in_bytes] - buffer[i & STBTT__OVER_MASK];
				buffer[(i + kernel_width) & STBTT__OVER_MASK] = pixels[i*stride_in_bytes];
				pixels[i*stride_in_bytes] = (unsigned char)(total / 2);
			}
			break;
		case 3:
			for (i = 0; i <= safe_h; ++i) {
				total += pixels[i*stride_in_bytes] - buffer[i & STBTT__OVER_MASK];
				buffer[(i + kernel_width) & STBTT__OVER_MASK] = pixels[i*stride_in_bytes];
				pixels[i*stride_in_bytes] = (unsigned char)(total / 3);
			}
			break;
		case 4:
			for (i = 0; i <= safe_h; ++i) {
				total += pixels[i*stride_in_bytes] - buffer[i & STBTT__OVER_MASK];
				buffer[(i + kernel_width) & STBTT__OVER_MASK] = pixels[i*stride_in_bytes];
				pixels[i*stride_in_bytes] = (unsigned char)(total / 4);
			}
			break;
		default:
			for (i = 0; i <= safe_h; ++i) {
				total += pixels[i*stride_in_bytes] - buffer[i & STBTT__OVER_MASK];
				buffer[(i + kernel_width) & STBTT__OVER_MASK] = pixels[i*stride_in_bytes];
				pixels[i*stride_in_bytes] = (unsigned char)(total / kernel_width);
			}
			break;
		}

		for (; i < h; ++i) {
			STBTT_assert(pixels[i*stride_in_bytes] == 0);
			total -= buffer[i & STBTT__OVER_MASK];
			pixels[i*stride_in_bytes] = (unsigned char)(total / kernel_width);
		}

		pixels += 1;
	}
}

static float stbtt__oversample_shift(int oversample)
{
	if (!oversample)
		return 0.0f;

	// The prefilter is a box filter of width "oversample",
	// which shifts phase by (oversample - 1)/2 pixels in
	// oversampled space. We want to shift in the opposite
	// direction to counter this.
	return (float)-(oversample - 1) / (2.0f * (float)oversample);
}

STBTT_DEF int stbtt_PackFontRanges(stbtt_pack_context *spc, unsigned char *fontdata, int font_index, stbtt_pack_range *ranges, int num_ranges)
{
	stbtt_fontinfo info;
	float recip_h = 1.0f / spc->h_oversample;
	float recip_v = 1.0f / spc->v_oversample;
	float sub_x = stbtt__oversample_shift(spc->h_oversample);
	float sub_y = stbtt__oversample_shift(spc->v_oversample);
	int i, j, k, n, return_value = 1;
	stbrp_context *context = (stbrp_context *)spc->pack_info;
	stbrp_rect    *rects;

	// flag all characters as NOT packed
	for (i = 0; i < num_ranges; ++i)
	for (j = 0; j < ranges[i].num_chars_in_range; ++j)
		ranges[i].chardata_for_range[j].x0 =
		ranges[i].chardata_for_range[j].y0 =
		ranges[i].chardata_for_range[j].x1 =
		ranges[i].chardata_for_range[j].y1 = 0;

	n = 0;
	for (i = 0; i < num_ranges; ++i)
		n += ranges[i].num_chars_in_range;

	rects = (stbrp_rect *)STBTT_malloc(sizeof(*rects) * n, spc->user_allocator_context);
	if (rects == NULL)
		return 0;

	stbtt_InitFont(&info, fontdata, stbtt_GetFontOffsetForIndex(fontdata, font_index));
	k = 0;
	for (i = 0; i < num_ranges; ++i) {
		float fh = ranges[i].font_size;
		float scale = fh > 0 ? stbtt_ScaleForPixelHeight(&info, fh) : stbtt_ScaleForMappingEmToPixels(&info, -fh);
		for (j = 0; j < ranges[i].num_chars_in_range; ++j) {
			int x0, y0, x1, y1;
			stbtt_GetCodepointBitmapBoxSubpixel(&info, ranges[i].first_unicode_char_in_range + j,
				scale * spc->h_oversample,
				scale * spc->v_oversample,
				0, 0,
				&x0, &y0, &x1, &y1);
			rects[k].w = (stbrp_coord)(x1 - x0 + spc->padding + spc->h_oversample - 1);
			rects[k].h = (stbrp_coord)(y1 - y0 + spc->padding + spc->v_oversample - 1);
			++k;
		}
	}

	stbrp_pack_rects(context, rects, k);

	k = 0;
	for (i = 0; i < num_ranges; ++i) {
		float fh = ranges[i].font_size;
		float scale = fh > 0 ? stbtt_ScaleForPixelHeight(&info, fh) : stbtt_ScaleForMappingEmToPixels(&info, -fh);
		for (j = 0; j < ranges[i].num_chars_in_range; ++j) {
			stbrp_rect *r = &rects[k];
			if (r->was_packed) {
				stbtt_packedchar *bc = &ranges[i].chardata_for_range[j];
				int advance, lsb, x0, y0, x1, y1;
				int glyph = stbtt_FindGlyphIndex(&info, ranges[i].first_unicode_char_in_range + j);
				stbrp_coord pad = (stbrp_coord)spc->padding;

				// pad on left and top
				r->x += pad;
				r->y += pad;
				r->w -= pad;
				r->h -= pad;
				stbtt_GetGlyphHMetrics(&info, glyph, &advance, &lsb);
				stbtt_GetGlyphBitmapBox(&info, glyph,
					scale * spc->h_oversample,
					scale * spc->v_oversample,
					&x0, &y0, &x1, &y1);
				stbtt_MakeGlyphBitmapSubpixel(&info,
					spc->pixels + r->x + r->y*spc->stride_in_bytes,
					r->w - spc->h_oversample + 1,
					r->h - spc->v_oversample + 1,
					spc->stride_in_bytes,
					scale * spc->h_oversample,
					scale * spc->v_oversample,
					0, 0,
					glyph);

				if (spc->h_oversample > 1)
					stbtt__h_prefilter(spc->pixels + r->x + r->y*spc->stride_in_bytes,
					r->w, r->h, spc->stride_in_bytes,
					spc->h_oversample);

				if (spc->v_oversample > 1)
					stbtt__v_prefilter(spc->pixels + r->x + r->y*spc->stride_in_bytes,
					r->w, r->h, spc->stride_in_bytes,
					spc->v_oversample);

				bc->x0 = (stbtt_int16)r->x;
				bc->y0 = (stbtt_int16)r->y;
				bc->x1 = (stbtt_int16)(r->x + r->w);
				bc->y1 = (stbtt_int16)(r->y + r->h);
				bc->xadvance = scale * advance;
				bc->xoff = (float)x0 * recip_h + sub_x;
				bc->yoff = (float)y0 * recip_v + sub_y;
				bc->xoff2 = (x0 + r->w) * recip_h + sub_x;
				bc->yoff2 = (y0 + r->h) * recip_v + sub_y;
			}
			else {
				return_value = 0; // if any fail, report failure
			}

			++k;
		}
	}

	STBTT_free(rects, spc->user_allocator_context);
	return return_value;
}

STBTT_DEF int stbtt_PackFontRange(stbtt_pack_context *spc, unsigned char *fontdata, int font_index, float font_size,
	int first_unicode_char_in_range, int num_chars_in_range, stbtt_packedchar *chardata_for_range)
{
	stbtt_pack_range range;
	range.first_unicode_char_in_range = first_unicode_char_in_range;
	range.num_chars_in_range = num_chars_in_range;
	range.chardata_for_range = chardata_for_range;
	range.font_size = font_size;
	return stbtt_PackFontRanges(spc, fontdata, font_index, &range, 1);
}

STBTT_DEF void stbtt_GetPackedQuad(stbtt_packedchar *chardata, int pw, int ph, int char_index, float *xpos, float *ypos, stbtt_aligned_quad *q, int align_to_integer)
{
	float ipw = 1.0f / pw, iph = 1.0f / ph;
	stbtt_packedchar *b = chardata + char_index;

	if (align_to_integer) {
		float x = (float)STBTT_ifloor((*xpos + b->xoff) + 0.5f);
		float y = (float)STBTT_ifloor((*ypos + b->yoff) + 0.5f);
		q->x0 = x;
		q->y0 = y;
		q->x1 = x + b->xoff2 - b->xoff;
		q->y1 = y + b->yoff2 - b->yoff;
	}
	else {
		q->x0 = *xpos + b->xoff;
		q->y0 = *ypos + b->yoff;
		q->x1 = *xpos + b->xoff2;
		q->y1 = *ypos + b->yoff2;
	}

	q->s0 = b->x0 * ipw;
	q->t0 = b->y0 * iph;
	q->s1 = b->x1 * ipw;
	q->t1 = b->y1 * iph;

	*xpos += b->xadvance;
}


//////////////////////////////////////////////////////////////////////////////
//
// font name matching -- recommended not to use this
//

// check if a utf8 string contains a prefix which is the utf16 string; if so return length of matching utf8 string
static stbtt_int32 stbtt__CompareUTF8toUTF16_bigendian_prefix(const stbtt_uint8 *s1, stbtt_int32 len1, const stbtt_uint8 *s2, stbtt_int32 len2)
{
	stbtt_int32 i = 0;

	// convert utf16 to utf8 and compare the results while converting
	while (len2) {
		stbtt_uint16 ch = s2[0] * 256 + s2[1];
		if (ch < 0x80) {
			if (i >= len1) return -1;
			if (s1[i++] != ch) return -1;
		}
		else if (ch < 0x800) {
			if (i + 1 >= len1) return -1;
			if (s1[i++] != 0xc0 + (ch >> 6)) return -1;
			if (s1[i++] != 0x80 + (ch & 0x3f)) return -1;
		}
		else if (ch >= 0xd800 && ch < 0xdc00) {
			stbtt_uint32 c;
			stbtt_uint16 ch2 = s2[2] * 256 + s2[3];
			if (i + 3 >= len1) return -1;
			c = ((ch - 0xd800) << 10) + (ch2 - 0xdc00) + 0x10000;
			if (s1[i++] != 0xf0 + (c >> 18)) return -1;
			if (s1[i++] != 0x80 + ((c >> 12) & 0x3f)) return -1;
			if (s1[i++] != 0x80 + ((c >> 6) & 0x3f)) return -1;
			if (s1[i++] != 0x80 + ((c)& 0x3f)) return -1;
			s2 += 2; // plus another 2 below
			len2 -= 2;
		}
		else if (ch >= 0xdc00 && ch < 0xe000) {
			return -1;
		}
		else {
			if (i + 2 >= len1) return -1;
			if (s1[i++] != 0xe0 + (ch >> 12)) return -1;
			if (s1[i++] != 0x80 + ((ch >> 6) & 0x3f)) return -1;
			if (s1[i++] != 0x80 + ((ch)& 0x3f)) return -1;
		}
		s2 += 2;
		len2 -= 2;
	}
	return i;
}

STBTT_DEF int stbtt_CompareUTF8toUTF16_bigendian(const char *s1, int len1, const char *s2, int len2)
{
	return len1 == stbtt__CompareUTF8toUTF16_bigendian_prefix((const stbtt_uint8*)s1, len1, (const stbtt_uint8*)s2, len2);
}

// returns results in whatever encoding you request... but note that 2-byte encodings
// will be BIG-ENDIAN... use stbtt_CompareUTF8toUTF16_bigendian() to compare
STBTT_DEF const char *stbtt_GetFontNameString(const stbtt_fontinfo *font, int *length, int platformID, int encodingID, int languageID, int nameID)
{
	stbtt_int32 i, count, stringOffset;
	stbtt_uint8 *fc = font->data;
	stbtt_uint32 offset = font->fontstart;
	stbtt_uint32 nm = stbtt__find_table(fc, offset, "name");
	if (!nm) return NULL;

	count = ttUSHORT(fc + nm + 2);
	stringOffset = nm + ttUSHORT(fc + nm + 4);
	for (i = 0; i < count; ++i) {
		stbtt_uint32 loc = nm + 6 + 12 * i;
		if (platformID == ttUSHORT(fc + loc + 0) && encodingID == ttUSHORT(fc + loc + 2)
			&& languageID == ttUSHORT(fc + loc + 4) && nameID == ttUSHORT(fc + loc + 6)) {
			*length = ttUSHORT(fc + loc + 8);
			return (const char *)(fc + stringOffset + ttUSHORT(fc + loc + 10));
		}
	}
	return NULL;
}

static int stbtt__matchpair(stbtt_uint8 *fc, stbtt_uint32 nm, stbtt_uint8 *name, stbtt_int32 nlen, stbtt_int32 target_id, stbtt_int32 next_id)
{
	stbtt_int32 i;
	stbtt_int32 count = ttUSHORT(fc + nm + 2);
	stbtt_int32 stringOffset = nm + ttUSHORT(fc + nm + 4);

	for (i = 0; i < count; ++i) {
		stbtt_uint32 loc = nm + 6 + 12 * i;
		stbtt_int32 id = ttUSHORT(fc + loc + 6);
		if (id == target_id) {
			// find the encoding
			stbtt_int32 platform = ttUSHORT(fc + loc + 0), encoding = ttUSHORT(fc + loc + 2), language = ttUSHORT(fc + loc + 4);

			// is this a Unicode encoding?
			if (platform == 0 || (platform == 3 && encoding == 1) || (platform == 3 && encoding == 10)) {
				stbtt_int32 slen = ttUSHORT(fc + loc + 8);
				stbtt_int32 off = ttUSHORT(fc + loc + 10);

				// check if there's a prefix match
				stbtt_int32 matchlen = stbtt__CompareUTF8toUTF16_bigendian_prefix(name, nlen, fc + stringOffset + off, slen);
				if (matchlen >= 0) {
					// check for target_id+1 immediately following, with same encoding & language
					if (i + 1 < count && ttUSHORT(fc + loc + 12 + 6) == next_id && ttUSHORT(fc + loc + 12) == platform && ttUSHORT(fc + loc + 12 + 2) == encoding && ttUSHORT(fc + loc + 12 + 4) == language) {
						slen = ttUSHORT(fc + loc + 12 + 8);
						off = ttUSHORT(fc + loc + 12 + 10);
						if (slen == 0) {
							if (matchlen == nlen)
								return 1;
						}
						else if (matchlen < nlen && name[matchlen] == ' ') {
							++matchlen;
							if (stbtt_CompareUTF8toUTF16_bigendian((char*)(name + matchlen), nlen - matchlen, (char*)(fc + stringOffset + off), slen))
								return 1;
						}
					}
					else {
						// if nothing immediately following
						if (matchlen == nlen)
							return 1;
					}
				}
			}

			// @TODO handle other encodings
		}
	}
	return 0;
}

static int stbtt__matches(stbtt_uint8 *fc, stbtt_uint32 offset, stbtt_uint8 *name, stbtt_int32 flags)
{
	stbtt_int32 nlen = (stbtt_int32)STBTT_strlen((char *)name);
	stbtt_uint32 nm, hd;
	if (!stbtt__isfont(fc + offset)) return 0;

	// check italics/bold/underline flags in macStyle...
	if (flags) {
		hd = stbtt__find_table(fc, offset, "head");
		if ((ttUSHORT(fc + hd + 44) & 7) != (flags & 7)) return 0;
	}

	nm = stbtt__find_table(fc, offset, "name");
	if (!nm) return 0;

	if (flags) {
		// if we checked the macStyle flags, then just check the family and ignore the subfamily
		if (stbtt__matchpair(fc, nm, name, nlen, 16, -1))  return 1;
		if (stbtt__matchpair(fc, nm, name, nlen, 1, -1))  return 1;
		if (stbtt__matchpair(fc, nm, name, nlen, 3, -1))  return 1;
	}
	else {
		if (stbtt__matchpair(fc, nm, name, nlen, 16, 17))  return 1;
		if (stbtt__matchpair(fc, nm, name, nlen, 1, 2))  return 1;
		if (stbtt__matchpair(fc, nm, name, nlen, 3, -1))  return 1;
	}

	return 0;
}

STBTT_DEF int stbtt_FindMatchingFont(const unsigned char *font_collection, const char *name_utf8, stbtt_int32 flags)
{
	stbtt_int32 i;
	for (i = 0;; ++i) {
		stbtt_int32 off = stbtt_GetFontOffsetForIndex(font_collection, i);
		if (off < 0) return off;
		if (stbtt__matches((stbtt_uint8 *)font_collection, off, (stbtt_uint8*)name_utf8, flags))
			return off;
	}
}

