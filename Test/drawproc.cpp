
#include "drawproc.h"

#include <math.h>


// Globals for the environment
pb_rgba *gpb;
size_t width = 0;
size_t height = 0;
pb_rect pixelFrame;

int grectMode = CORNER;
int gellipseMode = CORNER;


/*
// Keyboard
int keyCode = 0;
int key = 0;
int isKeyPressed = 0;

// Mouse
int mouseX = 0;
int mouseY = 0;
bool isMousePressed = false;
int mouseButton = 0;
*/
// color setting
uint32_t bgColor = pDarkGray;
pb_rgba *bgImage = nullptr;
uint32_t strokeColor = RGBA(0, 0, 0, 255);
uint32_t fillColor = RGBA(255, 255, 255, 255);

// Text Settings
font_t gfont;

void init()
{
	// Setup text
	font_t_init(&gfont, verdana12);
}

// size of window
void size(const size_t lwidth, const size_t lheight)
{
	init();

	width = lwidth;
	height = lheight;

	pixelFrame.x = 0;
	pixelFrame.y = 0;
	pixelFrame.width = width;
	pixelFrame.height = height;

	void *data = SetWindowSize(lwidth, lheight);

	// Create the global pixel buffer
	// Save the pointer into the pb_rgba pixel buffer
	gpb = new pb_rgba();
	gpb->data = (uint8_t *)data;
	gpb->owndata = false;
	gpb->pixelpitch = width;
	gpb->frame.height = height;
	gpb->frame.width = width;
	gpb->frame.x = 0;
	gpb->frame.y = 0;

	// paint the background at least once before beginning
	raster_rgba_rect_fill(gpb, 0, 0, width, height, bgColor);
}




// Math
int random(const int rndMax)
{
	return rand() % rndMax;
}

// color setting
void background(const uint32_t value)
{
	bgColor = value;
	if ((gpb != NULL) && (width > 0) && (height > 0))
	{
		raster_rgba_rect_fill(gpb, 0, 0, width - 1, height - 1, bgColor);
	}
}

void noFill()
{
	fillColor = 0;
}

void noStroke()
{
	strokeColor = 0;
}

void stroke(const uint8_t value)
{
	strokeColor = RGBA(value, value, value, 255);
}

void stroke(const uint32_t value)
{
	strokeColor = value;
}

void fill(const uint8_t value)
{
	fillColor = RGBA(value, value, value, 255);
}

void fill(const uint32_t value)
{
	fillColor = value;
}

// 2D primitives
void bezier(const int x1, const int y1, const int x2, const int y2, const int x3, const int y3)
{

}

void ellipse(const int a, const int b, const int c, const int d)
{
	size_t xradius = c / 2;
	size_t yradius = d / 2;

	uint32_t cx = a + xradius;
	uint32_t cy = b + yradius;
	raster_rgba_ellipse_stroke(gpb, cx, cy, xradius, yradius, strokeColor);
}

//
// Line Clipping in preparation for line drawing
//
typedef int OutCode;

static const int INSIDE = 0; // 0000
static const int LEFT = 1;   // 0001
static const int RIGHT = 2;  // 0010
static const int BOTTOM = 4; // 0100
static const int TOP = 8;    // 1000

// Compute the bit code for a point (x, y) using the clip rectangle
// bounded diagonally by (xmin, ymin), and (xmax, ymax)

OutCode ComputeOutCode(const pb_rect &rct, const int x, const int y)
{
	OutCode code;
	double xmin = rct.x;
	double xmax = rct.x + rct.width - 1;
	double ymin = rct.y;
	double ymax = rct.y + rct.height - 1;

	code = INSIDE;          // initialised as being inside of clip window

	if (x < xmin)           // to the left of clip window
		code |= LEFT;
	else if (x > xmax)      // to the right of clip window
		code |= RIGHT;
	if (y < ymin)           // below the clip window
		code |= BOTTOM;
	else if (y > ymax)      // above the clip window
		code |= TOP;

	return code;
}

// Cohen–Sutherland clipping algorithm clips a line from
// P0 = (x0, y0) to P1 = (x1, y1) against a rectangle with 
// diagonal from (xmin, ymin) to (xmax, ymax).
bool ClipLine(const pb_rect &bounds, int &x0, int &y0, int &x1, int &y1)
{
	// compute outcodes for P0, P1, and whatever point lies outside the clip rectangle
	OutCode outcode0 = ComputeOutCode(bounds, x0, y0);
	OutCode outcode1 = ComputeOutCode(bounds, x1, y1);
	bool accept = false;
	double xmin = bounds.x;
	double xmax = bounds.x + bounds.width - 1;
	double ymin = bounds.y;
	double ymax = bounds.y + bounds.height - 1;


	while (true) {
		if (!(outcode0 | outcode1)) { // Bitwise OR is 0. Trivially accept and get out of loop
			accept = true;
			break;
		}
		else if (outcode0 & outcode1) { // Bitwise AND is not 0. Trivially reject and get out of loop
			break;
		}
		else {
			// failed both tests, so calculate the line segment to clip
			// from an outside point to an intersection with clip edge
			double x, y;

			// At least one endpoint is outside the clip rectangle; pick it.
			OutCode outcodeOut = outcode0 ? outcode0 : outcode1;

			// Now find the intersection point;
			// use formulas y = y0 + slope * (x - x0), x = x0 + (1 / slope) * (y - y0)
			if (outcodeOut & TOP) {           // point is above the clip rectangle
				x = x0 + (x1 - x0) * (ymax - y0) / (y1 - y0);
				y = ymax;
			}
			else if (outcodeOut & BOTTOM) { // point is below the clip rectangle
				x = x0 + (x1 - x0) * (ymin - y0) / (y1 - y0);
				y = ymin;
			}
			else if (outcodeOut & RIGHT) {  // point is to the right of clip rectangle
				y = y0 + (y1 - y0) * (xmax - x0) / (x1 - x0);
				x = xmax;
			}
			else if (outcodeOut & LEFT) {   // point is to the left of clip rectangle
				y = y0 + (y1 - y0) * (xmin - x0) / (x1 - x0);
				x = xmin;
			}

			// Now we move outside point to intersection point to clip
			// and get ready for next pass.
			if (outcodeOut == outcode0) {
				x0 = x;
				y0 = y;
				outcode0 = ComputeOutCode(bounds, x0, y0);
			}
			else {
				x1 = x;
				y1 = y;
				outcode1 = ComputeOutCode(bounds, x1, y1);
			}
		}
	}

	return accept;
}



void line(const int x1, const int y1, const int x2, const int y2)
{
	int xx1 = x1;
	int yy1 = y1;
	int xx2 = x2;
	int yy2 = y2;

	if (!ClipLine(pixelFrame, xx1, yy1, xx2, yy2))
	{
		return;
	}

	// TODO - need to intersect line with pixelFrame
	if ((xx1 < 0) || (yy1 < 0))
		return;

	if ((xx2 > xx1 + width) || (yy2 > y1 + height))
		return;

	raster_rgba_line(gpb, xx1, yy1, xx2, yy2, strokeColor);
}

void lineloop(const size_t nPts, const int *pts)
{
	if (nPts < 2)
		return;

	int firstx = pts[0];
	int firsty = pts[1];
	int lastx = firstx;
	int lasty = firsty;

	for (int idx = 1; idx < nPts; idx++)
	{
		int nextx = pts[idx * 2];
		int nexty = pts[(idx * 2) + 1];
		line(lastx, lasty, nextx, nexty);
		lastx = nextx;
		lasty = nexty;
	}

	// draw last point to beginning
	line(lastx, lasty, firstx, firsty);
}

void point(const int x, const int y)
{
	if (!pb_rect_contains_point(pixelFrame, x, y))
		return;

	pb_rgba_cover_pixel(gpb, x, y, strokeColor);
}

void rectMode(const int mode)
{
	grectMode = mode;
}

void rect(const int a, const int b, const int c, const int d)
{
	int x1 = 0, y1 = 0;
	int rwidth = 0, rheight = 0;

	switch (grectMode) {
	case CORNER:
		x1 = a;
		y1 = b;
		rwidth = c;
		rheight = d;
		break;

	case CORNERS:
		x1 = a;
		y1 = b;
		rwidth = c - a + 1;
		rheight = d - b + 1;
		break;

	case CENTER:
		x1 = a - c / 2;
		y1 = b - d / 2;
		rwidth = c;
		rheight = d;
		break;

	case RADIUS:
		x1 = a - c;
		y1 = b - d;
		rwidth = c * 2;
		rheight = d * 2;
		break;
	}

	// find the intersection of the rectangle and the pixelframe
	pb_rect crect;
	pb_rect_intersection(crect, pixelFrame, { x1, y1, rwidth, rheight });

	if ((crect.width == 0) || (crect.height == 0))
		return;

	if (fillColor != 0) {
		//raster_rgba_rect_fill_blend(gpb, x1, y1, rwidth, rheight, fillColor);
		raster_rgba_rect_fill_blend(gpb, crect.x, crect.y, crect.width, crect.height, fillColor);
	}

	// if the strokeColor != 0 then 
	// frame the rectangle in the strokeColor
	if (strokeColor != 0) {
		int pts[] = {
			crect.x, crect.y,
			crect.x, crect.y + crect.height - 1,
			crect.x + crect.width - 1, crect.y + crect.height - 1,
			crect.x + crect.width - 1, crect.y
		};

		lineloop(4, pts);
	}
}

#define min3(a,b,c) min(min(a,b),c)
#define max3(a,b,c) max(max(a,b),c)


void triangle(const int x1, const int y1, const int x2, const int y2, const int x3, const int y3)
{
	if (fillColor != 0) {
		raster_rgba_triangle_fill(gpb, x1, y1, x2, y2, x3, y3, fillColor);
	}

	if (strokeColor != 0) {
		int pts[] = {
			x1, y1,
			x2, y2,
			x3, y3
		};
		lineloop(3, pts);
	}
}

void quad(const int x1, const int y1, const int x2, const int y2, const int x3, const int y3, const int x4, const int y4)
{
	// preserve current stroke color
	uint32_t savedStroke = strokeColor;

	noStroke();
	// triangle 1
	triangle(x1, y1, x2, y2, x4, y4);

	// triangle 2
	triangle(x2, y2, x3, y3, x4, y4);

	// outline
	stroke(savedStroke);

	if (strokeColor != 0) {
		int pts[] = {
			x1, y1,
			x2, y2,
			x3, y3,
			x4, y4
		};
		lineloop(4, pts);
	}
}


// polygon
//  public-domain code by Darel Rex Finley, 2007
// http://alienryderflex.com/polygon_fill/
//
#define MAX_POLY_CORNERS 256


//  Globals which should be set before calling this function:
//
//  int    polyCorners  =  how many corners the polygon has
//  float  polyX[]      =  horizontal coordinates of corners
//  float  polyY[]      =  vertical coordinates of corners
//  float  x, y         =  point to be tested
//
//
//  The function will return YES if the point x,y is inside the polygon, or
//  NO if it is not.  If the point is exactly on the edge of the polygon,
//  then the function may return YES or NO.
//


int pointInPolygon(int polyCorners, int polyX[], int polyY[], const int x, const int y) 
{
	int   i, j = polyCorners - 1;
	int  oddNodes = 0;

	for (i = 0; i<polyCorners; i++) 
	{
		if ((polyY[i]< y && polyY[j] >= y
			|| polyY[j]< y && polyY[i] >= y)
			&& (polyX[i] <= x || polyX[j] <= x)) 
		{
			oddNodes ^= (polyX[i] + (y - polyY[i]) / (polyY[j] - polyY[i])*(polyX[j] - polyX[i])<x);
		}
		j = i;
	}

	return oddNodes;
}

int FinfMax(int **a, int i)
{
	if (a[i][1] > a[i + 1][1]) {
		return a[i][1];
	}
	else {
		return a[i + 1][1];
	}
}

int FinfMin(int **a, int **b, int i)
{
	if (a[i][1] < a[i + 1][1]) {
		b[i][2] = a[i][0];
		return a[i][1];
	}
	else {
		b[i][2] = a[i + 1][0];
		return a[i + 1][1];
	}
}

double FinSlop(int **a, float *slope, int i)
{
	int dy = a[i + 1][1] - a[i][1];
	int dx = a[i + 1][0] - a[i][0];
	if (dy == 0) { slope[i] = 1; }
	if (dx == 0) { slope[i] = 0; }
	if ((dy != 0) && (dx != 0)) // calculate inverse slope
		slope[i] = ((float)dx / (float)dy);

	return (float)slope[i];
}


void polygon(int nverts, int **a)
{

}

/*
void polygon(int polyCorners, int polyX[], int polyY[])
{
	int IMAGE_TOP = 0;
	int IMAGE_LEFT = 0;
	int IMAGE_RIGHT = width-1;
	int IMAGE_BOT = height-1;

	int nodes;
	int nodeX[MAX_POLY_CORNERS];
	int pixelX, pixelY;
	int i, j;
	int swap;

	//  Loop through the rows of the image.
	for (pixelY = IMAGE_TOP; pixelY < IMAGE_BOT; pixelY++)
	{
		//  Build a list of nodes.
		nodes = 0;
		j = polyCorners - 1;
		for (i = 0; i < polyCorners; i++) 
		{
			if (polyY[i] < (double)pixelY && polyY[j] >= (double)pixelY
				|| polyY[j] < (double)pixelY && polyY[i] >= (double)pixelY)
			{
				nodeX[nodes++] = (int)(polyX[i] + (pixelY - polyY[i]) / (polyY[j] - polyY[i])*(polyX[j] - polyX[i]));
			}
			j = i;
		}

		//  Sort the nodes, via a simple “Bubble” sort.
		i = 0;
		while (i<nodes - 1)
		{
			if (nodeX[i]>nodeX[i + 1])
			{
				swap = nodeX[i];
				nodeX[i] = nodeX[i + 1];
				nodeX[i + 1] = swap;
				if (i) i--;
			}
			else {
				i++;
			}
		}

		//  Fill the pixels between node pairs.
		for (i = 0; i<nodes; i += 2) {
			if (nodeX[i] >= IMAGE_RIGHT) break;
			if (nodeX[i + 1]> IMAGE_LEFT) {
				if (nodeX[i]< IMAGE_LEFT) nodeX[i] = IMAGE_LEFT;
				if (nodeX[i + 1]> IMAGE_RIGHT) nodeX[i + 1] = IMAGE_RIGHT;
				for (pixelX = nodeX[i]; pixelX < nodeX[i + 1]; pixelX++)
					point(pixelX, pixelY);
			}
		}
	}
}
*/

// Text Processing
void text(const char *str, const int x, const int y)
{
	scan_str(gpb, &gfont, x, y, str, fillColor);
}

void setFont(const uint8_t *fontdata)
{
	font_t_init(&gfont, fontdata);
}
