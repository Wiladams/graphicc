/*
Drawing Shapes on a click

This code tests the noLoop() function of the drawproc routines.
The draw() function should only be called once, at the very beginning.  Nothing 
is done in this draw routine.

All drawing is driven by the mousePressed() routine.  In this case, we
cycle through a series of simple drawings.  Each time the mouse is pressed
a shape is drawn, and the redraw() function is called to force the image
to be updated.
*/
#include "drawproc.h"

static const int gMaxMode = 3;
static int gMode = 0;

void drawShape();


void drawShape()
{
	rectMode(CENTER);
	fill(RGBA(random(255), random(255), random(255), 255));
	background(pLightGray);

	switch (gMode)
	{
	case 0:
		triangle(width / 2, 0, width - 1, height - 1, 0, height - 1);
		break;

	case 1:
		quad(width / 2, 0, width - 1, height / 2, width / 2, height - 1, 0, height / 2);
		break;

	case 2: {
				static coord polyverts[] = {
					(width / 2) - 100, 10,
					10, height / 2,
					(width / 2) + 100, (height / 2),
					(width / 2) + 100, 10,
				};
				static int nverts = 4;
				raster_rgba_convex_polygon_fill(gpb, polyverts, nverts, pixelFrame, pYellow);
	} break;

	default:
		rect(width / 2, height / 2, width / 2, height / 2);
		break;
	}

	redraw();
}

void CALLBACK mousePressed()
{
	gMode++;
	if (gMode >= gMaxMode) {
		gMode = 0;
	}

	drawShape();
}


void setup()
{
	size(1024, 768);
	background(pLightGray);
	noLoop();

	setOnMousePressedHandler(mousePressed);
}

void draw()
{
}
