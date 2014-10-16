#include "test_common.h"

void checkerboard(pb_rgba *pb, const size_t cols, const size_t rows, const size_t width, const size_t height, int color1, int color2)
{
	size_t tilewidth = width / cols;
	size_t tileheight = (height / rows);

	for (size_t c = 0; c < cols; c++){
		for (size_t r = 0; r < rows; r++){
			raster_rgba_rect_fill(pb, c*tilewidth, r*tileheight, tilewidth / 2, tileheight / 2, color1);
			raster_rgba_rect_fill(pb, (c*tilewidth) + tilewidth / 2, r*tileheight, tilewidth / 2, tileheight / 2, color2);
			raster_rgba_rect_fill(pb, c*tilewidth, (r*tileheight) + tileheight / 2, tilewidth / 2, tileheight / 2, color2);
			raster_rgba_rect_fill(pb, (c*tilewidth) + tilewidth / 2, (r*tileheight) + tileheight / 2, tilewidth / 2, tileheight / 2, color1);
		}
	}
}

void test_blender()
{
	size_t width = 800;
	size_t height = 600;

	pb_rgba pb;
	pb_rgba_init(&pb, width, height);

	// Red background
	raster_rgba_rect_fill(&pb, 0, 0, width, height, pRed);

	// create checkerboard background
	checkerboard(&pb, 16, 16, width, height, pLightGray, pYellow);

	// Draw some blended rectangles atop the whole
	for (int offset = 10; offset < 400; offset += 40) {
		float factor = offset / 400.0f;
		int alpha = (int)(factor * 255);
		//printf("factor: %f alpha: %d\n", factor, alpha);

		int fgColor = RGBA(0, 255, 255, alpha);
		raster_rgba_rect_fill_blend(&pb, offset, offset, 100, 100, fgColor);
	}


	// Now we have a simple image, so write it to a file
	int err = write_PPM("test_blender.ppm", &pb);
}

int main(int argc, char **argv)
{
	test_blender();

	return 0;
}
