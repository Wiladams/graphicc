#include "test_common.h"

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

	mat4 scalemat;
	trans3d_scale(scalemat, 1.0 / 2.0f, 1.0 / 3.0f, 1.0f);

	size_t nrows = sizeof(pts1) / sizeof(REAL) / 4;

	trans3d_transform_rows(nrows, res, pts1, scalemat);

	print_rows(nrows, res);
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

	trans3d_map_to_window(screenx, screeny,
		clipx, clipy, clipw,
		winResx, winResy,
		winCenterx, winCentery);

	printf("  clip: %4.3f, %4.3f, %4.3f\n", clipx, clipy, clipw);
	printf("screen: %4.3f, %4.3f\n", screenx, screeny);
}

void test_model_view_transform()
{
	REAL res[32];

	real4 eye = {-10, 3, 2, 1};
	real4 lookAt = {0,0,0,1};
	real4 up = { 0, 1, 0, 1 };
	mat4 mviewmat;

	size_t nrows = sizeof(pts1) / sizeof(REAL) / 4;

	trans3d_lookat(mviewmat, eye, lookAt, up);

	trans3d_transform_rows(nrows, res, pts1, mviewmat);

	print_rows(nrows, res);
}

int main(int argc, char **argv)
{
	//test_scale();
	//test_screen_transform();
	test_model_view_transform();
}