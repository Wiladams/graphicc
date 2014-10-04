#include "linearalgebra.h"
#include "pixelbuffer.h"
#include "render_rgba.h"
#include "pbm.h"

#include <stdio.h>

void real3_write_array(const real3 c)
{
	printf("[%f, %f, %f]", c[0], c[1], c[2]);
}

void real3_print(const real3 c, const char *caption)
{
	printf("%s ==> ", caption);
	real3_write_array(c);
	printf("\n");
}

void test_arithmetic()
{
	real3 c;
	real3 a = { 1, 0, 1 };
	real3 b = { 0, 1, 0 };
	real3 u = { 1, 1, 1 };
	real3 ux = { 1, 0, 0 };
	real3 uy = { 0, 1, 0 };

	real3_add(c, a, b);
	real3_print(c, "c = a + b");

	real3_sub(c, a, b);
	real3_print(c, "c = a - b");

	real3_neg(c, a);
	real3_print(c, "c = -a");

	real3_mul_scalar(c, a, 5);
	real3_print(c, "c = a * 5");

	real3_div_scalar(c, c, 5);
	real3_print(c, "c = c / 5");

	printf("MAG: %f\n", real3_mag(u));
	printf("DOT: %f\n", real3_dot(u, u));

	real3_cross(c, ux, uy);
	real3_print(c, "c = ux cross uy");

	real3 newer = { 5, 10, 15 };
	real3_normalize(c, newer);
	real3_print(c, "c = newer.normalize()");
	printf("UMAG: %f\n", real3_mag(c));

	real3 p1 = { 1, 2, 0 };
	real3 p2 = { 4, 2, 0 };
	printf("DISTANCE: %f\n", real3_distance(p1, p2));
}

void test_writebitmap()
{
	size_t width = 320;
	size_t height = 240;
	pb_rgba fb;
	pb_rgba_init(&fb, width, height);

	unsigned int pRed = RGBA(255, 0, 0, 0);

	for (size_t row = 0; row < height; row++) {
		render_rgba_set_pixel(&fb, row, row, pRed);
	}

	// Now we have a simple image, so write it to a file
	int err = write_PPM("test.ppm", &fb);

}

int main(int argc, char **argv)
{
	//test_arithmetic();
	test_writebitmap();

	return 0;
}