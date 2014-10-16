#include "test_common.h"

void test_blit()
{
	size_t width = 640;
	size_t height = 480;

	pb_rgba pb;
	pb_rgba_init(&pb, width, height);

	pb_rgba fpb;
	pb_rgba_get_frame(&pb, 0, 0, width / 2, height / 2, &fpb);

	// draw into primary pixel buffer
	raster_rgba_rect_fill(&pb, 10, 10, (width / 2) - 20, (height / 2) - 20, pBlue);

	// draw the frame in another location
	raster_rgba_blit(&pb, width / 2, height / 2, &fpb);

	// Now we have a simple image, so write it to a file
	int err = write_PPM("test_blit.ppm", &pb);

}

int main(int argc, char **argv)
{
	test_blit();

	return 0;
}
