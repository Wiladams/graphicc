/*

References:
	http://warp.povusers.org/Mandelbrot/

	Runs from -2 - 1 in the x direction
	Runs from 
*/

#include "animwin32.h"
#include "linearalgebra.h"
#include <math.h>

double MinRe = -2.0;	// -2.0
double MaxRe = 1.0;		// 1.0
double MinIm = -1.2;


static unsigned int MaxIterations = 60;



void drawMouse()
{
	int mWidth = 64;
	int mHeight = 64;

	// calculate the rectangle boundaries
	int x1 = mouseX, y1 = mouseY;
	int rwidth = 0, rheight = 0;

	x1 = mouseX - mWidth / 2;
	y1 = mouseY - mHeight / 2;
	rwidth = mWidth;
	rheight = mHeight;

	pb_rect crect;
	pb_rect_intersection(crect, pixelFrame, { x1, y1, rwidth, rheight });
	
	// map that to the real and imaginary parts of the image
	
	rectMode(CENTER);
	stroke(pBlack);
	fill(RGBA(0, 127, 255, 127));
	rect(mouseX, mouseY, mWidth, mHeight);
}

void drawMandelbrot()
{

	// Parameters for mandelbrot drawing
	double MaxIm = MinIm + (MaxRe - MinRe)*height / width;
	double Re_factor = (MaxRe - MinRe) / (width - 1);
	double Im_factor = (MaxIm - MinIm) / (height - 1);

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

int OnMouseDown(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	// use the mouse rectangle to zoom
}

int OnMouseWheel(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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
	size(640, 480);
	background(pLightGray);

	setOnMouseWheelHandler(OnMouseWheel);
}

extern "C"
void draw()
{
	drawMandelbrot();
	drawMouse();
}