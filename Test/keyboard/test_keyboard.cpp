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

static bool gShowKeyRect = true;
static bool showGrid = false;
static bool gShowCrosshair = true;

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
	case VK_SPACE:
		write_PPM_binary("test_keyboard.ppm", gpb);
		break;

	case VK_F1:
		showGrid = !showGrid;
		break;
	case VK_F2:
		gShowKeyRect = !gShowKeyRect;
		break;
	case VK_F3:
		gShowCrosshair = !gShowCrosshair;
		break;

	case VK_RIGHT: // increase width of keyRect
		keyRect.width += 1;
		break;

	case VK_LEFT:
		keyRect.width -= 1;
		if (keyRect.width < 4) keyRect.width = 4;
		break;
	case VK_UP:
		keyRect.height += 1;
		break;
	case VK_DOWN:
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
	fillRGBA(pWhite);
	rect(0, fb.frame.height + 2, width, 24);

	if (gShowKeyRect) {
		// draw the key rectangle
		fillRGBA(RGBA(255, 238, 200, 180));
		stroke(pDarkGray);
		rect(keyRect.x, keyRect.y, keyRect.width, keyRect.height);
	}

	// select verdana font
	setFont(verdana17);
	char infobuff[256];
	sprintf_s(infobuff, "Mouse X: %3d Y: %3d    Key: (%3f, %3f)(%3.0f, %3.0f)  Seconds: %3.2f", mouseX, mouseY, 
		keyRect.x, keyRect.y, keyRect.width, keyRect.height,
		seconds());
	fillRGBA(pBlack);
	textAlign(TX_LEFT, TX_TOP);
	text(infobuff, 0, fb.frame.height + 2);

}

void draw()
{
	backgroundRGBA(pLightGray);
	backgroundImage(&fb);
	drawCrossHairs();
	drawGrid();

	drawMouseInfo();
}

void setup()
{
	int ret = PPM_read_binary("c:/repos/graphicc/Test/windows-keyboard-60-keys.ppm", &fb);
//	int ret = PPM_read_binary("..\\Test\\windows-keyboard-60-keys.ppm", &fb);
	
	size(fb.frame.width+4, fb.frame.height+4+30);
	backgroundRGBA(pLightGray);
}


