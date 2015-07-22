#include "drawproc.h"

int *xvals;
int *yvals;
int *bvals;

void setup()
{
	size(640, 360);
	noSmooth();
	xvals = new int[width];
	yvals = new int[width];
	bvals = new int[width];
}

int arrayindex = 0;

void draw()
{
	background(RGBA(102,102,102,255));

	for (int i = 1; i < width; i++) {
		xvals[i - 1] = xvals[i];
		yvals[i - 1] = yvals[i];
		bvals[i - 1] = bvals[i];
	}
	// Add the new values to the end of the array 
	xvals[width - 1] = MAP(mouseX, 0, width-1, height/3, 0);
	yvals[width - 1] = MAP(mouseY, 0, height-1, height/3,  height/3 + (height/3)-1);
	if (isMousePressed) {
		bvals[width - 1] = 0;
	}
	else {
		bvals[width - 1] = 255;
	}

	fillGray(255);
	noStroke();
	rect(0, height / 3, width, height / 3 + 1);

	for (int i = 1; i<width; i++) {
		strokeGray(255);
		point(i, xvals[i]);

		strokeGray(0);
		point(i, yvals[i]);
		//point(i, height / 3 + yvals[i] / 3);

		strokeGray(255);
		line(i, 2 * height / 3 + bvals[i] / 3, i, (2 * height / 3 + bvals[i - 1] / 3));
	}
}
