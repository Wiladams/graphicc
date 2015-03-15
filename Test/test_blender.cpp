#include "test_common.h"
#include "bezier.h"

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
	size_t width = 640;
	size_t height = 480;

	pb_rgba pb;
	pb_rgba_init(&pb, width, height);

	// Red background
	raster_rgba_rect_fill(&pb, 0, 0, width, height, pRed);

	// create checkerboard background
	checkerboard(&pb, 8, 8, width, height, pBlack, pWhite);

	// Draw some blended rectangles atop the whole
	for (int iter = 1; iter <= 9; iter++) {
		float alpha = MAP(iter, 1, 9, 0, 255);
		printf("iteration: %d alpha: %f\n", iter, alpha);

		int fgColor = RGBA(0, 255, 255, (int)alpha);
		int offset = iter * 40;
		raster_rgba_rect_fill_blend(&pb, offset, offset, 100, 100, fgColor);
	}


	// Now we have a simple image, so write it to a file
	int err = write_PPM("test_blender.ppm", &pb);
}

void polyline(pb_rgba *pb, Pt3 *curve, const int nPts, int color)
{
	for (int idx = 0; idx < nPts; idx++) {
		raster_rgba_line(pb, curve[idx].x, curve[idx].y, curve[idx + 1].x, curve[idx + 1].y, color);
	}
}

void test_bezier()
{

	size_t width = 400;
	size_t height = 400;
	int centerx = width / 2;
	int centery = height / 2;
	int xsize = (int)(centerx*0.9);
	int ysize = (int)(centery*0.9);

	pb_rgba pb;
	pb_rgba_init(&pb, width, height);

	// background color
	raster_rgba_rect_fill(&pb, 0, 0, width, height, cornsilk);

	// One curve drooping down
	Pt3 controls[4] = { { centerx - xsize, centery, 0 }, { centerx, centery + ysize, 0 }, { centerx, centery + ysize, 0 }, { centerx + xsize, centery, 0 } };
	int nControls = 4;
	int m = 100;
	Pt3 curve[200];
	bezier4(controls, m, curve);
	polyline(&pb, curve, m, pGreen);

	// Several curves going up
	for (int offset = 0; offset < ysize; offset += 5) {
		Pt3 ctrls2[4] = { { centerx - xsize, centery, 0 }, { centerx, centery - offset, 0 }, { centerx, centery - offset, 0 }, { centerx + xsize, centery, 0 } };
		//bezier(ctrls2, nControls, m, curve);
		bezier4(ctrls2, m, curve);
		polyline(&pb, curve, m, pBlue);
	}

	// One loop
	int nctrls = 5;
	Pt3 ctrls1[5] = { { centerx - xsize, centery, 0 }, { centerx, centery - ysize, 0 }, { centerx + xsize, centery, 0 }, { centerx, centery + ysize, 0 }, { centerx - xsize, centery, 0 } };
	bezier(ctrls1, nctrls, m, curve);
	polyline(&pb, curve, m, pBlack);


	// one double peak through the middle
	Pt3 ctrls3[5] = { { centerx - xsize, centery, 0 }, { centerx-(xsize*0.3f), centery + ysize, 0 }, { centerx, centery - ysize, 0 }, { centerx+(xsize*0.3f), centery + ysize, 0 }, { centerx + xsize, centery, 0 } };
	bezier(ctrls3, nctrls, m, curve);
	polyline(&pb, curve, m, pRed);

	// Now we have a simple image, so write it to a file
	int err = write_PPM("test_bezier.ppm", &pb);
}

void test_factorial()
{
	printf("factorial (7): %d\n", intfactorial(7));
}

int main(int argc, char **argv)
{
	test_blender();
//	test_bezier();
//	test_factorial();

	return 0;
}
