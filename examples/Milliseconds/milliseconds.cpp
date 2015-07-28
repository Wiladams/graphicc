#include "drawproc.h"

float scale;

void setup() {
	size(640, 360);
	noStroke();
	scale = width / 20.0f;
}

void draw() {
	for (int i = 0; i < scale; i++) {
		colorMode(COLOR_MODE_RGB, (i + 1) * scale * 10);
		fill(millis() % (int)((i + 1) * scale * 10));
		rect(i*scale, 0, scale, height);
	}
}
