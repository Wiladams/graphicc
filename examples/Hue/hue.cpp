#include "drawproc.h"

int barWidth = 10;
int lastBar = -1;

void setup()
{
	size(640, 360);
	colorMode(COLOR_MODE_HSB, height, height, height);
	noStroke();
	background(height);
}

void draw()
{
	int whichBar = mouseX / barWidth;
	if (whichBar != lastBar) {
		int barX = whichBar * barWidth;
		fill(mouseY, height, height);
		rect(barX, 0, barWidth, height);
		lastBar = whichBar;
	}
}
