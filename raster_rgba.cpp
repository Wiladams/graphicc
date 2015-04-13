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

#include "raster_rgba.h"

#include <math.h>
#include <string.h>
#include <stdlib.h>


// Some helpful macros
#define lerp255(bg, fg, a) ((uint8_t)div255((fg*a+bg*(255-a))))

#define blend_color(bg, fg) RGBA(				\
	lerp255(GET_R(bg), GET_R(fg), GET_A(fg)), \
	lerp255(GET_G(bg), GET_G(fg), GET_A(fg)), \
	lerp255(GET_B(bg), GET_B(fg), GET_A(fg)), 255)



// Draw a span of pixels
// This is basically a SRCCOPY
void raster_rgba_span(pb_rgba *pb, const uint32_t x, const uint32_t y, const size_t len, const uint32_t *data)
{
	// get the pointer to the beginning of the row
	// in the destination
	uint32_t *dstPtr = &((uint32_t *)pb->data)[y*pb->pixelpitch + x];
	
	// Just loop over pointers doing basic copies
	size_t idx;
	for (idx = 0; idx < len; idx++)
	{
		*dstPtr = data[idx];
		dstPtr++;
	}
}

void raster_rgba_hline_fade(pb_rgba *pb, int x1, int color1, int x2, int color2, int y)
{
	int xdiff = x2 - x1;
	if (xdiff == 0)
		return;

	int c1rd = GET_R(color1);
	int c1gr = GET_G(color1);
	int c1bl = GET_B(color1);

	int c2rd = GET_R(color2);
	int c2gr = GET_G(color2);
	int c2bl = GET_B(color2);

	int rdx = c2rd - c1rd;
	int gdx = c2gr - c1gr;
	int bdx = c2bl - c1bl;

	float factor = 0.0f;
	float factorStep = 1.0f / (float)xdiff;

	// draw each pixel in the span
	for (int x = x1; x < x2; x++) {
		int rd = c1rd + (int)(rdx*factor);
		int gr = c1gr + (int)(gdx*factor);
		int bl = c1bl + (int)(bdx*factor);
		int fg = RGBA(rd, gr, bl, 255);
		pb_rgba_set_pixel(pb, x, y, fg);

		factor += factorStep;
	}
}

void raster_rgba_vline_fade(pb_rgba *pb, int y1, int color1, int y2, int color2, int x)
{
	int ydiff = y2 - y1;
	if (ydiff == 0)
		return;

	int c1rd = GET_R(color1);
	int c1gr = GET_G(color1);
	int c1bl = GET_B(color1);

	int c2rd = GET_R(color2);
	int c2gr = GET_G(color2);
	int c2bl = GET_B(color2);

	int rdx = c2rd - c1rd;
	int gdx = c2gr - c1gr;
	int bdx = c2bl - c1bl;

	float factor = 0.0f;
	float factorStep = 1.0f / (float)ydiff;

	// draw each pixel in the span
	for (int y = y1; y < y2; y++) {
		int rd = c1rd + (int)(rdx*factor);
		int gr = c1gr + (int)(gdx*factor);
		int bl = c1bl + (int)(bdx*factor);
		int fg = RGBA(rd, gr, bl, 255);
		pb_rgba_set_pixel(pb, x, y, fg);

		factor += factorStep;
	}
}

int raster_rgba_hline(pb_rgba *pb, unsigned int x, unsigned int y, unsigned int length, int value)
{
	size_t terminus = x + length;
	x = x < 0 ? 0 : x;
	terminus = terminus - x;

	unsigned int * data = &((unsigned int *)pb->data)[y*pb->pixelpitch+x];
	size_t count = 1;
	for (size_t idx = 0; idx < terminus; idx++)
	{
		*data = value;
		data++;
	}

	return 0;
}



int raster_rgba_hline_blend(pb_rgba *pb, unsigned int x, unsigned int y, unsigned int length, int value)
{
	size_t terminus = x + length;
	x = x < 0 ? 0 : x;
	terminus = terminus - x;

	unsigned int * data = &((unsigned int *)pb->data)[y*pb->pixelpitch + x];
	size_t count = 1;
	for (size_t idx = 0; idx < terminus; idx++)
	{
		int bg = *data;
		int fg = value;

		*data = blend_color(bg, fg);
		data++;
	}

	return 0;
}

int raster_rgba_vline(pb_rgba *pb, unsigned int x, unsigned int y, unsigned int length, int value)
{
	unsigned int * data = &((unsigned int *)pb->data)[y*pb->frame.width + x];
	size_t count = 1;
	while (count <= length) {
		*data = value;
		data += pb->pixelpitch;
		count++;
	}

	return 0;
}

//#define sgn(x) x < 0 ? -1 : x==0?0 : 1
#define sgn(val) ((0 < val) - (val < 0))

// Bresenham simple line drawing
void raster_rgba_line(pb_rgba *pb, unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, int color)
{
	int dx, dy;
	int i;
	int sdx, sdy, dxabs, dyabs;
	unsigned int x, y, px, py;

	dx = x2 - x1;      /* the horizontal distance of the line */
	dy = y2 - y1;      /* the vertical distance of the line */
	dxabs = abs(dx);
	dyabs = abs(dy);
	sdx = sgn(dx);
	sdy = sgn(dy);
	x = dyabs >> 1;
	y = dxabs >> 1;
	px = x1;
	py = y1;

	pb_rgba_set_pixel(pb, x1, y1, color);

	if (dxabs >= dyabs) /* the line is more horizontal than vertical */
	{
		for (i = 0; i<dxabs; i++)
		{
			y += dyabs;
			if (y >= (unsigned int)dxabs)
			{
				y -= dxabs;
				py += sdy;
			}
			px += sdx;
			pb_rgba_set_pixel(pb, px, py, color);
		}
	}
	else /* the line is more vertical than horizontal */
	{
		for (i = 0; i<dyabs; i++)
		{
			x += dxabs;
			if (x >= (unsigned int)dyabs)
			{
				x -= dyabs;
				px += sdx;
			}
			py += sdy;
			pb_rgba_set_pixel(pb, px, py, color);
		}
	}
}


void raster_rgba_blit(pb_rgba *pb, unsigned int x, unsigned int y, pb_rgba *src)
{
	uint32_t *dstPtr = (uint32_t *)pb->data;
	uint32_t *srcPtr = (uint32_t *)src->data;

	dstPtr += y*pb->pixelpitch + x;

	// IMPROVE
	// use _span

	for (int srcrow = 0; srcrow < src->frame.height; srcrow++)
	{
		// for each row of the source
		// copy to the destination
		memcpy(dstPtr, srcPtr, src->pixelpitch * 4);
		dstPtr += pb->pixelpitch;
		srcPtr += src->pixelpitch;
	}
}

/*
	Bresenham ellipse drawing algorithm
	Only for the frame, not for filling
*/
typedef void(* EllipseHandler)(pb_rgba *pb, const uint32_t cx, const uint32_t cy, const unsigned int x, const unsigned int y, const uint32_t color);

inline void Plot4EllipsePoints(pb_rgba *pb, const uint32_t cx, const uint32_t cy, const unsigned int x, const unsigned int y, const uint32_t color)
{
	pb_rgba_cover_pixel(pb, cx + x, cy + y, color);
	pb_rgba_cover_pixel(pb, cx - x, cy + y, color);
	pb_rgba_cover_pixel(pb, cx - x, cy - y, color);
	pb_rgba_cover_pixel(pb, cx + x, cy - y, color);
}

inline void fill2EllipseLines(pb_rgba *pb, const uint32_t cx, const uint32_t cy, const unsigned int x, const unsigned int y, const uint32_t color)
{
	raster_rgba_hline_blend(pb, cx - x, cy + y, 2*x, color);
	raster_rgba_hline_blend(pb, cx - x, cy - y, 2 * x, color);
}

void raster_rgba_ellipse(pb_rgba *pb, const uint32_t cx, const uint32_t cy, const size_t xradius, size_t yradius, const uint32_t color, EllipseHandler handler)
{
	int x, y;
	int xchange, ychange;
	int ellipseerror;
	int twoasquare, twobsquare;
	int stoppingx, stoppingy;

	twoasquare = 2 * xradius*xradius;
	twobsquare = 2 * yradius*yradius;

	x = xradius;
	y = 0;

	xchange = yradius*yradius*(1 - 2 * xradius);
	ychange = xradius*xradius;
	ellipseerror = 0;
	stoppingx = twobsquare*xradius;
	stoppingy = 0;

	// first set of points, sides
	while (stoppingx >= stoppingy)
	{
		handler(pb, cx, cy, x, y, color);
		y++;
		stoppingy += twoasquare;
		ellipseerror += ychange;
		ychange += twoasquare;
		if ((2 * ellipseerror + xchange) > 0) {
			x--;
			stoppingx -= twobsquare;
			ellipseerror += xchange;
			xchange += twobsquare;
		}
	}

	// second set of points, top and bottom
	x = 0;
	y = yradius;
	xchange = yradius*yradius;
	ychange = xradius*xradius*(1 - 2 * yradius);
	ellipseerror = 0;
	stoppingx = 0;
	stoppingy = twoasquare*yradius;

	while (stoppingx <= stoppingy) {
		handler(pb, cx, cy, x, y, color);
		x++;
		stoppingx += twobsquare;
		ellipseerror += xchange;
		xchange += twobsquare;
		if ((2 * ellipseerror + ychange) > 0) {
			y--;
			stoppingy -= twoasquare;
			ellipseerror += ychange;
			ychange += twoasquare;
		}
	}
}

void raster_rgba_ellipse_stroke(pb_rgba *pb, const uint32_t cx, const uint32_t cy, const size_t xradius, size_t yradius, const uint32_t color)
{
	raster_rgba_ellipse(pb, cx, cy, xradius, yradius, color, Plot4EllipsePoints);
}

void raster_rgba_ellipse_fill(pb_rgba *pb, const uint32_t cx, const uint32_t cy, const size_t xradius, size_t yradius, const uint32_t color)
{
	raster_rgba_ellipse(pb, cx, cy, xradius, yradius, color, fill2EllipseLines);
}
