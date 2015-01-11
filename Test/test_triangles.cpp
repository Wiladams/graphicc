

#include "test_common.h"

void test_writebitmap()
{
	size_t width = 480;
	size_t height = 480;
	pb_rgba pb;
	pb_rgba_init(&pb, width, height);


	// Draw some triangles 
	size_t midx = (size_t)(width / 2);
	size_t midy = (size_t)(height / 2);

	raster_rgba_triangle_fill(&pb, 0, 0, width-1, 0, midx, midy, pRed);
	raster_rgba_triangle_fill(&pb, 0, 0, midx, midy, 0, height-1, pGreen);
	raster_rgba_triangle_fill(&pb, width-1, 0, midx, midy, width-1, height - 1, pBlue);
	raster_rgba_triangle_fill(&pb, midx, midy, 0, height-1, width - 1, height - 1, pDarkGray);

	// Now we have a simple image, so write it to a file
	int err = write_PPM("test_triangle.ppm", &pb);

}

int main(int argc, char **argv)
{
	test_writebitmap();

	return 0;
}
