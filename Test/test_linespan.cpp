#include "test_common.h"

void test_linespan()
{
	size_t width = 320;
	size_t height = 240;
	pb_rgba pb;
	pb_rgba_init(&pb, width, height);


	// Set a white background
	raster_rgba_rect_fill(&pb, 0, 0, width, height, pWhite);

	// do a horizontal fade
	int color1 = RGBA(127, 102, 0, 255);
	int color2 = RGBA(0, 127, 212, 255);

	for (size_t row = 0; row < 100; row++) {
		int x1 = 0;
		int x2 = width - 1;
		raster_rgba_hline_span(&pb, x1, color1, x2, color2, row);
	}

	// Draw a button looking thing
	// black
	raster_rgba_hline(&pb, 20, 20, 80, pBlack);
	raster_rgba_hline(&pb, 20, 21, 80, pDarkGray);
	raster_rgba_hline(&pb, 20, 22, 80, pLightGray);


	// light gray rect
	raster_rgba_rect_fill(&pb, 20, 23, 80, 24, pLightGray);

	// fade to black
	for (size_t col = 20; col < 100; col++) {
		raster_rgba_vline_span(&pb, 46, pLightGray, 77, pBlack, col);
	}

	// Draw some blended lines atop the whole
	for (size_t row = 35; row < 120; row++){
		raster_rgba_hline_blend(&pb, 45, row, 100, RGBA(0, 153, 153, 203));
	}

	// Now we have a simple image, so write it to a file
	int err = write_PPM("test_linespan.ppm", &pb);

}



int main(int argc, char **argv)
{
	test_linespan();

	return 0;
}