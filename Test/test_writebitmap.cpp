/*
#include "linearalgebra.h"
#include "pixelbuffer.h"
#include "raster_rgba.h"
#include "pbm.h"

#include <stdio.h>

static unsigned int pRed = RGBA(255, 0, 0, 255);
static unsigned int pGreen = RGBA(0, 255, 0, 255);
static unsigned int pBlue = RGBA(0, 0, 255, 255);

static unsigned int pBlack = RGBA(0, 0, 0, 255);
static unsigned int pWhite = RGBA(255, 255, 255, 255);
static unsigned int pYellow = RGBA(255, 255, 0, 255);
static unsigned int pTurquoise = RGBA(0, 255, 255, 255);
static unsigned int pDarkGray = RGBA(93, 93, 93, 255);
static unsigned int pLightGray = RGBA(163, 163, 163, 255); 
*/

#include "test_common.h"

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
	test_writebitmap();

	return 0;
}
