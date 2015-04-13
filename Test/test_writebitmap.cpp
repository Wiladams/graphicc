

#include "test_common.h"

void test_writesinglepixel()
{
	pb_rgba pb;
	pb_rgba_init(&pb, 100, 3);

	uint32_t color = RGBA(127, 20, 30, 255);
	raster_rgba_hline(&pb, 0, 0, 100, RGBA(0xfe,0,0,255));
	raster_rgba_hline(&pb, 0, 1, 100, RGBA(0, 0xfe, 0, 255));
	raster_rgba_hline(&pb, 0, 2, 100, RGBA(0, 0, 0xfe, 255));

	int err = write_PPM_ascii("test_writesinglepixel_ascii.ppm", &pb);
	err = write_PPM_binary("test_writesinglepixel_binary.ppm", &pb);

}

void test_writebitmap()
{
	size_t width = 480;
	size_t height = 480;
	pb_rgba pb;
	pb_rgba_init(&pb, width, height);


	// draw horizontal lines top and bottom
	raster_rgba_hline(&pb, 0, 0, width - 1, pWhite);
	raster_rgba_hline(&pb, 0, height - 1, width - 1, pWhite);

	// draw vertical lines left and right
	raster_rgba_vline(&pb, 0, 0, height - 1, pGreen);
	raster_rgba_vline(&pb, width - 1, 0, height - 1, pTurquoise);

	// draw criss cross lines
	raster_rgba_line(&pb, 0, 0, width - 1, height - 1, pRed);
	raster_rgba_line(&pb, width - 1, 0, 0, height - 1, pYellow);

	// draw a couple of rectangles
	raster_rgba_rect_fill(&pb, 5, 5, 60, 60, pLightGray);
	raster_rgba_rect_fill(&pb, width - 65, height - 65, 60, 60, pLightGray);

	// draw a rectangle in the center
	pb_rgba fpb;
	pb_rgba_get_frame(&pb, (width / 2) - 100, (height / 2) - 100, 200, 200, &fpb);
	raster_rgba_rect_fill(&fpb, 0, 0, 200, 200, pBlue);

	// Draw triangle 
	raster_rgba_triangle_fill(&pb, 0, height - 10, 0, 10, (width / 2) - 10, height / 2, pGreen);

	// Now we have a simple image, so write it to a file
	int err = write_PPM("test_writebitmap.ppm", &pb);

}

int main(int argc, char **argv)
{
	//test_writebitmap();
	test_writesinglepixel();

	return 0;
}
