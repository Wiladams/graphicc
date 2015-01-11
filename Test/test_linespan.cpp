#include "test_common.h"

void test_hline()
{
	size_t width = 320;
	size_t height = 240;
	pb_rgba pb;
	pb_rgba_init(&pb, width, height);

	// Set a white background
	raster_rgba_rect_fill(&pb, 0, 0, width, height, pWhite);


	// Draw some horizontal lines
	raster_rgba_hline(&pb, 20, 20, 80, pBlack);
	raster_rgba_hline(&pb, 20, 21, 80, pBlack);

	raster_rgba_hline(&pb, 21, 23, 80, pDarkGray);
	raster_rgba_hline(&pb, 21, 24, 80, pDarkGray);

	raster_rgba_hline(&pb, 22, 26, 80, pLightGray);
	raster_rgba_hline(&pb, 22, 27, 80, pLightGray);

	// light gray rect
	//raster_rgba_rect_fill(&pb, 20, 23, 80, 24, pLightGray);

	// Now we have a simple image, so write it to a file
	int err = write_PPM("test_hline.ppm", &pb);
}

void test_hline_blend()
{
	size_t width = 320;
	size_t height = 240;
	pb_rgba pb;
	pb_rgba_init(&pb, width, height);


	// Set a white background
	raster_rgba_rect_fill(&pb, 0, 0, width, height, pWhite);



	// Draw some blended lines atop the whole
	for (size_t row = 35; row < 120; row++){
		raster_rgba_hline_blend(&pb, 45, row, 100, RGBA(0, 153, 153, 203));
	}

	// Now we have a simple image, so write it to a file
	int err = write_PPM("test_hline_blend.ppm", &pb);

}

void test_linespan()
{
	size_t width = 320;
	size_t height = 240;
	pb_rgba pb;
	pb_rgba_init(&pb, width, height);


	// Set a white background
	raster_rgba_rect_fill(&pb, 0, 0, width, height, pWhite);

	uint32_t span[120];

	int idx = 0;
	uint8_t c;

	// ramp up
	while (idx <= 60)
	{
		c = uint8_t((idx / 60.0f)*255);
		((pix_rgba*)span)[idx].r = c;
		((pix_rgba*)span)[idx].g = c;
		((pix_rgba*)span)[idx].b = c;
		((pix_rgba*)span)[idx].a = 255;

		idx++;
	}

	// ramp down
	idx = 61;
	while (idx < 120)
	{
		c = 255 - uint8_t((idx / 60.0f) * 255);
		((pix_rgba*)span)[idx].r = c;
		((pix_rgba*)span)[idx].g = c;
		((pix_rgba*)span)[idx].b = c;
		((pix_rgba*)span)[idx].a = 255;

		idx++;
	}


	for (int row = 20; row < 41; row++)
	{
		raster_rgba_span(&pb, 20, row, 120, span);
	}
/*
// do a horizontal fade
	int color1 = RGBA(127, 102, 0, 255);
	int color2 = RGBA(0, 127, 212, 255);

	// Fill a span with colors
	for (size_t row = 0; row < 100; row++) {
		int x1 = 0;
		int x2 = width - 1;
		raster_rgba_hline_fade(&pb, x1, color1, x2, color2, row);
	}

	// fade to black
	for (size_t col = 20; col < 100; col++) {
		raster_rgba_vline_fade(&pb, 46, pLightGray, 77, pBlack, col);
	}
*/

	// Now we have a simple image, so write it to a file
	int err = write_PPM("test_span.ppm", &pb);

}



int main(int argc, char **argv)
{
	test_hline();
	test_hline_blend();
	test_linespan();

	return 0;
}