

#include "animwin32.h"
#include "linearalgebra.h"
#include <math.h>

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
void setup()
{
	size(640, 480);

	float zero = MAP(0.0f, 0.0f, 1.0f, 639, 0);
	float half = MAP(0.5f, 0.0f, 1.0f, 639, 0);
	float full = MAP(1.0f, 0.0f, 1.0f, 639, 0);

}

static int barheight = 300;
static int barwidth = 64;
static int bargap = 10;

static double maxsecs1 = 1.0 /1.0;
static double maxsecs2 = 1.0 / 2.0;

extern "C"
void step(pb_rgba *pb)
{
	// create checkerboard background
	//checkerboard(pb, 8, 8, width, height, pBlack, pWhite);


	double secfrag;

	secfrag = fmod(seconds(), maxsecs1);
	barheight = MAP(secfrag, 0, maxsecs1, 4, height - 1);
	raster_rgba_rect_fill(pb, bargap, height - barheight, barwidth, barheight, pGreen);

	// draw the bar
	secfrag = fmod(seconds(), maxsecs2);
	barheight = MAP(secfrag, 0, maxsecs2, 4, height - 1);
	raster_rgba_rect_fill(pb, bargap + barwidth + bargap, height - barheight, barwidth, barheight, pRed);
}



