/*
test_text

Do some simple text drawing

*/
#include "drawproc.h"
#include "guistyle.h"

static GUIStyle styler;
static const int gMaxMode = 3;
static int gMode = 0;

pb_rgba fb;

void draw();


void CALLBACK mousePressed()
{
	gMode++;
	if (gMode >= gMaxMode) {
		gMode = 0;
	}

	draw();
}

LRESULT CALLBACK keyReleased(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
	case VK_SPACE:
		write_PPM_binary("test_keyboard.ppm", gpb);
		break;
	}

	return 0;
}

void setup()
{
	int ret = PPM_read_binary("c:/repos/graphicc/Test/windows-keyboard-60-keys.ppm", &fb);

	size(fb.frame.width+4, fb.frame.height+4+20);
	background(pLightGray);
	noLoop();

	setOnMousePressedHandler(mousePressed);
	setOnKeyReleasedHandler(keyReleased);
}

void draw()
{
	background(pLightGray);

	noStroke();
	fill(pRed);
	rect(0, 0, width, height);

	raster_rgba_blit(gpb, 2, 2, &fb);
	
}
