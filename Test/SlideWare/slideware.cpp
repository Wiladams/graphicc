/*
Drawing Shapes on a click
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
	size(640, 480);
	background(pLightGray);
	noLoop();

	setOnMousePressedHandler(mousePressed);
}

void draw()
{

}
