#include "test_common.h"
#include "agg_embedded_raster_fonts.h"
#include "animwin32.h"

struct font_entry {
	char *name;
	const uint8_t *data;
};


struct font_entry fontlist[] = {
	{"gse4x6", gse4x6 },
	{ "gse4x8", gse4x8 },
	{ "gse5x7", gse5x7 },
	{ "gse5x9", gse5x9 },
	{ "gse6x12", gse6x12 },
	{ "gse6x9", gse6x9 },
	{ "gse7x11", gse7x11 },
	{ "gse7x11_bold", gse7x11_bold },
	{ "gse7x15", gse7x15 },
	{ "gse7x15_bold", gse7x15_bold },
	{ "gse8x16", gse8x16 },
	{ "gse8x16_bold", gse8x16_bold },
	{ "mcs11_prop", mcs11_prop },
	{ "mcs11_prop_condensed", mcs11_prop_condensed },
	{ "mcs12_prop", mcs12_prop },
	{ "mcs13_prop", mcs13_prop },
	{ "mcs5x10_mono", mcs5x10_mono },
	{ "mcs5x11_mono", mcs5x11_mono },
	{ "mcs6x10_mono", mcs6x10_mono },
	{ "mcs6x11_mono", mcs6x11_mono },
	{ "mcs7x12_mono_high", mcs7x12_mono_high },
	{ "mcs7x12_mono_low", mcs7x12_mono_low },
	{ "verdana12", verdana12 },
	{ "verdana12_bold", verdana12_bold },
	{ "verdana13", verdana13 },
	{ "verdana13_bold", verdana13_bold },
	{ "verdana14", verdana14 },
	{ "verdana14_bold", verdana14_bold },
	{ "verdana16", verdana16 },
	{ "verdana16_bold", verdana16_bold },
	{ "verdana17", verdana17 },
	{ "verdana17_bold", verdana17_bold },
	{ "verdana18", verdana18 },
	{ "verdana18_bold", verdana18_bold }

};
static const int numfonts = sizeof(fontlist) / sizeof(fontlist[0]);
static int fontidx = 0;

void test_font_info()
{
	font_t vd12;

	// Read a font and print out info
	font_t_init(&vd12, verdana12);


	printf("FONT (Verdana 12) Height: %d\tbaseline: %d\tstart_char: %d\tnum_chars: %d\n", 
		vd12.height, vd12.baseline, 
		vd12.start_char,
		vd12.num_chars);

	size_t strwidth = font_t_str_width(&vd12, "Hello, World!");
	printf("string width ==> 'Hello, World!' : %d\n", strwidth);

	strwidth = font_t_str_width(&vd12, ".");
	printf("string width ==> '.': %d\n", strwidth);

	strwidth = font_t_str_width(&vd12, "+");
	printf("string width ==> '+': %d\n", strwidth);
}

void test_prepare_glyph()
{
	font_t vd12;
	glyph_t ginfo;
	struct glyph_rect r;
	double x = 0;
	double y = 12;
	bool flip = true;

	font_t_init(&vd12, verdana12);
	glyph_t_init(&vd12, &ginfo, 'H');
	glyph_t_prepare(&vd12, &ginfo, &r, x, y, flip);

	printf("PREPARED ('H'): %d, %d  %d, %d\n", r.x1, r.y1, r.x2, r.y2);
}



void test_glyph_scan()
{
	font_t vd12;
	glyph_t ginfo;
	struct glyph_rect r;
	double x = 0;
	double y = 12;
	bool flip = false;

	size_t pbwidth = 20;
	size_t pbheight = 20;

	pb_rgba pb;
	pb_rgba_init(&pb, pbwidth, pbheight);

	raster_rgba_rect_fill(&pb, 0, 0, pbwidth - 1, pbheight - 1, pWhite);


	font_t_init(&vd12, verdana12);
	glyph_t_init(&vd12, &ginfo, 'A');
	glyph_t_prepare(&vd12, &ginfo, &r, x, y, flip);

	scan_glyph(&pb, &vd12, &ginfo, 0, 0, pBlack);

	int err = write_PPM("test_glyph_scan.ppm", &pb);
}

LRESULT CALLBACK keyReleased(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
		case VK_UP:
			fontidx--;
			if (fontidx < 0) {
				fontidx = numfonts - 1;
			}
		break;

		case VK_DOWN:
			fontidx++;
			if (fontidx >= numfonts){
				fontidx = 0;
			}
		break;
	}

	return 0;
}

void setup()
{
	size(640, 480);
	background(pWhite);

	setOnKeyReleasedHandler(keyReleased);
}

void draw()
{
	// Allocate a color buffer based on the size of the string
	//size_t pbwidth = 640;
	//size_t pbheight = 480;

	//pb_rgba pb;
	//pb_rgba_init(&pb, pbwidth, pbheight);
	//pb_rgba_init(&pb, pbwidth, pbheight);
	//raster_rgba_rect_fill(&pb, 0, 0, pbwidth - 1, pbheight - 1, pWhite);

	char CAPS[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	char LOWS[] = "abcdefghijklmnopqrstuvwxyz";
	char NUMS[] = "1234567890";
	char SYMS[] = "!@#$%^&*()_+-={}|[]\\:\"; '<>?,./~`";
	char SOME[] = "The quick brown fox jumped over the lazy dog.";

	font_t font;

	font_t_init(&font, fontlist[fontidx].data);

	scan_str(gpb, &font, 0, 0, fontlist[fontidx].name, pBlack);
	scan_str(gpb, &font, 0, font.height * 1, CAPS, pBlack);
	scan_str(gpb, &font, 0, font.height * 2, LOWS, pRed);
	scan_str(gpb, &font, 0, font.height * 3, NUMS, pGreen);
	scan_str(gpb, &font, 0, font.height * 4, SYMS, pBlue);
	scan_str(gpb, &font, 0, font.height * 5, SOME, pBlack);


}


