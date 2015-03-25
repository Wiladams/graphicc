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





extern "C"
void step(pb_rgba *pb)
{
	uint32_t color = rand() % 10;

	for (int offset = 0; offset < numbars; offset++)
	{
		double secfrag = fmod(seconds(), bars[offset].interval);
		int barheight = MAP(secfrag, 0, bars[offset].interval, 4, height - 1);
		raster_rgba_rect_fill(pb, offset*(bargap + barwidth) + bargap, height - barheight, barwidth, barheight, bars[offset].color);
	}

	stroke(pBlack);
	ellipse(10, 10, 160, 120);

	line(30, 20, 85, 75);
	line(30, 20, 85, 20);
	stroke((uint8_t)126);
	line(85, 20, 85, 75);
	stroke((uint8_t)255);
	line(85, 75, 30, 75);

	stroke(pBlack);
	point(130, 20);
	point(185, 20);
	point(185, 75);
	point(130, 75);

}



