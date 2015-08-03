#include "drawproc.h"

int drawMode = 1;
static const int lastMode = 3;

void mouseReleased()
{
	drawMode += 1;
	if (drawMode > lastMode) {
		drawMode = 1;
	}

	draw();
	redraw();	// since we're not in a loop, we must force a redraw()
}


void setup()
{
	size(200, 200);

	noLoop();
}

void drawRGB()
{
	noStroke();
	colorMode(COLOR_MODE_RGB, width);
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			stroke(i, j, 0);
			point(i, j);
		}
	}
}

void drawHSB()
{
	noStroke();
	colorMode(COLOR_MODE_HSB, width);
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			stroke(i, j, width);
			point(i, j);
		}
	}
}

void drawTeal()
{
	colorMode(COLOR_MODE_HSB, 360, 100, 100);

	background(180, 50, 50);
}

void draw()
{
	background(0);

	switch (drawMode) {
		case 1:
			drawRGB();
		break;

		case 2:
			drawHSB();
		break;

		case 3:
			drawTeal();
		break;
	}

}
