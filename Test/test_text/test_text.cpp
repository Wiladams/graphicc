/*
	test_text

	Do some simple text drawing

*/
#include "drawproc.h"

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


void setup()
{
	size(320, 240);
	background(pLightGray);
	noLoop();

	setOnMousePressedHandler(mousePressed);
}

void draw()
{
	background(pLightGray);


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

	textAlign(TX_CENTER, TX_CENTER);
	text("CENTER CENTER", midx, midy);
}
