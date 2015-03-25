/*
	test_main.cpp

	For graphicc, primarily testing drawing rectangles.


	For animwin32, testing sizing a window, and drawing into 
	it over time.

	The step() function basically draws a set of bars which are
	proportional to the modulus of their time interval.  So, a bar
	that is supposed to take one second to go from zero to the top, 
	will grow from size zero, the height of window, within that second.
*/

#include "animwin32.h"
#include "linearalgebra.h"
#include <math.h>





static uint32_t colors[] = {
	pBlack,
	pRed,
	pDarkGray,
	pGreen,
	pLightGray,
	pBlue,
	aliceblue,
	pYellow,
	pTurquoise,
	pWhite,
};

static double intervals[] = {
//	1.0 / 0.125,
//	1.0 / 0.25,
//	1.0 / 0.333,
//	1.0 / 0.5,
	1.0 / 0.75,
	1.0 / 1.0,
	1.0 / 1.25,
	1.0 / 2.0,
	1.0 / 3.0
};

static int numintervals = 0;
static int numcolors = 0;

static int bargap = 4;
static int numbars = 10;
static int barwidth = 0;


typedef struct {
	uint32_t color;
	double interval;
}bar;

bar *bars;

extern "C"
void setup()
{
	size(640, 480);
	background(pLightGray);

	// setup the array of color bars
	numintervals = sizeof(intervals) / sizeof(intervals[0]);
	numcolors = sizeof(colors) / sizeof(colors[0]);

	int availwidth = width - (2 + numbars - 1)*bargap;
	barwidth = availwidth / numbars;

	// assign colors to bars
	bars = new bar[numbars];
	for (int idx = 0; idx < numbars; idx++)
	{
		bars[idx].color = colors[rand() % 10];
		bars[idx].interval = intervals[rand() % numintervals];
	}
}



void drawPoints()
{
	stroke(pBlack);
	point(130, 20);
	point(185, 20);
	point(185, 75);
	point(130, 75);
}

void drawLines()
{
	line(30, 20, 85, 75);
	line(30, 20, 85, 20);
	stroke((uint8_t)126);
	line(85, 20, 85, 75);
	stroke((uint8_t)255);
	line(85, 75, 30, 75);
}

void drawRects()
{
	rectMode(CORNER);
	fill((uint8_t)255);
	rect(25, 25, 60, 60);

	rectMode(CORNERS);
	fill((uint8_t)100);
	rect(25, 25, 50, 50);

	rectMode(RADIUS);
	fill((uint8_t)255);
	rect(250, 50, 30, 30);

	rectMode(CENTER);
	fill((uint8_t)100);
	rect(250, 50, 30, 30);
}

void drawEllipses()
{
	stroke(pBlack);
	ellipse(10, 10, 160, 120);
}

void drawTriangles()
{
	stroke(pBlack);
	fill((uint8_t)255);
	triangle(30, 75, 58, 20, 86, 75);
}

void drawBars()
{
	rectMode(CORNER);
	//nostroke();

	for (int offset = 0; offset < numbars; offset++)
	{
		double secfrag = fmod(seconds(), bars[offset].interval);
		int barheight = MAP(secfrag, 0, bars[offset].interval, 4, height - 1);
		fill(bars[offset].color);
		rect(offset*(bargap + barwidth) + bargap, height - barheight, barwidth, barheight);
	}
}

void drawMouse()
{
	int mWidth = 64;
	int mHeight = 64;

	rectMode(CENTER);
	stroke(pBlack);
	fill(RGBA(0, 127, 255, 200));
	rect(mouseX, mouseY, mWidth, mHeight);
}


extern "C"
void step(pb_rgba *pb)
{
	drawBars();
	//drawEllipses();
	drawLines();
	//drawPoints();
	drawRects();
	drawTriangles();
	drawMouse();

}



