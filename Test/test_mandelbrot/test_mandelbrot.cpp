/*

References:
	http://warp.povusers.org/Mandelbrot/

	Runs from -2 - 1 in the x direction
	Runs from -1.2 => 1.2 in the y direction
*/

#include "drawproc.h"
//#include "linearalgebra.h"
#include <math.h>

// Mandelbrot parameters
double MinRe = -2.0;	// -2.0
double MaxRe = 1.0;		// 1.0
double MinIm = -1.2;
double MaxIm = MinIm;

static unsigned int MaxIterations = 50;

// Mouse box parameters
static int mWidth = 64;
static int mHeight = 64;



void drawMouse()
{	
	rectMode(CENTER);
	stroke(pBlack);
	fill(RGBA(0, 127, 255, 127));
	rect(mouseX, mouseY, mWidth, mHeight);
}

void drawMandelbrot()
{
	// Parameters for mandelbrot drawing
	MaxIm = MinIm + (MaxRe - MinRe)*height / width;
	double Re_factor = (MaxRe - MinRe) / (width - 1);
	double Im_factor = (MaxIm - MinIm) / (height - 1);	// How much to increment by row

	for (unsigned y = 0; y<height; ++y)
	{
		double c_im = MaxIm - y*Im_factor;
		for (unsigned x = 0; x<width; ++x)
		{
			double c_re = MinRe + x*Re_factor;

			double Z_re = c_re;
			double Z_im = c_im;

			bool isInside = true;
			unsigned int n;
			for (n = 0; n<MaxIterations; ++n)
			{
				double Z_re2 = Z_re*Z_re, Z_im2 = Z_im*Z_im;
				if (Z_re2 + Z_im2 > 4)
				{
					isInside = false;
					break;
				}
				Z_im = 2 * Z_re*Z_im + c_im;
				Z_re = Z_re2 - Z_im2 + c_re;
			}
			if (isInside) {
				stroke(pBlack);
				point(x, y);
			}
			else{
				int rc = MAP(n, 0, MaxIterations- 1, 0, 255);

				stroke(RGBA(rc, 0, 0, 255));
				point(x, y);
			}
		}
	}
}

void CALLBACK mouseDown()
{
	// calculate the rectangle boundaries

	int x1 = mouseX - mWidth / 2;
	int y1 = mouseY - mHeight / 2;
	int rwidth = mWidth;
	int rheight = mHeight;

	pb_rect crect;
	pb_rect_intersection(crect, pixelFrame, { x1, y1, rwidth, rheight });

	// map that to the real and imaginary parts of the image
	MinRe = MAP(crect.x, 0, width - 1, MinRe, MaxRe);
	MaxRe = MAP(crect.x + crect.x + crect.width, 0, width - 1, MinRe, MaxRe);

	MinIm = MAP(crect.y, 0, height - 1, MinIm, MaxIm);

}

LRESULT CALLBACK mouseWheel(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int fwKeys = GET_KEYSTATE_WPARAM(wParam);
	int zDelta = GET_WHEEL_DELTA_WPARAM(wParam);

	// zDelta > 0 ==> away from user 
	if (zDelta > 0) {
		// zoom in
		MinRe += 0.10;
		MaxRe -= 0.10;
	} else {
		// zDelta < 0 ==> towards user
		MinRe -= 0.1;
		MaxRe += 0.1;
	}


	return 0;
}

extern "C"
void setup()
{
	size(1024, 768);
	background(pLightGray);

	setOnMouseWheelHandler(mouseWheel);
	//setOnMouseDownHandler(mouseDown);
	setOnMousePressedHandler(mouseDown);
}

extern "C"
void draw()
{
	drawMandelbrot();
	drawMouse();
}