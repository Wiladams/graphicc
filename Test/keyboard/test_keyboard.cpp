/*
test_keyboard

Do some simple mouse and keyboard tracking

*/
#include "drawproc.h"
#include "guistyle.h"
#include <stdio.h>

static GUIStyle styler;
static const int gMaxMode = 3;
static int gMode = 0;
static const int gridSize = 8;

static bool gShowKeyRect = false;
static bool showGrid = false;
static bool gShowCrosshair = true;

PImage *kbImage;
pb_rgba fb;
pb_rect keyRect = { 0, 0, 34, 34 };

void  mousePressed()
{
	gMode++;
	if (gMode >= gMaxMode) {
		gMode = 0;
	}
}

void  mouseMoved()
{
	keyRect.x = mouseX - keyRect.width / 2;
	keyRect.y = mouseY - keyRect.height / 2;
}

void  keyReleased()
{
	switch (keyCode)
	{
	case KC_SPACE:
		write_PPM_binary("test_keyboard.ppm", gpb);
		break;

	case KC_F1:
		showGrid = !showGrid;
		break;
	case KC_F2:
		gShowKeyRect = !gShowKeyRect;
		break;
	case KC_F3:
		gShowCrosshair = !gShowCrosshair;
		break;

	case KC_RIGHT: // increase width of keyRect
		keyRect.width += 1;
		break;

	case KC_LEFT:
		keyRect.width -= 1;
		if (keyRect.width < 4) keyRect.width = 4;
		break;
	case KC_UP:
		keyRect.height += 1;
		break;
	case KC_DOWN:
		keyRect.height -= 1;
		if (keyRect.height < 4) keyRect.height = 4;
		break;
	}

	keyRect.x = mouseX - keyRect.width / 2;
	keyRect.y = mouseY - keyRect.height / 2;

}

void drawCrossHairs()
{
	if (!gShowCrosshair) {
		return;
	}

	stroke(84);

	// horizontal line
	line(0, mouseY, width - 1, mouseY);
	line(mouseX, 0, mouseX, height - 1);
}

void drawGrid()
{
	if (!showGrid) {
		return;
	}

	stroke(132, 86);
	for (int x = 0; x < width; x++)
	{
		if (x % gridSize == 0) {
			line(x, 0, x, height - 1);
		}
	}

	for (int y = 0; y < height; y++)
	{
		if (y % gridSize == 0) {
			line(0, y, width-1, y);
		}
	}
}
// Draw information about the mouse
// location, buttons pressed, etc
void drawMouseInfo()
{
	// draw a white banner across the bottom
	noStroke();
	fill(pWhite);
	rect(0, height - 1 - 34, width, 34);

	if (gShowKeyRect) {
		// draw the key rectangle
		fill(COLOR(255, 238, 200, 180));
		stroke(pDarkGray);
		rect(keyRect.x, keyRect.y, keyRect.width, keyRect.height);
	}

	// select verdana font
	setFont(verdana17);
	char infobuff[256];
	sprintf_s(infobuff, "Mouse X: %3d Y: %3d    Key: (%3f, %3f)(%3.0f, %3.0f)  Seconds: %3.2f", mouseX, mouseY, 
		keyRect.x, keyRect.y, keyRect.width, keyRect.height,
		seconds());
	fill(pBlack);
	textAlign(TX_LEFT, TX_TOP);
	text(infobuff, 0, height-34);

}

void draw()
{
	background(pLightGray);
	image(kbImage, 0, 0);

	drawCrossHairs();
	drawGrid();

	drawMouseInfo();
}

void setup()
{
	kbImage = loadImage("c:/repos/graphicc/Test/windows-keyboard-60-keys.ppm", nullptr);
	
	size(kbImage->fb.frame.width, kbImage->fb.frame.height+4+30);
	background(pLightGray);
}


