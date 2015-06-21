

#include "drawproc.h"
#include "truetype.h"

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdint.h>
#include <string.h>




void ttfset_print(struct ttfcollection &fset)
{
	printf("TTF Set: %s\n", fset.filename);
}

/*
bool ttfset_open(struct ttfset &fset, const char *filename)
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
*/

struct stbtt_fontinfo ttfcollection_font_from_index(const struct ttfcollection &fset, const int idx)
{
	int offset = stbtt_GetFontOffsetForIndex(fset.data, idx);
	
	struct stbtt_fontinfo finfo;

	stbtt_InitFont(&finfo, fset.data, offset);

	return finfo;
}

void font_print_info(struct stbtt_fontinfo &info)
{
	int ascent, descent, lineGap;
	int x0, y0 = 0;
	int x1, y1 = 0;


	stbtt_GetFontVMetrics(&info, &ascent, &descent, &lineGap);
	printf("== VMetrics ==\n");
	printf("   Ascent: %d\n", ascent);
	printf("  Descent: %d\n", descent);
	printf("  lineGap: %d\n", lineGap);

	stbtt_GetFontBoundingBox(&info, &x0, &y0, &x1, &y1);
	printf("== Bounding Box ==\n");
	printf("[%d, %d], [%d,%d]\n", x0, y0, x1, y1);
}

/*
int pb_gray8_init(pb_rgba *fb, const unsigned int width, const unsigned int height)
{
	if (!fb) return -1;

	size_t pitch = width * 1;
	size_t datasize = height * pitch;

	fb->data = (unsigned char *)calloc(datasize, 1);
	fb->owndata = 1;

	fb->frame.x = 0;
	fb->frame.y = 0;
	fb->frame.width = width;
	fb->frame.height = height;

	fb->pixelpitch = width;
	fb->bitStride = width * 8;

	return 0;
}
*/


struct ttfcollection fontset;
struct stbtt_fontinfo finfo;

void setup()
{
	size(1024, 768);

	//char *filename = "c:/windows/fonts/arial.ttf";
	char *filename = "c:/windows/fonts/segoeui.ttf";

	if (!ttfcollection_open(filename, fontset)) {
		return ;
	}

	finfo = ttfcollection_font_from_index(fontset, 0);
}

unsigned char screen[20][79];


void drawText()
{
	int i, j;
	int ascent, descent, lineGap, baseline;
	float scale, xpos = 2; // leave a little padding in case the character extends left
	float ypos = 2;

	char *text = "Hella Werld!";
	unsigned char *bitmap;

	stbtt_GetFontVMetrics(&finfo, &ascent, &descent, &lineGap);

	for (int pixfactor = 3; pixfactor < 9; pixfactor++) {

		int pixsize = pow((float)2, pixfactor);
		scale = stbtt_ScaleForPixelHeight(&finfo, pixsize);
		baseline = (int)(ascent*scale);

		int idx = 0;
		while (text[idx]) {
			int advance, lsb, x0, y0, x1, y1;
			float x_shift = xpos - (float)floor(xpos);
			stbtt_GetCodepointHMetrics(&finfo, text[idx], &advance, &lsb);
			stbtt_GetCodepointBitmapBoxSubpixel(&finfo, text[idx], scale, scale, x_shift, 0, &x0, &y0, &x1, &y1);
			//stbtt_MakeCodepointBitmapSubpixel(&finfo, &screen[baseline + y0][(int)xpos + x0], x1 - x0, y1 - y0, 79, scale, scale, x_shift, 0, text[idx]);
			// note that this stomps the old data, so where character boxes overlap (e.g. 'lj') it's wrong
			// because this API is really for baking character bitmaps into textures. if you want to render
			// a sequence of characters, you really need to render each bitmap to a temp buffer, then
			// "alpha blend" that into the working buffer

			int w, h;
			bitmap = stbtt_GetCodepointBitmap(&finfo, 0, scale, text[idx], &w, &h, 0, 0);

			raster_rgba_blend_alphamap(gpb, xpos, ypos + baseline + y0, bitmap, w, h, pYellow);

			//printf("%d %d %d", baseline, y0, y1);
			xpos += (advance * scale);
			if (text[idx + 1])
				xpos += scale*stbtt_GetCodepointKernAdvance(&finfo, text[idx], text[idx + 1]);
			idx++;

			stbtt_FreeBitmap(bitmap, NULL);
		}
		xpos = 2;
		ypos += pixsize-(scale*descent);
	}
}

void drawFontBox()
{
	char strbuff[256];

	float scale = stbtt_ScaleForPixelHeight(&finfo, 96);
	int ascent, descent, lineGap;

	stbtt_GetFontVMetrics(&finfo, &ascent, &descent, &lineGap);
	printf("scale: %f\tascent: %d\tdescent: %d\tlineGap: %d\n", scale, ascent, descent, lineGap);

	int x0, y0, x1, y1;
	stbtt_GetFontBoundingBox(&finfo, &x0, &y0, &x1, &y1);
	printf("bounds: [%d, %d] [%d,%d]\n", x0, y0, x1, y1);

	int hoffset = 36;
	int voffset = 36;
	int fwidth = x1 - x0;
	int fheight = y1 - y0;
	int xleft = MAP(x0, x0, x1, hoffset, width - hoffset);
	int xright = MAP(x1, x0, x1, hoffset, width - hoffset);
	int xcenter = MAP(0, x0, x1, hoffset, width - hoffset);

	int ytop = MAP(y0, y0, y1, height - voffset, voffset);
	int ybottom = MAP(y1, y0, y1, height - voffset, voffset );
	int ycenter = MAP(0, y0, y1, height - voffset, voffset);

	// display the bounding box for the font
	stroke(pDarkGray);
	line(xleft, 0, xleft, height);
	line(xright, 0, xright, height);

	line(0, ytop, width, ytop);
	line(0, ybottom, width, ybottom);

	stroke(pBlue);
	line(xcenter, 0, xcenter, height);
	line(0, ycenter, width, ycenter);

	textAlign(TX_CENTER, TX_TOP);
	fill(pBlack);
	// x boundary labels
	sprintf_s(strbuff, "%d", x0);
	text(strbuff, xleft, 8);
	sprintf_s(strbuff, "%d", x1);
	text(strbuff, xright, 8);

	// y boundary labels
	sprintf_s(strbuff, "%d", y0);
	textAlign(TX_CENTER, TX_TOP);
	text(strbuff, width / 2, ytop);
	sprintf_s(strbuff, "%d", y1);
	textAlign(TX_CENTER, TX_BOTTOM);
	text(strbuff, width / 2, ybottom);

	// descent and ascent lines
	int descentY = MAP(descent, y0, y1, height - voffset, voffset);
	int ascentY = MAP(ascent, y0, y1, height - voffset, voffset);
	stroke(pRed);
	line(0, descentY, width, descentY);
	line(0, ascentY, width, ascentY);
}

void draw()
{
	background(pLightGray);
	//drawFontBox();
	drawText();
}