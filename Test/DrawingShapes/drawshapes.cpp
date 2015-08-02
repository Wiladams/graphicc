/*
	Drawing Shapes
*/
#include "drawproc.h"

float radiusX, radiusY;
int screenFadeValue = 20;
int sizeMax = 30;

void fadeScreen()
{
	fill(130, 130, 240, screenFadeValue);
	noStroke();
	rect(0, 0, width, height);
}

void paintShapes(bool isDragged)
{
	if (isDragged) {
		noStroke();
		fill(pWhite);
		ellipse(mouseX, mouseY, radiusX, radiusY);
	}
	else {
		noFill();
		stroke((uint8_t)random(255));
		rect(mouseX, mouseY, random(sizeMax), random(sizeMax));
	}
}

void mousePressed()
{
	fadeScreen();
	radiusX = random(sizeMax);
	radiusY = random(sizeMax);

	fill(random(255), random(255), random(255), 255);

}

void mouseMoved()
{
	paintShapes(false);
}

void mouseDragged()
{
	paintShapes(true);
}

void setup()
{
	size(400, 400);
	background(130, 130, 240, 255);
	noFill();

}

