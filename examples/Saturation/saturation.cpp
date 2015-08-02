#include "drawproc.h"

int barWidth = 10;
int lastBar = -1;


void setup() {
	size(640, 360);
	colorMode(COLOR_MODE_HSB, width, height, 100);
	noStroke();
}


void draw() {
	int whichBar = mouseX / barWidth;
	if (whichBar != lastBar) {
		int barX = whichBar * barWidth;
		fill(barX, mouseY, 66);
		rect(barX, 0, barWidth, height);
		lastBar = whichBar;
	}
}