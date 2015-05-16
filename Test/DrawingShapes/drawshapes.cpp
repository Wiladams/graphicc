/*
	Drawing Shapes
*/
#include "drawproc.h"

float radiusX, radiusY;
int screenFadeValue = 20;
int sizeMax = 30;



void fadeScreen()
{
	fill(RGBA(130, 130, 240, screenFadeValue));
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

void CALLBACK mousePressed()
{
	fadeScreen();
	radiusX = random(sizeMax);
	radiusY = random(sizeMax);

	fill(RGBA(random(255), random(255), random(255), 255));

}

void CALLBACK mouseMoved()
{
	paintShapes(false);
}

LRESULT CALLBACK mouseDragged(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	paintShapes(true);
	return 0;
}

void setup()
{
	size(400, 400);
	background(RGBA(130, 130, 240, 255));
	noFill();

	setOnMousePressedHandler(mousePressed);
	setOnMouseDraggedHandler(mouseDragged);
	setOnMouseMovedHandler(mouseMoved);
}

void draw()
{

}
