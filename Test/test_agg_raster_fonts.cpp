
#include "drawproc.h"

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


void keyReleased()
{
	switch (keyCode)
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

		case VK_SPACE:
			write_PPM_binary("test_agg_raster_fonts.ppm", gpb);
		break;
	}

}

void setup()
{
	size(640, 480);
	background(155);
}

static char CAPS[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
static char LOWS[] = "abcdefghijklmnopqrstuvwxyz";
static char NUMS[] = "1234567890";
static char SYMS[] = "!@#$%^&*()_+-={}|[]\\:\"; '<>?,./~`";
static char SOME[] = "The quick brown fox jumped over the lazy dog.";

void draw()
{
	background(255);

	setFont(fontlist[fontidx].data);

	textAlign(TX_LEFT, TX_TOP);
	fillRGBA(pBlack);
	text(fontlist[fontidx].name, 0, 0);
	text(CAPS, 0, gfont.height * 1);

	fillRGBA(pRed);
	text(LOWS, 0, gfont.height * 2);

	fillRGBA(pGreen);
	text(NUMS, 0, gfont.height * 3);

	fillRGBA(pBlue);
	text(SYMS, 0, gfont.height * 4);

	fillRGBA(pBlack);
	text(SOME, 0, gfont.height * 5);
}


