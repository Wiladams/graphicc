#include "test_common.h"

void test_drawpixels()
{
	size_t width = 256;
	size_t height = 256;
	pb_rgba pb;
	pb_rgba_init(&pb, width, height);

	for (size_t row = 0; row < height; row++) {
		for (size_t col = 0; col < width; col++) {
			int value = RGBA(row, col, 255, 255);
			pb_rgba_set_pixel(&pb, col, row, value);
		}
	}

	// Now we have a simple image, so write it to a file
	int err = write_PPM("test_drawpixel.ppm", &pb);

}

int main(int argc, char **argv)
{
	test_drawpixels();

	return 0;
}
