

#include "animwin32.h"
#include "linearalgebra.h"

static const size_t gwidth = 640;
static const size_t gheight = 480;


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


extern "C"
void setup(size_t &width, size_t &height)
{
	width = gwidth;
	height = gheight;
}

extern "C"
void step(pb_rgba *pb)
{
	// create checkerboard background
	checkerboard(pb, 8, 8, gwidth, gheight, pBlack, pWhite);
}



