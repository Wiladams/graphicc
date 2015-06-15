#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdint.h>
#include <string.h>

#include "truetype.h"

struct font {
	char *filename;
	uint8_t *data;
	stbtt_fontinfo info;
};


void font_print_info(struct font &finfo)
{
	int ascent, descent, lineGap;
	int x0, y0 = 0;
	int x1, y1 = 0;

	printf("Name: %s\n", finfo.filename);

	stbtt_GetFontVMetrics(&finfo.info, &ascent, &descent, &lineGap);
	printf("== VMetrics ==\n");
	printf("   Ascent: %d\n", ascent);
	printf("  Descent: %d\n", descent);
	printf("  lineGap: %d\n", lineGap);

	stbtt_GetFontBoundingBox(&finfo.info, &x0, &y0, &x1, &y1);
	printf("== Bounding Box ==\n");
	printf("[%d, %d], [%d,%d]\n", x0, y0, x1, y1);
}

bool font_open(struct font &finfo, const char *filename)
{
	FILE *f;
	fopen_s(&f, filename, "rb");

	if (f == nullptr) {
		return false;
	}

	// calculate size of file to allocate buffer
	// use fseek, ftell
	fseek(f, 0, SEEK_END);
	int bufferSize = ftell(f)+1;
	fseek(f, 0, SEEK_SET);

	finfo.data = (uint8_t *)malloc(bufferSize);
	finfo.filename = _strdup(filename);
	fread(finfo.data, 1, bufferSize, f);
	fclose(f);

	stbtt_InitFont(&finfo.info, finfo.data, 0);

	return true;
}


#if 0
unsigned char ttf_buffer[1 << 25];

int main(int argc, char **argv)
{
	stbtt_fontinfo font;
	unsigned char *bitmap;
	int w, h, i, j, c = (argc > 1 ? atoi(argv[1]) : 'a'), s = (argc > 2 ? atoi(argv[2]) : 20);

	FILE * f;
	fopen_s(&f, argc > 3 ? argv[3] : "c:/windows/fonts/arialbd.ttf", "rb");
	fread(ttf_buffer, 1, 1 << 25, f);

	stbtt_InitFont(&font, ttf_buffer, stbtt_GetFontOffsetForIndex(ttf_buffer, 0));
	bitmap = stbtt_GetCodepointBitmap(&font, 0, stbtt_ScaleForPixelHeight(&font, s), c, &w, &h, 0, 0);

	for (j = 0; j < h; ++j) {
		for (i = 0; i < w; ++i)
			putchar(" .:ioVM@"[bitmap[j*w + i] >> 5]);
		putchar('\n');
	}
	return 0;
}
#endif

#if 1
unsigned char screen[20][79];

int main(int arg, char **argv)
{
	struct font finfo;
	int i, j, ascent, baseline, ch = 0;
	float scale, xpos = 2; // leave a little padding in case the character extends left
	char *text = "Heljo World!";
	char *filename = "c:/windows/fonts/arial.ttf";
	//char *filename = "c:/windows/fonts/vijaya.ttf";

	if (!font_open(finfo, filename)) {
		return 1;
	}

	font_print_info(finfo);

	scale = stbtt_ScaleForPixelHeight(&finfo.info, 15);
	stbtt_GetFontVMetrics(&finfo.info, &ascent, 0, 0);
	baseline = (int)(ascent*scale);

	while (text[ch]) {
		int advance, lsb, x0, y0, x1, y1;
		float x_shift = xpos - (float)floor(xpos);
		stbtt_GetCodepointHMetrics(&finfo.info, text[ch], &advance, &lsb);
		stbtt_GetCodepointBitmapBoxSubpixel(&finfo.info, text[ch], scale, scale, x_shift, 0, &x0, &y0, &x1, &y1);
		stbtt_MakeCodepointBitmapSubpixel(&finfo.info, &screen[baseline + y0][(int)xpos + x0], x1 - x0, y1 - y0, 79, scale, scale, x_shift, 0, text[ch]);
		// note that this stomps the old data, so where character boxes overlap (e.g. 'lj') it's wrong
		// because this API is really for baking character bitmaps into textures. if you want to render
		// a sequence of characters, you really need to render each bitmap to a temp buffer, then
		// "alpha blend" that into the working buffer
		xpos += (advance * scale);
		if (text[ch + 1])
			xpos += scale*stbtt_GetCodepointKernAdvance(&finfo.info, text[ch], text[ch + 1]);
		++ch;
	}

	for (j = 0; j < 20; ++j) {
		for (i = 0; i < 79; ++i)
			putchar(" .:ioVM@"[screen[j][i] >> 5]);
		putchar('\n');
	}

	return 0;
}
#endif
