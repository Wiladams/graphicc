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

// Draw information about the mouse
// location, buttons pressed, etc
void drawMouseInfo()
{
	// draw a white banner across the bottom
	noStroke();
	fill(pWhite);
	rect(0, fb.frame.height + 2, width, 24);


	// draw the key rectangle
	fill(RGBA(255, 238, 200, 180));
	stroke(pDarkGray);
	rect(keyRect.x, keyRect.y, keyRect.width, keyRect.height);

	// select verdana font
	setFont(verdana17);
	char infobuff[256];
	sprintf_s(infobuff, "Mouse X: %3d Y: %3d    Key: (%3f, %3f)(%3.0f, %3.0f)", mouseX, mouseY, keyRect.x, 
		keyRect.y, keyRect.width, keyRect.height);
	fill(pBlack);
	textAlign(TX_LEFT, TX_TOP);
	text(infobuff, 0, fb.frame.height + 2);

}

void draw()
{
	background(pLightGray);
	backgroundImage(&fb);

	drawMouseInfo();
}

void setup()
{
	int ret = PPM_read_binary("c:/repos/graphicc/Test/windows-keyboard-60-keys.ppm", &fb);

	size(fb.frame.width+4, fb.frame.height+4+30);
	background(pLightGray);
}


