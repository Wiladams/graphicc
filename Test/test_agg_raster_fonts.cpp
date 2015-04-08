#include "test_common.h"
#include "agg_embedded_raster_fonts.h"



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

void test_str_print()
{
	font_t font;
	glyph_t ginfo;

	//font_t_init(&vd12, verdana12);
	//font_t_init(&font, verdana12_bold);
	//font_t_init(&font, verdana18);
	font_t_init(&font, verdana18_bold);

	//char words[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890!@#$%^&*()";
	//char words[] = "Hello, World!";
	//char words[] = "He";
	char TITLE[] = "Verdana 18 Bold";
	char CAPS[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	char LOWS[] = "abcdefghijklmnopqrstuvwxyz";
	char NUMS[] = "1234567890";
	char SYMS[] = "!@#$%^&*()_+-={}|[]\\:\"; '<>?,./~`";
	char *chars = LOWS;


	// Allocate a color buffer based on the size of the string
	size_t pbwidth = 640;
	size_t pbheight = 480;

	pb_rgba pb;
	//pb_rgba_init(&pb, pbwidth, pbheight);
	pb_rgba_init(&pb, pbwidth, pbheight);
	raster_rgba_rect_fill(&pb, 0, 0, pbwidth - 1, pbheight - 1, pWhite);

	scan_str(&pb, &font, 0, 0, TITLE, pBlack);
	scan_str(&pb, &font, 0, font.height * 1, CAPS, pBlack);
	scan_str(&pb, &font, 0, font.height * 2, LOWS, pRed);
	scan_str(&pb, &font, 0, font.height * 3, NUMS, pGreen);
	scan_str(&pb, &font, 0, font.height * 4, SYMS, pBlue);


	int err = write_PPM("test_str_print.ppm", &pb);

}

int main(int argc, char **argv)
{
	size_t pbwidth = 640;
	size_t pbheight = 480;

	pb_rgba pb;
	pb_rgba_init(&pb, pbwidth, pbheight);

	raster_rgba_rect_fill(&pb, 0, 0, pbwidth-1, pbheight-1, pBlue);

	//test_font_info();
	//test_prepare_glyph();
	//test_glyph_scan();
	test_str_print();

	int err = write_PPM("test_agg_raster_fonts.ppm", &pb);

	return 0;
}
