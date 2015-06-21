// stb_truetype.h - v1.05 - public domain
// authored from 2009-2014 by Sean Barrett / RAD Game Tools

#include "truetype.h"
#include <stdio.h>




typedef int stbtt__test_oversample_pow2[(STBTT_MAX_OVERSAMPLE & (STBTT_MAX_OVERSAMPLE - 1)) == 0 ? 1 : -1];



STBTT_DEF 
int stbtt__isfont(const stbtt_uint8 *font)
{
	// check the version number
	if (stbtt_tag4(font, '1', 0, 0, 0))  return 1; // TrueType 1
	if (stbtt_tag(font, "typ1"))   return 1; // TrueType with type 1 font -- we don't support this!
	if (stbtt_tag(font, "OTTO"))   return 1; // OpenType with CFF
	if (stbtt_tag4(font, 0, 1, 0, 0)) return 1; // OpenType 1.0
	return 0;
}

STBTT_DEF
bool ttfcollection_open(const char *filename, struct ttfcollection &fset)
{
	FILE *f;
	fopen_s(&f, filename, "rb");

	if (f == nullptr) {
		return false;
	}

	// calculate size of file to allocate buffer
	// use fseek, ftell
	fseek(f, 0, SEEK_END);
	int bufferSize = ftell(f);
	fseek(f, 0, SEEK_SET);

	fset.data = (uint8_t *)malloc(bufferSize);
	fset.filename = _strdup(filename);
	fset.dataSize = bufferSize;
	fread(fset.data, 1, bufferSize, f);
	fclose(f);

	return true;
}

// @OPTIMIZE: binary search
STBTT_DEF stbtt_uint32 stbtt__find_table(stbtt_uint8 *data, stbtt_uint32 fontstart, const char *tag)
{
	stbtt_int32 num_tables = ttUSHORT(data + fontstart + 4);
	stbtt_uint32 tabledir = fontstart + 12;
	stbtt_int32 i;
	for (i = 0; i < num_tables; ++i) {
		stbtt_uint32 loc = tabledir + 16 * i;
		if (stbtt_tag(data + loc + 0, tag))
			return ttULONG(data + loc + 8);
	}
	return 0;
}

STBTT_DEF int stbtt_GetFontOffsetForIndex(const unsigned char *font_collection, int index)
{
	// if it's just a font, there's only one valid index
	if (stbtt__isfont(font_collection))
		return index == 0 ? 0 : -1;

	// check if it's a TTC
	if (stbtt_tag(font_collection, "ttcf")) {
		// version 1?
		if (ttULONG(font_collection + 4) == 0x00010000 || ttULONG(font_collection + 4) == 0x00020000) {
			stbtt_int32 n = ttLONG(font_collection + 8);
			if (index >= n)
				return -1;
			return ttULONG(font_collection + 12 + index * 14);
		}
	}
	return -1;
}

STBTT_DEF int stbtt_InitFont(stbtt_fontinfo *info, const unsigned char *data2, int fontstart)
{
	stbtt_uint8 *data = (stbtt_uint8 *)data2;
	stbtt_uint32 cmap, t;
	stbtt_int32 i, numTables;

	info->data = data;
	info->fontstart = fontstart;

	cmap = stbtt__find_table(data, fontstart, "cmap");       // required
	info->loca = stbtt__find_table(data, fontstart, "loca"); // required
	info->head = stbtt__find_table(data, fontstart, "head"); // required
	info->glyf = stbtt__find_table(data, fontstart, "glyf"); // required
	info->hhea = stbtt__find_table(data, fontstart, "hhea"); // required
	info->hmtx = stbtt__find_table(data, fontstart, "hmtx"); // required
	info->kern = stbtt__find_table(data, fontstart, "kern"); // not required
	if (!cmap || !info->loca || !info->head || !info->glyf || !info->hhea || !info->hmtx)
		return 0;

	t = stbtt__find_table(data, fontstart, "maxp");
	if (t)
		info->numGlyphs = ttUSHORT(data + t + 4);
	else
		info->numGlyphs = 0xffff;

	// find a cmap encoding table we understand *now* to avoid searching
	// later. (todo: could make this installable)
	// the same regardless of glyph.
	numTables = ttUSHORT(data + cmap + 2);
	info->index_map = 0;
	for (i = 0; i < numTables; ++i) {
		stbtt_uint32 encoding_record = cmap + 4 + 8 * i;
		// find an encoding we understand:
		switch (ttUSHORT(data + encoding_record)) {
		case STBTT_PLATFORM_ID_MICROSOFT:
			switch (ttUSHORT(data + encoding_record + 2)) {
			case STBTT_MS_EID_UNICODE_BMP:
			case STBTT_MS_EID_UNICODE_FULL:
				// MS/Unicode
				info->index_map = cmap + ttULONG(data + encoding_record + 4);
				break;
			}
			break;
		case STBTT_PLATFORM_ID_UNICODE:
			// Mac/iOS has these
			// all the encodingIDs are unicode, so we don't bother to check it
			info->index_map = cmap + ttULONG(data + encoding_record + 4);
			break;
		}
	}
	if (info->index_map == 0)
		return 0;

	info->indexToLocFormat = ttUSHORT(data + info->head + 50);
	return 1;
}

STBTT_DEF int stbtt_FindGlyphIndex(const stbtt_fontinfo *info, int unicode_codepoint)
{
	stbtt_uint8 *data = info->data;
	stbtt_uint32 index_map = info->index_map;

	stbtt_uint16 format = ttUSHORT(data + index_map + 0);
	if (format == 0) { // apple byte encoding
		stbtt_int32 bytes = ttUSHORT(data + index_map + 2);
		if (unicode_codepoint < bytes - 6)
			return ttBYTE(data + index_map + 6 + unicode_codepoint);
		return 0;
	}
	else if (format == 6) {
		stbtt_uint32 first = ttUSHORT(data + index_map + 6);
		stbtt_uint32 count = ttUSHORT(data + index_map + 8);
		if ((stbtt_uint32)unicode_codepoint >= first && (stbtt_uint32)unicode_codepoint < first + count)
			return ttUSHORT(data + index_map + 10 + (unicode_codepoint - first) * 2);
		return 0;
	}
	else if (format == 2) {
		STBTT_assert(0); // @TODO: high-byte mapping for japanese/chinese/korean
		return 0;
	}
	else if (format == 4) { // standard mapping for windows fonts: binary search collection of ranges
		stbtt_uint16 segcount = ttUSHORT(data + index_map + 6) >> 1;
		stbtt_uint16 searchRange = ttUSHORT(data + index_map + 8) >> 1;
		stbtt_uint16 entrySelector = ttUSHORT(data + index_map + 10);
		stbtt_uint16 rangeShift = ttUSHORT(data + index_map + 12) >> 1;

		// do a binary search of the segments
		stbtt_uint32 endCount = index_map + 14;
		stbtt_uint32 search = endCount;

		if (unicode_codepoint > 0xffff)
			return 0;

		// they lie from endCount .. endCount + segCount
		// but searchRange is the nearest power of two, so...
		if (unicode_codepoint >= ttUSHORT(data + search + rangeShift * 2))
			search += rangeShift * 2;

		// now decrement to bias correctly to find smallest
		search -= 2;
		while (entrySelector) {
			stbtt_uint16 end;
			searchRange >>= 1;
			end = ttUSHORT(data + search + searchRange * 2);
			if (unicode_codepoint > end)
				search += searchRange * 2;
			--entrySelector;
		}
		search += 2;

		{
			stbtt_uint16 offset, start;
			stbtt_uint16 item = (stbtt_uint16)((search - endCount) >> 1);

			STBTT_assert(unicode_codepoint <= ttUSHORT(data + endCount + 2 * item));
			start = ttUSHORT(data + index_map + 14 + segcount * 2 + 2 + 2 * item);
			if (unicode_codepoint < start)
				return 0;

			offset = ttUSHORT(data + index_map + 14 + segcount * 6 + 2 + 2 * item);
			if (offset == 0)
				return (stbtt_uint16)(unicode_codepoint + ttSHORT(data + index_map + 14 + segcount * 4 + 2 + 2 * item));

			return ttUSHORT(data + offset + (unicode_codepoint - start) * 2 + index_map + 14 + segcount * 6 + 2 + 2 * item);
		}
	}
	else if (format == 12 || format == 13) {
		stbtt_uint32 ngroups = ttULONG(data + index_map + 12);
		stbtt_int32 low, high;
		low = 0; high = (stbtt_int32)ngroups;
		// Binary search the right group.
		while (low < high) {
			stbtt_int32 mid = low + ((high - low) >> 1); // rounds down, so low <= mid < high
			stbtt_uint32 start_char = ttULONG(data + index_map + 16 + mid * 12);
			stbtt_uint32 end_char = ttULONG(data + index_map + 16 + mid * 12 + 4);
			if ((stbtt_uint32)unicode_codepoint < start_char)
				high = mid;
			else if ((stbtt_uint32)unicode_codepoint > end_char)
				low = mid + 1;
			else {
				stbtt_uint32 start_glyph = ttULONG(data + index_map + 16 + mid * 12 + 8);
				if (format == 12)
					return start_glyph + unicode_codepoint - start_char;
				else // format == 13
					return start_glyph;
			}
		}
		return 0; // not found
	}
	// @TODO
	STBTT_assert(0);
	return 0;
}

STBTT_DEF int stbtt_GetCodepointShape(const stbtt_fontinfo *info, int unicode_codepoint, stbtt_vertex **vertices)
{
	return stbtt_GetGlyphShape(info, stbtt_FindGlyphIndex(info, unicode_codepoint), vertices);
}

static void stbtt_setvertex(stbtt_vertex *v, stbtt_uint8 type, stbtt_int32 x, stbtt_int32 y, stbtt_int32 cx, stbtt_int32 cy)
{
	v->type = type;
	v->x = (stbtt_int16)x;
	v->y = (stbtt_int16)y;
	v->cx = (stbtt_int16)cx;
	v->cy = (stbtt_int16)cy;
}

static int stbtt__GetGlyfOffset(const stbtt_fontinfo *info, int glyph_index)
{
	int g1, g2;

	if (glyph_index >= info->numGlyphs) return -1; // glyph index out of range
	if (info->indexToLocFormat >= 2)    return -1; // unknown index->glyph map format

	if (info->indexToLocFormat == 0) {
		g1 = info->glyf + ttUSHORT(info->data + info->loca + glyph_index * 2) * 2;
		g2 = info->glyf + ttUSHORT(info->data + info->loca + glyph_index * 2 + 2) * 2;
	}
	else {
		g1 = info->glyf + ttULONG(info->data + info->loca + glyph_index * 4);
		g2 = info->glyf + ttULONG(info->data + info->loca + glyph_index * 4 + 4);
	}

	return g1 == g2 ? -1 : g1; // if length is 0, return -1
}

STBTT_DEF int stbtt_GetGlyphBox(const stbtt_fontinfo *info, int glyph_index, int *x0, int *y0, int *x1, int *y1)
{
	int g = stbtt__GetGlyfOffset(info, glyph_index);
	if (g < 0) return 0;

	if (x0) *x0 = ttSHORT(info->data + g + 2);
	if (y0) *y0 = ttSHORT(info->data + g + 4);
	if (x1) *x1 = ttSHORT(info->data + g + 6);
	if (y1) *y1 = ttSHORT(info->data + g + 8);
	return 1;
}

STBTT_DEF int stbtt_GetCodepointBox(const stbtt_fontinfo *info, int codepoint, int *x0, int *y0, int *x1, int *y1)
{
	return stbtt_GetGlyphBox(info, stbtt_FindGlyphIndex(info, codepoint), x0, y0, x1, y1);
}

STBTT_DEF int stbtt_IsGlyphEmpty(const stbtt_fontinfo *info, int glyph_index)
{
	stbtt_int16 numberOfContours;
	int g = stbtt__GetGlyfOffset(info, glyph_index);
	if (g < 0) return 1;
	numberOfContours = ttSHORT(info->data + g);
	return numberOfContours == 0;
}

static int stbtt__close_shape(stbtt_vertex *vertices, int num_vertices, int was_off, int start_off,
	stbtt_int32 sx, stbtt_int32 sy, stbtt_int32 scx, stbtt_int32 scy, stbtt_int32 cx, stbtt_int32 cy)
{
	if (start_off) {
		if (was_off)
			stbtt_setvertex(&vertices[num_vertices++], STBTT_vcurve, (cx + scx) >> 1, (cy + scy) >> 1, cx, cy);
		stbtt_setvertex(&vertices[num_vertices++], STBTT_vcurve, sx, sy, scx, scy);
	}
	else {
		if (was_off)
			stbtt_setvertex(&vertices[num_vertices++], STBTT_vcurve, sx, sy, cx, cy);
		else
			stbtt_setvertex(&vertices[num_vertices++], STBTT_vline, sx, sy, 0, 0);
	}
	return num_vertices;
}

STBTT_DEF int stbtt_GetGlyphShape(const stbtt_fontinfo *info, int glyph_index, stbtt_vertex **pvertices)
{
	stbtt_int16 numberOfContours;
	stbtt_uint8 *endPtsOfContours;
	stbtt_uint8 *data = info->data;
	stbtt_vertex *vertices = 0;
	int num_vertices = 0;
	int g = stbtt__GetGlyfOffset(info, glyph_index);

	*pvertices = NULL;

	if (g < 0) return 0;

	numberOfContours = ttSHORT(data + g);

	if (numberOfContours > 0) {
		stbtt_uint8 flags = 0, flagcount;
		stbtt_int32 ins, i, j = 0, m, n, next_move, was_off = 0, off, start_off = 0;
		stbtt_int32 x, y, cx, cy, sx, sy, scx, scy;
		stbtt_uint8 *points;
		endPtsOfContours = (data + g + 10);
		ins = ttUSHORT(data + g + 10 + numberOfContours * 2);
		points = data + g + 10 + numberOfContours * 2 + 2 + ins;

		n = 1 + ttUSHORT(endPtsOfContours + numberOfContours * 2 - 2);

		m = n + 2 * numberOfContours;  // a loose bound on how many vertices we might need
		vertices = (stbtt_vertex *)STBTT_malloc(m * sizeof(vertices[0]), info->userdata);
		if (vertices == 0)
			return 0;

		next_move = 0;
		flagcount = 0;

		// in first pass, we load uninterpreted data into the allocated array
		// above, shifted to the end of the array so we won't overwrite it when
		// we create our final data starting from the front

		off = m - n; // starting offset for uninterpreted data, regardless of how m ends up being calculated

		// first load flags

		for (i = 0; i < n; ++i) {
			if (flagcount == 0) {
				flags = *points++;
				if (flags & 8)
					flagcount = *points++;
			}
			else
				--flagcount;
			vertices[off + i].type = flags;
		}

		// now load x coordinates
		x = 0;
		for (i = 0; i < n; ++i) {
			flags = vertices[off + i].type;
			if (flags & 2) {
				stbtt_int16 dx = *points++;
				x += (flags & 16) ? dx : -dx; // ???
			}
			else {
				if (!(flags & 16)) {
					x = x + (stbtt_int16)(points[0] * 256 + points[1]);
					points += 2;
				}
			}
			vertices[off + i].x = (stbtt_int16)x;
		}

		// now load y coordinates
		y = 0;
		for (i = 0; i < n; ++i) {
			flags = vertices[off + i].type;
			if (flags & 4) {
				stbtt_int16 dy = *points++;
				y += (flags & 32) ? dy : -dy; // ???
			}
			else {
				if (!(flags & 32)) {
					y = y + (stbtt_int16)(points[0] * 256 + points[1]);
					points += 2;
				}
			}
			vertices[off + i].y = (stbtt_int16)y;
		}

		// now convert them to our format
		num_vertices = 0;
		sx = sy = cx = cy = scx = scy = 0;
		for (i = 0; i < n; ++i) {
			flags = vertices[off + i].type;
			x = (stbtt_int16)vertices[off + i].x;
			y = (stbtt_int16)vertices[off + i].y;

			if (next_move == i) {
				if (i != 0)
					num_vertices = stbtt__close_shape(vertices, num_vertices, was_off, start_off, sx, sy, scx, scy, cx, cy);

				// now start the new one               
				start_off = !(flags & 1);
				if (start_off) {
					// if we start off with an off-curve point, then when we need to find a point on the curve
					// where we can start, and we need to save some state for when we wraparound.
					scx = x;
					scy = y;
					if (!(vertices[off + i + 1].type & 1)) {
						// next point is also a curve point, so interpolate an on-point curve
						sx = (x + (stbtt_int32)vertices[off + i + 1].x) >> 1;
						sy = (y + (stbtt_int32)vertices[off + i + 1].y) >> 1;
					}
					else {
						// otherwise just use the next point as our start point
						sx = (stbtt_int32)vertices[off + i + 1].x;
						sy = (stbtt_int32)vertices[off + i + 1].y;
						++i; // we're using point i+1 as the starting point, so skip it
					}
				}
				else {
					sx = x;
					sy = y;
				}
				stbtt_setvertex(&vertices[num_vertices++], STBTT_vmove, sx, sy, 0, 0);
				was_off = 0;
				next_move = 1 + ttUSHORT(endPtsOfContours + j * 2);
				++j;
			}
			else {
				if (!(flags & 1)) { // if it's a curve
					if (was_off) // two off-curve control points in a row means interpolate an on-curve midpoint
						stbtt_setvertex(&vertices[num_vertices++], STBTT_vcurve, (cx + x) >> 1, (cy + y) >> 1, cx, cy);
					cx = x;
					cy = y;
					was_off = 1;
				}
				else {
					if (was_off)
						stbtt_setvertex(&vertices[num_vertices++], STBTT_vcurve, x, y, cx, cy);
					else
						stbtt_setvertex(&vertices[num_vertices++], STBTT_vline, x, y, 0, 0);
					was_off = 0;
				}
			}
		}
		num_vertices = stbtt__close_shape(vertices, num_vertices, was_off, start_off, sx, sy, scx, scy, cx, cy);
	}
	else if (numberOfContours == -1) {
		// Compound shapes.
		int more = 1;
		stbtt_uint8 *comp = data + g + 10;
		num_vertices = 0;
		vertices = 0;
		while (more) {
			stbtt_uint16 flags, gidx;
			int comp_num_verts = 0, i;
			stbtt_vertex *comp_verts = 0, *tmp = 0;
			float mtx[6] = { 1, 0, 0, 1, 0, 0 }, m, n;

			flags = ttSHORT(comp); comp += 2;
			gidx = ttSHORT(comp); comp += 2;

			if (flags & 2) { // XY values
				if (flags & 1) { // shorts
					mtx[4] = ttSHORT(comp); comp += 2;
					mtx[5] = ttSHORT(comp); comp += 2;
				}
				else {
					mtx[4] = ttCHAR(comp); comp += 1;
					mtx[5] = ttCHAR(comp); comp += 1;
				}
			}
			else {
				// @TODO handle matching point
				STBTT_assert(0);
			}
			if (flags & (1 << 3)) { // WE_HAVE_A_SCALE
				mtx[0] = mtx[3] = ttSHORT(comp) / 16384.0f; comp += 2;
				mtx[1] = mtx[2] = 0;
			}
			else if (flags & (1 << 6)) { // WE_HAVE_AN_X_AND_YSCALE
				mtx[0] = ttSHORT(comp) / 16384.0f; comp += 2;
				mtx[1] = mtx[2] = 0;
				mtx[3] = ttSHORT(comp) / 16384.0f; comp += 2;
			}
			else if (flags & (1 << 7)) { // WE_HAVE_A_TWO_BY_TWO
				mtx[0] = ttSHORT(comp) / 16384.0f; comp += 2;
				mtx[1] = ttSHORT(comp) / 16384.0f; comp += 2;
				mtx[2] = ttSHORT(comp) / 16384.0f; comp += 2;
				mtx[3] = ttSHORT(comp) / 16384.0f; comp += 2;
			}

			// Find transformation scales.
			m = (float)STBTT_sqrt(mtx[0] * mtx[0] + mtx[1] * mtx[1]);
			n = (float)STBTT_sqrt(mtx[2] * mtx[2] + mtx[3] * mtx[3]);

			// Get indexed glyph.
			comp_num_verts = stbtt_GetGlyphShape(info, gidx, &comp_verts);
			if (comp_num_verts > 0) {
				// Transform vertices.
				for (i = 0; i < comp_num_verts; ++i) {
					stbtt_vertex* v = &comp_verts[i];
					stbtt_vertex_type x, y;
					x = v->x; y = v->y;
					v->x = (stbtt_vertex_type)(m * (mtx[0] * x + mtx[2] * y + mtx[4]));
					v->y = (stbtt_vertex_type)(n * (mtx[1] * x + mtx[3] * y + mtx[5]));
					x = v->cx; y = v->cy;
					v->cx = (stbtt_vertex_type)(m * (mtx[0] * x + mtx[2] * y + mtx[4]));
					v->cy = (stbtt_vertex_type)(n * (mtx[1] * x + mtx[3] * y + mtx[5]));
				}
				// Append vertices.
				tmp = (stbtt_vertex*)STBTT_malloc((num_vertices + comp_num_verts)*sizeof(stbtt_vertex), info->userdata);
				if (!tmp) {
					if (vertices) STBTT_free(vertices, info->userdata);
					if (comp_verts) STBTT_free(comp_verts, info->userdata);
					return 0;
				}
				if (num_vertices > 0) STBTT_memcpy(tmp, vertices, num_vertices*sizeof(stbtt_vertex));
				STBTT_memcpy(tmp + num_vertices, comp_verts, comp_num_verts*sizeof(stbtt_vertex));
				if (vertices) STBTT_free(vertices, info->userdata);
				vertices = tmp;
				STBTT_free(comp_verts, info->userdata);
				num_vertices += comp_num_verts;
			}
			// More components ?
			more = flags & (1 << 5);
		}
	}
	else if (numberOfContours < 0) {
		// @TODO other compound variations?
		STBTT_assert(0);
	}
	else {
		// numberOfCounters == 0, do nothing
	}

	*pvertices = vertices;
	return num_vertices;
}

STBTT_DEF void stbtt_GetGlyphHMetrics(const stbtt_fontinfo *info, int glyph_index, int *advanceWidth, int *leftSideBearing)
{
	stbtt_uint16 numOfLongHorMetrics = ttUSHORT(info->data + info->hhea + 34);
	if (glyph_index < numOfLongHorMetrics) {
		if (advanceWidth)     *advanceWidth = ttSHORT(info->data + info->hmtx + 4 * glyph_index);
		if (leftSideBearing)  *leftSideBearing = ttSHORT(info->data + info->hmtx + 4 * glyph_index + 2);
	}
	else {
		if (advanceWidth)     *advanceWidth = ttSHORT(info->data + info->hmtx + 4 * (numOfLongHorMetrics - 1));
		if (leftSideBearing)  *leftSideBearing = ttSHORT(info->data + info->hmtx + 4 * numOfLongHorMetrics + 2 * (glyph_index - numOfLongHorMetrics));
	}
}

STBTT_DEF int  stbtt_GetGlyphKernAdvance(const stbtt_fontinfo *info, int glyph1, int glyph2)
{
	stbtt_uint8 *data = info->data + info->kern;
	stbtt_uint32 needle, straw;
	int l, r, m;

	// we only look at the first table. it must be 'horizontal' and format 0.
	if (!info->kern)
		return 0;
	if (ttUSHORT(data + 2) < 1) // number of tables, need at least 1
		return 0;
	if (ttUSHORT(data + 8) != 1) // horizontal flag must be set in format
		return 0;

	l = 0;
	r = ttUSHORT(data + 10) - 1;
	needle = glyph1 << 16 | glyph2;
	while (l <= r) {
		m = (l + r) >> 1;
		straw = ttULONG(data + 18 + (m * 6)); // note: unaligned read
		if (needle < straw)
			r = m - 1;
		else if (needle > straw)
			l = m + 1;
		else
			return ttSHORT(data + 22 + (m * 6));
	}
	return 0;
}

STBTT_DEF int  stbtt_GetCodepointKernAdvance(const stbtt_fontinfo *info, int ch1, int ch2)
{
	if (!info->kern) // if no kerning table, don't waste time looking up both codepoint->glyphs
		return 0;
	return stbtt_GetGlyphKernAdvance(info, stbtt_FindGlyphIndex(info, ch1), stbtt_FindGlyphIndex(info, ch2));
}

STBTT_DEF void stbtt_GetCodepointHMetrics(const stbtt_fontinfo *info, int codepoint, int *advanceWidth, int *leftSideBearing)
{
	stbtt_GetGlyphHMetrics(info, stbtt_FindGlyphIndex(info, codepoint), advanceWidth, leftSideBearing);
}

STBTT_DEF void stbtt_GetFontVMetrics(const stbtt_fontinfo *info, int *ascent, int *descent, int *lineGap)
{
	if (ascent) *ascent = ttSHORT(info->data + info->hhea + 4);
	if (descent) *descent = ttSHORT(info->data + info->hhea + 6);
	if (lineGap) *lineGap = ttSHORT(info->data + info->hhea + 8);
}

STBTT_DEF void stbtt_GetFontBoundingBox(const stbtt_fontinfo *info, int *x0, int *y0, int *x1, int *y1)
{
	*x0 = ttSHORT(info->data + info->head + 36);
	*y0 = ttSHORT(info->data + info->head + 38);
	*x1 = ttSHORT(info->data + info->head + 40);
	*y1 = ttSHORT(info->data + info->head + 42);
}

STBTT_DEF float stbtt_ScaleForPixelHeight(const stbtt_fontinfo *info, float height)
{
	int fheight = ttSHORT(info->data + info->hhea + 4) - ttSHORT(info->data + info->hhea + 6);
	return (float)height / fheight;
}

STBTT_DEF float stbtt_ScaleForMappingEmToPixels(const stbtt_fontinfo *info, float pixels)
{
	int unitsPerEm = ttUSHORT(info->data + info->head + 18);
	return pixels / unitsPerEm;
}

STBTT_DEF void stbtt_FreeShape(const stbtt_fontinfo *info, stbtt_vertex *v)
{
	STBTT_free(v, info->userdata);
}

//////////////////////////////////////////////////////////////////////////////
//
// antialiasing software rasterizer
//

STBTT_DEF void stbtt_GetGlyphBitmapBoxSubpixel(const stbtt_fontinfo *font, int glyph, float scale_x, float scale_y, float shift_x, float shift_y, int *ix0, int *iy0, int *ix1, int *iy1)
{
	int x0, y0, x1, y1;
	if (!stbtt_GetGlyphBox(font, glyph, &x0, &y0, &x1, &y1)) {
		// e.g. space character
		if (ix0) *ix0 = 0;
		if (iy0) *iy0 = 0;
		if (ix1) *ix1 = 0;
		if (iy1) *iy1 = 0;
	}
	else {
		// move to integral bboxes (treating pixels as little squares, what pixels get touched)?
		if (ix0) *ix0 = STBTT_ifloor(x0 * scale_x + shift_x);
		if (iy0) *iy0 = STBTT_ifloor(-y1 * scale_y + shift_y);
		if (ix1) *ix1 = STBTT_iceil(x1 * scale_x + shift_x);
		if (iy1) *iy1 = STBTT_iceil(-y0 * scale_y + shift_y);
	}
}

STBTT_DEF void stbtt_GetGlyphBitmapBox(const stbtt_fontinfo *font, int glyph, float scale_x, float scale_y, int *ix0, int *iy0, int *ix1, int *iy1)
{
	stbtt_GetGlyphBitmapBoxSubpixel(font, glyph, scale_x, scale_y, 0.0f, 0.0f, ix0, iy0, ix1, iy1);
}

STBTT_DEF void stbtt_GetCodepointBitmapBoxSubpixel(const stbtt_fontinfo *font, int codepoint, float scale_x, float scale_y, float shift_x, float shift_y, int *ix0, int *iy0, int *ix1, int *iy1)
{
	stbtt_GetGlyphBitmapBoxSubpixel(font, stbtt_FindGlyphIndex(font, codepoint), scale_x, scale_y, shift_x, shift_y, ix0, iy0, ix1, iy1);
}

STBTT_DEF void stbtt_GetCodepointBitmapBox(const stbtt_fontinfo *font, int codepoint, float scale_x, float scale_y, int *ix0, int *iy0, int *ix1, int *iy1)
{
	stbtt_GetCodepointBitmapBoxSubpixel(font, codepoint, scale_x, scale_y, 0.0f, 0.0f, ix0, iy0, ix1, iy1);
}

typedef struct stbtt__edge {
	float x0, y0, x1, y1;
	int invert;
} stbtt__edge;

typedef struct stbtt__active_edge
{
	int x, dx;
	float ey;
	struct stbtt__active_edge *next;
	int valid;
} stbtt__active_edge;

#define FIXSHIFT   10
#define FIX        (1 << FIXSHIFT)
#define FIXMASK    (FIX-1)

static stbtt__active_edge *new_active(stbtt__edge *e, int off_x, float start_point, void *userdata)
{
	stbtt__active_edge *z = (stbtt__active_edge *)STBTT_malloc(sizeof(*z), userdata); // @TODO: make a pool of these!!!
	float dxdy = (e->x1 - e->x0) / (e->y1 - e->y0);
	STBTT_assert(e->y0 <= start_point);
	if (!z) return z;
	// round dx down to avoid going too far
	if (dxdy < 0)
		z->dx = -STBTT_ifloor(FIX * -dxdy);
	else
		z->dx = STBTT_ifloor(FIX * dxdy);
	z->x = STBTT_ifloor(FIX * (e->x0 + dxdy * (start_point - e->y0)));
	z->x -= off_x * FIX;
	z->ey = e->y1;
	z->next = 0;
	z->valid = e->invert ? 1 : -1;
	return z;
}

// note: this routine clips fills that extend off the edges... ideally this
// wouldn't happen, but it could happen if the truetype glyph bounding boxes
// are wrong, or if the user supplies a too-small bitmap
static void stbtt__fill_active_edges(unsigned char *scanline, int len, stbtt__active_edge *e, int max_weight)
{
	// non-zero winding fill
	int x0 = 0, w = 0;

	while (e) {
		if (w == 0) {
			// if we're currently at zero, we need to record the edge start point
			x0 = e->x; w += e->valid;
		}
		else {
			int x1 = e->x; w += e->valid;
			// if we went to zero, we need to draw
			if (w == 0) {
				int i = x0 >> FIXSHIFT;
				int j = x1 >> FIXSHIFT;

				if (i < len && j >= 0) {
					if (i == j) {
						// x0,x1 are the same pixel, so compute combined coverage
						scanline[i] = scanline[i] + (stbtt_uint8)((x1 - x0) * max_weight >> FIXSHIFT);
					}
					else {
						if (i >= 0) // add antialiasing for x0
							scanline[i] = scanline[i] + (stbtt_uint8)(((FIX - (x0 & FIXMASK)) * max_weight) >> FIXSHIFT);
						else
							i = -1; // clip

						if (j < len) // add antialiasing for x1
							scanline[j] = scanline[j] + (stbtt_uint8)(((x1 & FIXMASK) * max_weight) >> FIXSHIFT);
						else
							j = len; // clip

						for (++i; i < j; ++i) // fill pixels between x0 and x1
							scanline[i] = scanline[i] + (stbtt_uint8)max_weight;
					}
				}
			}
		}

		e = e->next;
	}
}

static void stbtt__rasterize_sorted_edges(stbtt__bitmap *result, stbtt__edge *e, int n, int vsubsample, int off_x, int off_y, void *userdata)
{
	stbtt__active_edge *active = NULL;
	int y, j = 0;
	int max_weight = (255 / vsubsample);  // weight per vertical scanline
	int s; // vertical subsample index
	unsigned char scanline_data[512], *scanline;

	if (result->w > 512)
		scanline = (unsigned char *)STBTT_malloc(result->w, userdata);
	else
		scanline = scanline_data;

	y = off_y * vsubsample;
	e[n].y0 = (off_y + result->h) * (float)vsubsample + 1;

	while (j < result->h) {
		STBTT_memset(scanline, 0, result->w);
		for (s = 0; s < vsubsample; ++s) {
			// find center of pixel for this scanline
			float scan_y = y + 0.5f;
			stbtt__active_edge **step = &active;

			// update all active edges;
			// remove all active edges that terminate before the center of this scanline
			while (*step) {
				stbtt__active_edge * z = *step;
				if (z->ey <= scan_y) {
					*step = z->next; // delete from list
					STBTT_assert(z->valid);
					z->valid = 0;
					STBTT_free(z, userdata);
				}
				else {
					z->x += z->dx; // advance to position for current scanline
					step = &((*step)->next); // advance through list
				}
			}

			// resort the list if needed
			for (;;) {
				int changed = 0;
				step = &active;
				while (*step && (*step)->next) {
					if ((*step)->x > (*step)->next->x) {
						stbtt__active_edge *t = *step;
						stbtt__active_edge *q = t->next;

						t->next = q->next;
						q->next = t;
						*step = q;
						changed = 1;
					}
					step = &(*step)->next;
				}
				if (!changed) break;
			}

			// insert all edges that start before the center of this scanline -- omit ones that also end on this scanline
			while (e->y0 <= scan_y) {
				if (e->y1 > scan_y) {
					stbtt__active_edge *z = new_active(e, off_x, scan_y, userdata);
					// find insertion point
					if (active == NULL)
						active = z;
					else if (z->x < active->x) {
						// insert at front
						z->next = active;
						active = z;
					}
					else {
						// find thing to insert AFTER
						stbtt__active_edge *p = active;
						while (p->next && p->next->x < z->x)
							p = p->next;
						// at this point, p->next->x is NOT < z->x
						z->next = p->next;
						p->next = z;
					}
				}
				++e;
			}

			// now process all active edges in XOR fashion
			if (active)
				stbtt__fill_active_edges(scanline, result->w, active, max_weight);

			++y;
		}
		STBTT_memcpy(result->pixels + j * result->stride, scanline, result->w);
		++j;
	}

	while (active) {
		stbtt__active_edge *z = active;
		active = active->next;
		STBTT_free(z, userdata);
	}

	if (scanline != scanline_data)
		STBTT_free(scanline, userdata);
}

static int stbtt__edge_compare(const void *p, const void *q)
{
	stbtt__edge *a = (stbtt__edge *)p;
	stbtt__edge *b = (stbtt__edge *)q;

	if (a->y0 < b->y0) return -1;
	if (a->y0 > b->y0) return  1;
	return 0;
}

typedef struct
{
	float x, y;
} stbtt__point;

static void stbtt__rasterize(stbtt__bitmap *result, stbtt__point *pts, int *wcount, int windings, float scale_x, float scale_y, float shift_x, float shift_y, int off_x, int off_y, int invert, void *userdata)
{
	float y_scale_inv = invert ? -scale_y : scale_y;
	stbtt__edge *e;
	int n, i, j, k, m;
	int vsubsample = result->h < 8 ? 15 : 5;
	// vsubsample should divide 255 evenly; otherwise we won't reach full opacity

	// now we have to blow out the windings into explicit edge lists
	n = 0;
	for (i = 0; i < windings; ++i)
		n += wcount[i];

	e = (stbtt__edge *)STBTT_malloc(sizeof(*e) * (n + 1), userdata); // add an extra one as a sentinel
	if (e == 0) return;
	n = 0;

	m = 0;
	for (i = 0; i < windings; ++i) {
		stbtt__point *p = pts + m;
		m += wcount[i];
		j = wcount[i] - 1;
		for (k = 0; k < wcount[i]; j = k++) {
			int a = k, b = j;
			// skip the edge if horizontal
			if (p[j].y == p[k].y)
				continue;
			// add edge from j to k to the list
			e[n].invert = 0;
			if (invert ? p[j].y > p[k].y : p[j].y < p[k].y) {
				e[n].invert = 1;
				a = j, b = k;
			}
			e[n].x0 = p[a].x * scale_x + shift_x;
			e[n].y0 = (p[a].y * y_scale_inv + shift_y) * vsubsample;
			e[n].x1 = p[b].x * scale_x + shift_x;
			e[n].y1 = (p[b].y * y_scale_inv + shift_y) * vsubsample;
			++n;
		}
	}

	// now sort the edges by their highest point (should snap to integer, and then by x)
	STBTT_sort(e, n, sizeof(e[0]), stbtt__edge_compare);

	// now, traverse the scanlines and find the intersections on each scanline, use xor winding rule
	stbtt__rasterize_sorted_edges(result, e, n, vsubsample, off_x, off_y, userdata);

	STBTT_free(e, userdata);
}

static void stbtt__add_point(stbtt__point *points, int n, float x, float y)
{
	if (!points) return; // during first pass, it's unallocated
	points[n].x = x;
	points[n].y = y;
}

// tesselate until threshhold p is happy... @TODO warped to compensate for non-linear stretching
static int stbtt__tesselate_curve(stbtt__point *points, int *num_points, float x0, float y0, float x1, float y1, float x2, float y2, float objspace_flatness_squared, int n)
{
	// midpoint
	float mx = (x0 + 2 * x1 + x2) / 4;
	float my = (y0 + 2 * y1 + y2) / 4;
	// versus directly drawn line
	float dx = (x0 + x2) / 2 - mx;
	float dy = (y0 + y2) / 2 - my;
	if (n > 16) // 65536 segments on one curve better be enough!
		return 1;
	if (dx*dx + dy*dy > objspace_flatness_squared) { // half-pixel error allowed... need to be smaller if AA
		stbtt__tesselate_curve(points, num_points, x0, y0, (x0 + x1) / 2.0f, (y0 + y1) / 2.0f, mx, my, objspace_flatness_squared, n + 1);
		stbtt__tesselate_curve(points, num_points, mx, my, (x1 + x2) / 2.0f, (y1 + y2) / 2.0f, x2, y2, objspace_flatness_squared, n + 1);
	}
	else {
		stbtt__add_point(points, *num_points, x2, y2);
		*num_points = *num_points + 1;
	}
	return 1;
}

// returns number of contours
static stbtt__point *stbtt_FlattenCurves(stbtt_vertex *vertices, int num_verts, float objspace_flatness, int **contour_lengths, int *num_contours, void *userdata)
{
	stbtt__point *points = 0;
	int num_points = 0;

	float objspace_flatness_squared = objspace_flatness * objspace_flatness;
	int i, n = 0, start = 0, pass;

	// count how many "moves" there are to get the contour count
	for (i = 0; i < num_verts; ++i)
	if (vertices[i].type == STBTT_vmove)
		++n;

	*num_contours = n;
	if (n == 0) return 0;

	*contour_lengths = (int *)STBTT_malloc(sizeof(**contour_lengths) * n, userdata);

	if (*contour_lengths == 0) {
		*num_contours = 0;
		return 0;
	}

	// make two passes through the points so we don't need to realloc
	for (pass = 0; pass < 2; ++pass) {
		float x = 0, y = 0;
		if (pass == 1) {
			points = (stbtt__point *)STBTT_malloc(num_points * sizeof(points[0]), userdata);
			if (points == NULL) goto error;
		}
		num_points = 0;
		n = -1;
		for (i = 0; i < num_verts; ++i) {
			switch (vertices[i].type) {
			case STBTT_vmove:
				// start the next contour
				if (n >= 0)
					(*contour_lengths)[n] = num_points - start;
				++n;
				start = num_points;

				x = vertices[i].x, y = vertices[i].y;
				stbtt__add_point(points, num_points++, x, y);
				break;
			case STBTT_vline:
				x = vertices[i].x, y = vertices[i].y;
				stbtt__add_point(points, num_points++, x, y);
				break;
			case STBTT_vcurve:
				stbtt__tesselate_curve(points, &num_points, x, y,
					vertices[i].cx, vertices[i].cy,
					vertices[i].x, vertices[i].y,
					objspace_flatness_squared, 0);
				x = vertices[i].x, y = vertices[i].y;
				break;
			}
		}
		(*contour_lengths)[n] = num_points - start;
	}

	return points;
error:
	STBTT_free(points, userdata);
	STBTT_free(*contour_lengths, userdata);
	*contour_lengths = 0;
	*num_contours = 0;
	return NULL;
}

STBTT_DEF void stbtt_Rasterize(stbtt__bitmap *result, float flatness_in_pixels, stbtt_vertex *vertices, int num_verts, float scale_x, float scale_y, float shift_x, float shift_y, int x_off, int y_off, int invert, void *userdata)
{
	float scale = scale_x > scale_y ? scale_y : scale_x;
	int winding_count, *winding_lengths;
	stbtt__point *windings = stbtt_FlattenCurves(vertices, num_verts, flatness_in_pixels / scale, &winding_lengths, &winding_count, userdata);
	if (windings) {
		stbtt__rasterize(result, windings, winding_lengths, winding_count, scale_x, scale_y, shift_x, shift_y, x_off, y_off, invert, userdata);
		STBTT_free(winding_lengths, userdata);
		STBTT_free(windings, userdata);
	}
}

STBTT_DEF void stbtt_FreeBitmap(unsigned char *bitmap, void *userdata)
{
	STBTT_free(bitmap, userdata);
}

STBTT_DEF unsigned char *stbtt_GetGlyphBitmapSubpixel(const stbtt_fontinfo *info, float scale_x, float scale_y, float shift_x, float shift_y, int glyph, int *width, int *height, int *xoff, int *yoff)
{
	int ix0, iy0, ix1, iy1;
	stbtt__bitmap gbm;
	stbtt_vertex *vertices;
	int num_verts = stbtt_GetGlyphShape(info, glyph, &vertices);

	if (scale_x == 0) scale_x = scale_y;
	if (scale_y == 0) {
		if (scale_x == 0) return NULL;
		scale_y = scale_x;
	}

	stbtt_GetGlyphBitmapBoxSubpixel(info, glyph, scale_x, scale_y, shift_x, shift_y, &ix0, &iy0, &ix1, &iy1);

	// now we get the size
	gbm.w = (ix1 - ix0);
	gbm.h = (iy1 - iy0);
	gbm.pixels = NULL; // in case we error

	if (width) *width = gbm.w;
	if (height) *height = gbm.h;
	if (xoff) *xoff = ix0;
	if (yoff) *yoff = iy0;

	if (gbm.w && gbm.h) {
		gbm.pixels = (unsigned char *)STBTT_malloc(gbm.w * gbm.h, info->userdata);
		if (gbm.pixels) {
			gbm.stride = gbm.w;

			stbtt_Rasterize(&gbm, 0.35f, vertices, num_verts, scale_x, scale_y, shift_x, shift_y, ix0, iy0, 1, info->userdata);
		}
	}
	STBTT_free(vertices, info->userdata);
	return gbm.pixels;
}

STBTT_DEF unsigned char *stbtt_GetGlyphBitmap(const stbtt_fontinfo *info, float scale_x, float scale_y, int glyph, int *width, int *height, int *xoff, int *yoff)
{
	return stbtt_GetGlyphBitmapSubpixel(info, scale_x, scale_y, 0.0f, 0.0f, glyph, width, height, xoff, yoff);
}

STBTT_DEF void stbtt_MakeGlyphBitmapSubpixel(const stbtt_fontinfo *info, unsigned char *output, int out_w, int out_h, int out_stride, float scale_x, float scale_y, float shift_x, float shift_y, int glyph)
{
	int ix0, iy0;
	stbtt_vertex *vertices;
	int num_verts = stbtt_GetGlyphShape(info, glyph, &vertices);
	stbtt__bitmap gbm;

	stbtt_GetGlyphBitmapBoxSubpixel(info, glyph, scale_x, scale_y, shift_x, shift_y, &ix0, &iy0, 0, 0);
	gbm.pixels = output;
	gbm.w = out_w;
	gbm.h = out_h;
	gbm.stride = out_stride;

	if (gbm.w && gbm.h)
		stbtt_Rasterize(&gbm, 0.35f, vertices, num_verts, scale_x, scale_y, shift_x, shift_y, ix0, iy0, 1, info->userdata);

	STBTT_free(vertices, info->userdata);
}

STBTT_DEF void stbtt_MakeGlyphBitmap(const stbtt_fontinfo *info, unsigned char *output, int out_w, int out_h, int out_stride, float scale_x, float scale_y, int glyph)
{
	stbtt_MakeGlyphBitmapSubpixel(info, output, out_w, out_h, out_stride, scale_x, scale_y, 0.0f, 0.0f, glyph);
}

STBTT_DEF unsigned char *stbtt_GetCodepointBitmapSubpixel(const stbtt_fontinfo *info, float scale_x, float scale_y, float shift_x, float shift_y, int codepoint, int *width, int *height, int *xoff, int *yoff)
{
	return stbtt_GetGlyphBitmapSubpixel(info, scale_x, scale_y, shift_x, shift_y, stbtt_FindGlyphIndex(info, codepoint), width, height, xoff, yoff);
}

STBTT_DEF void stbtt_MakeCodepointBitmapSubpixel(const stbtt_fontinfo *info, unsigned char *output, int out_w, int out_h, int out_stride, float scale_x, float scale_y, float shift_x, float shift_y, int codepoint)
{
	stbtt_MakeGlyphBitmapSubpixel(info, output, out_w, out_h, out_stride, scale_x, scale_y, shift_x, shift_y, stbtt_FindGlyphIndex(info, codepoint));
}

STBTT_DEF unsigned char *stbtt_GetCodepointBitmap(const stbtt_fontinfo *info, float scale_x, float scale_y, int codepoint, int *width, int *height, int *xoff, int *yoff)
{
	return stbtt_GetCodepointBitmapSubpixel(info, scale_x, scale_y, 0.0f, 0.0f, codepoint, width, height, xoff, yoff);
}

STBTT_DEF void stbtt_MakeCodepointBitmap(const stbtt_fontinfo *info, unsigned char *output, int out_w, int out_h, int out_stride, float scale_x, float scale_y, int codepoint)
{
	stbtt_MakeCodepointBitmapSubpixel(info, output, out_w, out_h, out_stride, scale_x, scale_y, 0.0f, 0.0f, codepoint);
}

