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

#include <math.h>

#include "drawproc.h"
#include "test_common.h"

static uint32_t colors[] = {
	pBlack,
	RGBA(255, 0, 0, 202),	// Red
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
	1.0 / 0.25,
	1.0 / 0.333,
	1.0 / 0.5,
	1.0 / 0.75,
	1.0 / 1.0,
//	1.0 / 1.25,
//	1.0 / 2.0,
//	1.0 / 3.0
};

static int numintervals = 0;
static int numcolors = 0;

static int bargap = 4;
static int numbars = 10;
static int barwidth = 0;
static int frameCount = 0;
static double starttime = 0;

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
	int halfsize = 20;

	for (int cnt = 10001; cnt; cnt--)
	{
		uint8_t r = rand() % 255;
		uint8_t g = rand() % 255;
		uint8_t b = rand() % 255;
		uint32_t c = RGBA(r, g, b, 255);

		int x = rand() % width - 1;
		int y = rand() % height - 1;

		int x1 = x - rand() % halfsize - 1;
		int y1 = y - rand() % halfsize - 1;
		int x2 = x + rand() % halfsize - 1;
		int y2 = y + rand() % halfsize - 1;

		stroke(c);
		line(x1, y1, x2, y2);
	}
}

void drawEllipses()
{
	stroke(pBlack);
	fill(pRed);
	ellipse(10, 10, 160, 120);

	stroke(pBlue);
	fill(pYellow);
	ellipse(150, 200, 60, 150);
}

void drawTriangles()
{
	stroke(pBlack);
	fill((uint8_t)255);
	triangle(30, 75, 58, 20, 86, 75);

	stroke(pBlack);
	fill(pWhite);
	//triangle(38, 31, 86, 20, 30, 76);

}

void drawRandomTriangles()
{
	//noStroke();
	stroke(pBlack);

	for (int cnt = 1001; cnt; cnt--)
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

	int *b = (int *)malloc(sizeof(int)* 2 * (numbars+2));

	// first anchor point
	int polyidx = 0;
	b[(polyidx*2)+0] = bargap;
	b[(polyidx*2)+1] = height-1;
	
	for (int offset = 0; offset < numbars; offset++)
	{
		polyidx++;

		double secfrag = fmod(seconds(), bars[offset].interval);
		int barheight = MAP(secfrag, 0, bars[offset].interval, 4, height - 1);
		fill(bars[offset].color);
		int x1 = offset*(bargap + barwidth) + bargap;
		int y1 = height - barheight;
		rect(x1, y1, barwidth, barheight);

		b[(polyidx*2) + 0] = x1+(barwidth/2);
		b[(polyidx*2) + 1] = y1;
	}

	// anchor point
	polyidx++;
	b[(polyidx * 2) + 0] = width - 1;
	b[(polyidx * 2) + 1] = height - 1;

	//fill(RGBA(127, 127, 127, 80));
	fill(RGBA(0, 127, 255, 120));
	polygon(numbars + 2, b);

	free(b);
}

void drawPolygon()
{
	int a[] = {
		5,5,
		150,5,
		150,25,
		640,25,
		640,480,
		5,480
	};
	int nverts = (sizeof(a) / sizeof(a[0]))/2;

	fill(pYellow);
	polygon(nverts, a);

	int b[] = {
		5,25,
		160,25,
		 160, 5 ,
		 250, 5 ,
		 250, 25 ,
		 640, 25 ,
		 640, 480 ,
		 5, 480 
	};
	int bverts = (sizeof(b) / sizeof(b[0]))/2;

	fill(pGreen);
	polygon(bverts, b);

}

void drawShapes()
{
	stroke(pBlack);
	fill(pWhite);

	
	beginShape();
	vertex(30, 40);
	vertex(85, 40);
	vertex(85, 95);
	vertex(30, 95);
	endShape(CLOSE);
	

	beginShape(GR_POINTS);
	vertex(100, 40);
	vertex(155, 40);
	vertex(155, 95);
	vertex(100, 95);
	endShape();


	beginShape(GR_LINES);
	vertex(230, 40);
	vertex(285, 40);
	vertex(285, 95);
	vertex(230, 95);
	endShape();


	beginShape(GR_TRIANGLES);
	vertex(30, 195);
	vertex(40, 140);
	vertex(50, 195);
	vertex(60, 140);
	vertex(70, 195);
	vertex(80, 140);
	endShape(1);

	
	beginShape(GR_QUADS);
	vertex(130, 140);
	vertex(130, 195);
	vertex(150, 195);
	vertex(150, 140);
	vertex(165, 140);
	vertex(165, 195);
	vertex(185, 195);
	vertex(185, 140);
	endShape();
	

beginShape(GR_POLYGON);
	vertex(220, 140);
	vertex(240, 140);
	vertex(240, 160);
	vertex(260, 160);
	vertex(260, 180);
	vertex(220, 180);
	endShape(1);

}

void drawMouse()
{
	int mWidth = 128;
	int mHeight = 128;

	rectMode(CENTER);
	stroke(pBlack);
	fill(RGBA(0, 127, 255, 200));
	rect(mouseX, mouseY, mWidth, mHeight);
}

// Draw information about the mouse
// location, buttons pressed, etc
void drawMouseInfo()
{
	// draw a white banner across the top
	noStroke();
	fill(pWhite);
	rect(0, 0, width, 24);

	// select verdana font
	setFont(verdana17);

	// write some text
	double fps = (double)frameCount / seconds();

	char infobuff[256];
	sprintf_s(infobuff, "Mouse X: %3d Y: %3d\tFPS: %f\n", mouseX, mouseY, fps);
	fill(pBlack);
	text(infobuff, 10, 0);
}

static float a;

void drawLinearMotion()
{
	background((uint8_t)51);
	stroke(pWhite);

	int thickness = 6;
	while (thickness--) {
		line(0, a + thickness, width - 1, a + thickness);
	}


	a = a - 0.02;
	if (a < 0) {
		a = height;
	}
}

static bool dumpimage = false;

LRESULT CALLBACK keyReleased(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{

	case VK_SPACE:
		dumpimage = true;
		break;
	}

	return 0;
}

int desiredWidth = 640;
int desiredHeight = 480;
pb_rgba localpb;

extern "C"
void setup()
{

	size(desiredWidth, desiredHeight);
	background(pLightGray);

	a = height / 2;

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

	setOnKeyReleasedHandler(keyReleased);
	resettime();
}

extern "C"
void draw()
{
	frameCount++;

	background(pLightGray);

	//drawEllipses();
	//drawLines();
	//drawPoints();
	//drawRects();
	//drawTriangles();
	//drawQuads();
	//drawPolygon();
	drawShapes();

	//drawRandomRectangles();
	//drawRandomLines();
	//drawRandomTriangles();
	//drawBars();
	//drawLinearMotion();

	//drawMouse();
	drawMouseInfo();

	if (dumpimage)
	{
		write_PPM_binary("test_main.ppm", gpb);
		dumpimage = false;
	}
}



