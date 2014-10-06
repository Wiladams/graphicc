#include "linearalgebra.h"
#include "pixelbuffer.h"
#include "raster_rgba.h"
#include "pbm.h"

#include <stdio.h>

static unsigned int pRed = RGBA(255, 0, 0, 255);
static unsigned int pGreen = RGBA(0, 255, 0, 255);
static unsigned int pBlue = RGBA(0, 0, 255, 255);

static unsigned int pWhite = RGBA(255, 255, 255, 255);
static unsigned int pYellow = RGBA(255, 255, 0, 255);
static unsigned int pTurquoise = RGBA(0, 255, 255, 255);
static unsigned int pLightGray = RGBA(163, 163, 163, 255);


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

void test_pixelvalues()
{
	int value = RGBA(230, 63, 127, 255);

	printf("RED: %d (230)\tGREEN: %d (63)\tBLUE: %d (127)\tALPHA: %d (255)\n",
		GET_R(value), GET_G(value), GET_B(value), GET_A(value));
}

void test_drawpixels()
{
	size_t width = 256;
	size_t height = 256;
	pb_rgba pb;
	pb_rgba_init(&pb, width, height);

	for (size_t row = 0; row < height; row++) {
		for (size_t col = 0; col < width; col++) {
			int value = RGBA(row, col, 255, 255);
			//((unsigned int *)(&pb)->data)[(row*(&pb)->width) + col] = value;
			pb_rgba_set_pixel(&pb, col, row, value);
		}
	}

	// Now we have a simple image, so write it to a file
	int err = write_PPM("test_drawpixel.ppm", &pb);

}

void test_writebitmap()
{
	size_t width = 480;
	size_t height = 480;
	pb_rgba pb;
	pb_rgba_init(&pb, width, height);


	// draw horizontal lines top and bottom
	raster_rgba_hline(&pb, 0, 0, width-1, pWhite);
	raster_rgba_hline(&pb, 0, height - 1, width - 1, pWhite);
	
	// draw vertical lines left and right
	raster_rgba_vline(&pb, 0, 0, height - 1, pGreen);
	raster_rgba_vline(&pb, width - 1, 0, height - 1, pTurquoise);

	// draw criss cross lines
	raster_rgba_line(&pb, 0, 0, width - 1, height - 1, pRed);
	raster_rgba_line(&pb, width - 1, 0, 0, height - 1, pYellow);

	// draw a couple of rectangles
	raster_rgba_rect_fill(&pb, 5, 5, 60, 60, pLightGray);
	raster_rgba_rect_fill(&pb, width - 65, height - 65, 60, 60, pLightGray);

	// draw a rectangle in the center
	pb_rgba fpb;
	pb_rgba_get_frame(&pb, (width / 2) - 100, (height / 2) - 100, 200, 200, &fpb);
	raster_rgba_rect_fill(&fpb, 0, 0, 200, 200, pBlue);

	// Now we have a simple image, so write it to a file
	int err = write_PPM("test.ppm", &pb);

}

void test_blit()
{
	size_t width = 640;
	size_t height = 480;

	pb_rgba pb;
	pb_rgba_init(&pb, width, height);

	pb_rgba fpb;
	pb_rgba_get_frame(&pb, 0, 0, width/2, height/2, &fpb);

	// draw into primary pixel buffer
	raster_rgba_rect_fill(&pb, 10, 10, (width/2) - 20, (height/2) - 20, pBlue);

	// draw the frame in another location
	raster_rgba_blit(&pb, width / 2, height / 2, &fpb);

	// Now we have a simple image, so write it to a file
	int err = write_PPM("test_blit.ppm", &pb);

}

int main(int argc, char **argv)
{
	//test_arithmetic();
	//test_writebitmap();
	//test_drawpixels();
	//test_pixelvalues();
	test_blit();

	return 0;
}