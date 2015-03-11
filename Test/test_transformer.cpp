#include "test_common.h"
#include "pipeline_opengl.h"

REAL pts1[] = {
	0,0,1,1,
	2,0,1,1,
	2,3,1,1,
	0,3,1,1,
	0,0,0,1,
	2,0,0,1,
	2,3,0,1,
	0,3,0,1
};

void realn_write_array(const size_t nelems, const REAL *c)
{
	printf("[");
	for (size_t idx = 0; idx < nelems; idx++)
	{
		printf("%f", c[idx]);
		if (idx < nelems - 1) {
			printf(", ");
		}
	}
	printf("]");
}



void print_rows(const size_t nrows, const REAL *res)
{
	for (size_t idx = 0; idx < nrows; idx++)
	{
		realn_write_array(4, &res[idx * 4]);
		printf("\n");
	}
}

void test_scale()
{
	REAL res[32];
	real3 sxyz = { 1.0 / 2.0f, 1.0 / 3.0f, 1.0f };

	mat4 scalemat;
	ogl_scale(scalemat, sxyz);

	size_t nrows = sizeof(pts1) / sizeof(REAL) / 4;

	ogl_transform_rows(nrows, res, pts1, scalemat);

	print_rows(nrows, res);
}

void test_translate()
{
	real4 pt1 = { 1, 2, 3, 1 };
	real4 c;
	mat4 m;

	puts("==== test_translate ====");

	ogl_translate(m, -2, 2, 4);
	row4_mul_mat4(c, pt1, m);

	realn_write_array(4, c);
}

void test_screen_transform()
{
	// x := -1 (left), 1 (right)
	// y := -1 (bottom), 1 (top)
	//
	REAL screenx;
	REAL screeny;
	REAL clipx = 1.0f;
	REAL clipy = 0.0f;
	REAL clipw = 1;
	REAL winResx = 1280;
	REAL winResy = 1024;
	REAL winCenterx = 640;
	REAL winCentery = 512;

	// setup the pixel buffer
	pb_rgba pb;
	pb_rgba_init(&pb, winResx, winResy);
	raster_rgba_rect_fill(&pb, 0, 0, winResx, winResy, cornsilk);

	// Do some mapping
	// draw four rectangles in the given space
	REAL centerX = winResx / 4;
	REAL centerY = winResy / 4;
	REAL rectWidth = winResx / 4;
	REAL rectHeight = winResy / 4;

	ogl_map_to_window(screenx, screeny, -1, 1, clipw, winResx, winResy, centerX,centerY);
	raster_rgba_rect_fill(&pb, screenx, screeny, rectWidth, rectHeight, pRed);
	raster_rgba_rect_fill(&pb, screenx+rectWidth, screeny, rectWidth, rectHeight, pGreen);
	raster_rgba_rect_fill(&pb, screenx, screeny+rectHeight, rectWidth, rectHeight, pBlue);
	raster_rgba_rect_fill(&pb, screenx + rectWidth, screeny + rectHeight, rectWidth, rectHeight, pDarkGray);

/*
	ogl_map_to_window(screenx, screeny, 0, 1, clipw, winResx, winResy, centerX, centerY);
	raster_rgba_rect_fill(&pb, screenx, screeny, rectWidth, rectHeight, pGreen);

	ogl_map_to_window(screenx, screeny, -1, 0, clipw, winResx, winResy, centerX, centerY);
	raster_rgba_rect_fill(&pb, screenx, screeny, rectWidth, rectHeight, pBlue);

	ogl_map_to_window(screenx, screeny, 0, 0, clipw, winResx, winResy, centerX, centerY);
	raster_rgba_rect_fill(&pb, screenx, screeny, rectWidth, rectHeight, pDarkGray);
*/
	//printf("  clip: %4.3f, %4.3f, %4.3f\n", clipx, clipy, clipw);
	//printf("screen: %4.3f, %4.3f\n", screenx, screeny);

	// write color buffer to file
	int err = write_PPM("test_screentransform.ppm", &pb);
}

void test_model_view_transform()
{
	REAL res[32];

	real4 eye = {-10, 3, 2, 1};
	real4 lookAt = {0,0,0,1};
	real4 up = { 0, 1, 0, 1 };
	mat4 mviewmat;

	size_t nrows = sizeof(pts1) / sizeof(REAL) / 4;

	ogl_lookat(mviewmat, eye, lookAt, up);

	ogl_transform_rows(nrows, res, pts1, mviewmat);

	print_rows(nrows, res);
}

int main(int argc, char **argv)
{
	//test_scale();
	test_screen_transform();
	//test_model_view_transform();
	//test_translate();
}