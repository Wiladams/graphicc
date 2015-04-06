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
	stroke(pBlack);
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

void drawRandomRectangles()
{
	int lwidth = 32;
	int lheight = 32;

	rectMode(CORNER);
	//noStroke();
	stroke(pBlack);

	for (int cnt = 1001; cnt; cnt--)
	{
		uint8_t r = rand() % 255;
		uint8_t g = rand() % 255;
		uint8_t b = rand() % 255;
		uint32_t c = RGBA(r, g, b, 202);

		int x1 = rand() % (width - 1);
		int y1 = rand() % (height - 1);

		//noFill();
		fill(c);
		rect(x1, y1, lwidth, lheight);
	}
}

void drawQuads()
{
	stroke(pBlack);
	fill(pWhite);
	quad(38, 31, 86, 20, 69, 63, 30, 76);
}

void drawRandomLines()
{
	for (int cnt = 1001; cnt; cnt--)
	{
		uint8_t r = rand() % 255;
		uint8_t g = rand() % 255;
		uint8_t b = rand() % 255;
		uint32_t c = RGBA(r, g, b, 255);

		int x1 = rand() % width - 1;
		int y1 = rand() % height - 1;
		int x2 = rand() % width - 1;
		int y2 = rand() % height - 1;

		stroke(c);
		line(x1, y1, x2, y2);
	}
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
	//triangle(30, 75, 58, 20, 86, 75);

	stroke(pBlack);
	fill(pWhite);
	triangle(38, 31, 86, 20, 30, 76);

}

void drawRandomTriangles()
{
	//noStroke();
	stroke(pBlack);

	for (int cnt = 5001; cnt; cnt--)
	{
		uint8_t r = rand() % 255;
		uint8_t g = rand() % 255;
		uint8_t b = rand() % 255;
		uint32_t c = RGBA(r, g, b, 202);

		int x1 = rand() % (width - 64)+20;
		int y1 = rand() % (height - 64)+20;
		
		int maxsize = 20;
		int minx = x1 - maxsize;
		int miny = y1 - maxsize;

		int xrange = (x1 + maxsize) - (x1 - maxsize);
		int yrange = (y1 + maxsize) - (y1 - maxsize);

		int x2 = (rand() % xrange) + minx;
		int y2 = (rand() % yrange) + miny;

		int x3 = (rand() % xrange) + minx;
		int y3 = (rand() % yrange) + miny;

		//noFill();
		fill(c);
		triangle(x1, y1, x2, y2, x3, y3);
	}
}

void drawBars()
{
	rectMode(CORNER);
	//nostroke();
	stroke(pBlack);

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
void setup()
{
	size(1024, 768);
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

extern "C"
void draw()
{
	//drawEllipses();
	//drawLines();
	//drawPoints();
	//drawRects();
	//drawTriangles();
	//drawQuads();

	//drawRandomRectangles();
	//drawRandomLines();
	drawRandomTriangles();
	drawBars();

	drawMouse();

}



