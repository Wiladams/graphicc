/*
test_text

Do some simple text drawing

*/
#include "drawproc.h"
#include "guistyle.h"

static GUIStyle styler;
static const int gMaxMode = 3;
static int gMode = 0;

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
	size(1024, 768);
	background(pLightGray);
	noLoop();

	//setOnMousePressedHandler(mousePressed);
	setOnKeyReleasedHandler(keyReleased);
}

void draw()
{
	background(pLightGray);

}
