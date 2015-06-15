//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
////
////  SAMPLE PROGRAMS
////
//
//  Incomplete text-in-3d-api example, which draws quads properly aligned to be lossless
//
#if 0
#define STB_TRUETYPE_IMPLEMENTATION  // force following include to generate implementation
#include "stb_truetype.h"

unsigned char ttf_buffer[1 << 20];
unsigned char temp_bitmap[512 * 512];

stbtt_bakedchar cdata[96]; // ASCII 32..126 is 95 glyphs
GLuint ftex;

void my_stbtt_initfont(void)
{
	fread(ttf_buffer, 1, 1 << 20, fopen("c:/windows/fonts/times.ttf", "rb"));
	stbtt_BakeFontBitmap(ttf_buffer, 0, 32.0, temp_bitmap, 512, 512, 32, 96, cdata); // no guarantee this fits!
	// can free ttf_buffer at this point
	glGenTextures(1, &ftex);
	glBindTexture(GL_TEXTURE_2D, ftex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, 512, 512, 0, GL_ALPHA, GL_UNSIGNED_BYTE, temp_bitmap);
	// can free temp_bitmap at this point
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
}

void my_stbtt_print(float x, float y, char *text)
{
	// assume orthographic projection with units = screen pixels, origin at top left
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, ftex);
	glBegin(GL_QUADS);
	while (*text) {
		if (*text >= 32 && *text < 128) {
			stbtt_aligned_quad q;
			stbtt_GetBakedQuad(cdata, 512, 512, *text - 32, &x, &y, &q, 1);//1=opengl & d3d10+,0=d3d9
			glTexCoord2f(q.s0, q.t1); glVertex2f(q.x0, q.y0);
			glTexCoord2f(q.s1, q.t1); glVertex2f(q.x1, q.y0);
			glTexCoord2f(q.s1, q.t0); glVertex2f(q.x1, q.y1);
			glTexCoord2f(q.s0, q.t0); glVertex2f(q.x0, q.y1);
		}
		++text;
	}
	glEnd();
}
#endif
//
//
//////////////////////////////////////////////////////////////////////////////
//
// Complete program (this compiles): get a single bitmap, print as ASCII art
//
#if 0
#include <stdio.h>
#define STB_TRUETYPE_IMPLEMENTATION  // force following include to generate implementation
#include "stb_truetype.h"

char ttf_buffer[1 << 25];

int main(int argc, char **argv)
{
	stbtt_fontinfo font;
	unsigned char *bitmap;
	int w, h, i, j, c = (argc > 1 ? atoi(argv[1]) : 'a'), s = (argc > 2 ? atoi(argv[2]) : 20);

	fread(ttf_buffer, 1, 1 << 25, fopen(argc > 3 ? argv[3] : "c:/windows/fonts/arialbd.ttf", "rb"));

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
//
// Output:
//
//     .ii.
//    @@@@@@.
//   V@Mio@@o
//   :i.  V@V
//     :oM@@M
//   :@@@MM@M
//   @@o  o@M
//  :@@.  M@M
//   @@@o@@@@
//   :M@@V:@@.
//  
//////////////////////////////////////////////////////////////////////////////
// 
// Complete program: print "Hello World!" banner, with bugs
//
#if 0
char buffer[24 << 20];
unsigned char screen[20][79];

int main(int arg, char **argv)
{
	stbtt_fontinfo font;
	int i, j, ascent, baseline, ch = 0;
	float scale, xpos = 2; // leave a little padding in case the character extends left
	char *text = "Heljo World!";

	fread(buffer, 1, 1000000, fopen("c:/windows/fonts/arialbd.ttf", "rb"));
	stbtt_InitFont(&font, buffer, 0);

	scale = stbtt_ScaleForPixelHeight(&font, 15);
	stbtt_GetFontVMetrics(&font, &ascent, 0, 0);
	baseline = (int)(ascent*scale);

	while (text[ch]) {
		int advance, lsb, x0, y0, x1, y1;
		float x_shift = xpos - (float)floor(xpos);
		stbtt_GetCodepointHMetrics(&font, text[ch], &advance, &lsb);
		stbtt_GetCodepointBitmapBoxSubpixel(&font, text[ch], scale, scale, x_shift, 0, &x0, &y0, &x1, &y1);
		stbtt_MakeCodepointBitmapSubpixel(&font, &screen[baseline + y0][(int)xpos + x0], x1 - x0, y1 - y0, 79, scale, scale, x_shift, 0, text[ch]);
		// note that this stomps the old data, so where character boxes overlap (e.g. 'lj') it's wrong
		// because this API is really for baking character bitmaps into textures. if you want to render
		// a sequence of characters, you really need to render each bitmap to a temp buffer, then
		// "alpha blend" that into the working buffer
		xpos += (advance * scale);
		if (text[ch + 1])
			xpos += scale*stbtt_GetCodepointKernAdvance(&font, text[ch], text[ch + 1]);
		++ch;
	}

	for (j = 0; j < 20; ++j) {
		for (i = 0; i < 78; ++i)
			putchar(" .:ioVM@"[screen[j][i] >> 5]);
		putchar('\n');
	}

	return 0;
}
#endif