/*
	test_text

	Do some simple text drawing

*/
#include "drawproc.h"
#include "GUIStyle.h"

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
		write_PPM_binary("test_text.ppm", gpb);
		break;
	}

	return 0;
}

void setup()
{
	size(320, 240);
	background(pLightGray);
	noLoop();

	//setOnMousePressedHandler(mousePressed);
	setOnKeyReleasedHandler(keyReleased);
}

void draw()
{
	background(pLightGray);

	styler.SetBaseColor(pLightGray);
	styler.DrawFrame(20, 20, 100, 100, Sunken);
	styler.DrawSunkenRect(60, 60, 100, 100);
	styler.DrawRaisedRect(100, 100, 100, 100);

	// Draw some lines
	stroke(pBlack);
	line(width / 2, 0, width / 2, height - 1);
	line(0, height / 2, width - 1, height / 2);

	// draw some text
	int midx = width / 2;
	int midy = height / 2;
	fill(pBlack);
	textAlign(TX_LEFT);
	text("LEFT", midx, 20);
	
	textAlign(TX_CENTER);
	text("CENTER", midx, 40);
	
	textAlign(TX_RIGHT);
	text("RIGHT", midx, 60);

	// Around the center
	textAlign(TX_LEFT, TX_TOP);
	text("LEFT TOP", 0, 0);

	textAlign(TX_RIGHT, TX_TOP);
	text("RIGHT TOP",width,0);

	textAlign(TX_RIGHT, TX_BOTTOM);
	text("RIGHT BOTTOM", width,height);

	textAlign(TX_LEFT, TX_BOTTOM);
	text("LEFT BOTTOM",0,height);

	stroke(pRed);
	line(midx - 6, midy, midx + 6, midy);
	line(midx, midy - 6, midx, midy + 6);

	fill(pWhite);
	textAlign(TX_CENTER, TX_CENTER);
	text("CENTER CENTER", midx, midy);



}
