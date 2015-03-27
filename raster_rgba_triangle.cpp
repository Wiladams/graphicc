/*
Copyright 2015 William A Adams

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

/*
	References:
		http://joshbeam.com/articles/triangle_rasterization/
*/

#include "raster_rgba.h"

#define swap16(a, b) { int16_t t = a; a = b; b = t; }

typedef struct _point2d
{
	int x;
	int y;
} point2d;

void point2d_init(point2d &pt, const int x, const int y)
{
	pt.x = x;
	pt.y = y;
}

typedef struct _edge
{
	int Color1;
	int Color2;
	int X1, Y1, X2, Y2;
} edge;

void edge_init(edge &e, int x1, int y1, int color1, int x2, int y2, int color2)
{
	e.Color1 = color1;
	e.Color2 = color2;

	e.X1 = x1;
	e.X2 = x2;

	e.Y1 = y1;
	e.Y2 = y2;
}


int FindTopmostPolyVertex(const point2d *poly, const size_t nelems)
{
	int ymin = INT32_MAX;
	int vmin = 0;

	size_t idx = 0;
	while (idx < nelems) {
		if (poly[idx].y < ymin) {
			ymin = poly[idx].y;
			vmin = idx;
		}
		idx++;
	}

	return vmin;
}

void sortTriangle(point2d *sorted, const int x1, const int y1, const int x2, const int y2, const int x3, const int y3)
{
	point2d verts[3] = { { x1, y1 }, { x2, y2 }, { x3, y3 } };

	int topmost = FindTopmostPolyVertex(verts, 3);

	sorted[0].x = verts[topmost].x;
	sorted[0].y = verts[topmost].y;

	switch (topmost) {
		case 0:
			if (verts[1].y < verts[2].y)
			{
				sorted[1].x = verts[1].x; sorted[1].y = verts[1].y;
				sorted[2].x = verts[2].x; sorted[2].y = verts[2].y;
			}
			else {
				sorted[1].x = verts[2].x; sorted[1].y = verts[2].y;
				sorted[2].x = verts[1].x; sorted[2].y = verts[1].y;
			}
		break;
		
		case 1:
			if (verts[0].y < verts[2].y)
			{
				sorted[1].x = verts[0].x; sorted[1].y = verts[0].y;
				sorted[2].x = verts[2].x; sorted[2].y = verts[2].y;
			}
			else {
				sorted[1].x = verts[2].x; sorted[1].y = verts[2].y;
				sorted[2].x = verts[0].x; sorted[2].y = verts[0].y;
			}		
		break;
	
		case 2:
			if (verts[0].y < verts[1].y)
			{
				sorted[1].x = verts[0].x; sorted[1].y = verts[0].y;
				sorted[2].x = verts[1].x; sorted[2].y = verts[1].y;
			}
			else {
				sorted[1].x = verts[1].x; sorted[1].y = verts[1].y;
				sorted[2].x = verts[0].x; sorted[2].y = verts[0].y;
			}
		break;
	}
}

/*
void raster_rgba_draw_spans_between_edges(pb_rgba *pb, const edge &e1, const edge &e2)
{
	// calculate difference between the y coordinates
	// of the first edge and return if 0
	float e1ydiff = (float)(e1.Y2 - e1.Y1);
	if (e1ydiff == 0.0f)
		return;

	// calculate difference between the y coordinates
	// of the second edge and return if 0
	float e2ydiff = (float)(e2.Y2 - e2.Y1);
	if (e2ydiff == 0.0f)
		return;

	// calculate differences between the x coordinates
	// and colors of the points of the edges
	float e1xdiff = (float)(e1.X2 - e1.X1);
	float e2xdiff = (float)(e2.X2 - e2.X1);
	int e1colordiff = (e1.Color2 - e1.Color1);
	int e2colordiff = (e2.Color2 - e2.Color1);

	// calculate factors to use for interpolation
	// with the edges and the step values to increase
	// them by after drawing each span
	float factor1 = (float)(e2.Y1 - e1.Y1) / e1ydiff;
	float factorStep1 = 1.0f / e1ydiff;
	float factor2 = 0.0f;
	float factorStep2 = 1.0f / e2ydiff;

	// loop through the lines between the edges and draw spans
	for (int y = e2.Y1; y < e2.Y2; y++) {
		int spanlength = e2.X1 + (int)(e2xdiff * factor2) - e1.X1 + (int)(e1xdiff * factor1);
		// draw the span
		//raster_rgba_draw_span(pb, e1.X1 + (int)(e1xdiff * factor1), e1.Color1 + (e1colordiff * factor1),
		//	e2.X1 + (int)(e2xdiff * factor2), e2.Color1 + (e2colordiff * factor2), y);
		
		
		//raster_rgba_hline(pb, e1.X1 + (int)(e1xdiff * factor1), y, spanlength, e1.Color1);
		raster_rgba_hline_blend(pb, e1.X1 + (int)(e1xdiff * factor1), y, spanlength, e1.Color1);
		// increase factors
		factor1 += factorStep1;
		factor2 += factorStep2;
	}
}
*/

void raster_rgba_triangle_fill(pb_rgba *pb, 
	const unsigned int x1, const unsigned int  y1, 
	const unsigned int  x2, const unsigned int  y2, 
	const unsigned int  x3, const unsigned int  y3, 
	int color)
{
	int a, b, y, last;

	// sort vertices, such that 0 == y with lowest number (top)
	point2d sorted[3];
	sortTriangle(sorted, x1, y1, x2, y2, x3, y3);

	// Handle the case where points are colinear (all on same line)
	if (sorted[0].y == sorted[2].y) { 
		a = b = sorted[0].x;
		
		if (sorted[1].x < a) 
			a = sorted[1].x;
		else if (sorted[1].x > b) 
			b = sorted[1].x;

		if (sorted[2].x < a) 
			a = sorted[2].x;
		else if (sorted[2].x > b) 
			b = sorted[2].x;

		raster_rgba_hline_blend(pb, a, sorted[0].y, b - a + 1, color);
		return;
	}

	int16_t
		dx01 = sorted[1].x - sorted[0].x,
		dy01 = sorted[1].y - sorted[0].y,
		dx02 = sorted[2].x - sorted[0].x,
		dy02 = sorted[2].y - sorted[0].y,
		dx12 = sorted[2].x - sorted[1].x,
		dy12 = sorted[2].y - sorted[1].y;
	
	int32_t sa = 0, sb = 0;

	// For upper part of triangle, find scanline crossings for segments
	// 0-1 and 0-2. If y1=y2 (flat-bottomed triangle), the scanline y1
	// is included here (and second loop will be skipped, avoiding a /0
	// error there), otherwise scanline y1 is skipped here and handled
	// in the second loop...which also avoids a /0 error here if y0=y1
	// (flat-topped triangle).
	if (sorted[1].y == sorted[2].y) 
		last = sorted[1].y; // Include y1 scanline
	else 
		last = sorted[1].y - 1; // Skip it
	
	for (y = sorted[0].y; y <= last; y++) 
	{
		a = sorted[0].x + sa / dy01;
		b = sorted[0].x + sb / dy02;
		sa += dx01;
		sb += dx02;
		/* longhand:
		a = x0 + (x1 - x0) * (y - y0) / (y1 - y0);
		b = x0 + (x2 - x0) * (y - y0) / (y2 - y0);
		*/
		
		if (a > b) swap16(a, b);
		raster_rgba_hline_blend(pb, a, y, b - a + 1, color);
	}

	// For lower part of triangle, find scanline crossings for segments
	// 0-2 and 1-2. This loop is skipped if y1=y2.
	sa = dx12 * (y - sorted[1].y);
	sb = dx02 * (y - sorted[0].y);
	for (; y <= sorted[2].y; y++) 
	{
		a = sorted[1].x + sa / dy12;
		b = sorted[0].x + sb / dy02;
		sa += dx12;
		sb += dx02;
		/* longhand:
		a = x1 + (x2 - x1) * (y - y1) / (y2 - y1);
		b = x0 + (x2 - x0) * (y - y0) / (y2 - y0);
		*/
		if (a > b) 
			swap16(a, b);

		raster_rgba_hline_blend(pb, a, y, b - a + 1, color);
	}
}


/*
// http://www.sunshine2k.de/coding/java/TriangleRasterization/TriangleRasterization.html
void fillBottomFlatTriangle(pb_rgba *pb,
	const int v1x, const int v1y,
	const int v2x, const int v2y,
	const int v3x, const int v3y)
{
	float invslope1 = (v2x - v1x) / (v2y - v1y);
	float invslope2 = (v3x - v1x) / (v3y - v1y);

	float curx1 = v1x;
	float curx2 = v1x;

	for (int scanlineY = v1.y; scanlineY <= v2.y; scanlineY++)
	{
		line((int)curx1, scanlineY, (int)curx2, scanlineY);
		curx1 += invslope1;
		curx2 += invslope2;
	}
}


void fillTopFlatTriangle(Vertice v1, Vertice v2, Vertice v3)
{
float invslope1 = (v3.x - v1.x) / (v3.y - v1.y);
float invslope2 = (v3.x - v2.x) / (v3.y - v2.y);

float curx1 = v3.x;
float curx2 = v3.x;

for (int scanlineY = v3.y; scanlineY > v1.y; scanlineY--)
{
curx1 -= invslope1;
curx2 -= invslope2;
drawLine((int)curx1, scanlineY, (int)curx2, scanlineY);
}
}
*/
/*
void raster_triangle_fill(pb_rgba *pb,
	const unsigned int x1, const unsigned int  y1,
	const unsigned int  x2, const unsigned int  y2,
	const unsigned int  x3, const unsigned int  y3,
	int color)
{
	// at first sort the three vertices by y-coordinate ascending so v1 is the topmost vertice
	point2d sorted[3];
	sortTriangle(sorted, x1, y1, x2, y2, x3, y3);

	// here we know that v1.y <= v2.y <= v3.y
	// check for trivial case of bottom-flat triangle
	if (sorted[1].y == sorted[2].y)
	{
		fillBottomFlatTriangle(pb_rgba *pb, v1, v2, v3, color);
	} else if (vt1.y == vt2.y)
	{
		// check for trivial case of top-flat triangle
		fillTopFlatTriangle(g, vt1, vt2, vt3);
	} else {
		// general case - split the triangle in a topflat and bottom-flat one
		Vertice v4 = new Vertice(
		(int)(vt1.x + ((float)(vt2.y - vt1.y) / (float)(vt3.y - vt1.y)) * (vt3.x - vt1.x)), vt2.y);
		fillBottomFlatTriangle(g, vt1, vt2, v4);
		fillTopFlatTriangle(g, vt2, v4, vt3);
	}
}
*/
