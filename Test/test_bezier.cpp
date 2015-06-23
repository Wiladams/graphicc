#include "test_common.h"
#include "bezier.h"


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

	// needed for all curves
	int m = 100;
	Pt3 curve[200];

	// One loop
	int nctrls = 5;
	Pt3 ctrls1[5] = { { centerx - xsize, centery, 0 }, { centerx, centery - ysize, 0 }, { centerx + xsize, centery, 0 }, { centerx, centery + ysize, 0 }, { centerx - xsize, centery, 0 } };
	bez3_curve(ctrls1, nctrls, m, curve);
	polyline(&pb, curve, m, pBlack);


	// one double peak through the middle
	Pt3 ctrls3[5] = { { centerx - xsize, centery, 0 }, { centerx - (xsize*0.3f), centery + ysize, 0 }, { centerx, centery - ysize, 0 }, { centerx + (xsize*0.3f), centery + ysize, 0 }, { centerx + xsize, centery, 0 } };
	bezier(ctrls3, nctrls, m, curve);
	polyline(&pb, curve, m, pRed);

	// Now we have a simple image, so write it to a file
	int err = write_PPM("test_bezier.ppm", &pb);
}

void test_bezier4()
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


	// Now we have a simple image, so write it to a file
	int err = write_PPM("test_bezier4.ppm", &pb);
}

int main(int argc, char **argv)
{
	test_bezier();
	test_bezier4();

	return 0;
}

