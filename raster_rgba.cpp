#include "raster_rgba.h"
#include <math.h>
#include <string.h>
#include <stdlib.h>

int raster_rgba_hline(pb_rgba *pb, unsigned int x, unsigned int y, unsigned int length, int value)
{
	unsigned int * data = &((unsigned int *)pb->data)[y*pb->pixelpitch+x];
	size_t count = 1;
	while (count < length) {
		*data = value;
		data++;
		count++;
	}

	return 0;
}

int raster_rgba_vline(pb_rgba *pb, unsigned int x, unsigned int y, unsigned int length, int value)
{
	unsigned int * data = &((unsigned int *)pb->data)[y*pb->frame.width + x];
	size_t count = 1;
	while (count < length) {
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
	int i, dx, dy;
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
			if (y >= dxabs)
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
			if (x >= dyabs)
			{
				x -= dyabs;
				px += sdx;
			}
			py += sdy;
			pb_rgba_set_pixel(pb, px, py, color);
		}
	}
}

void raster_rgba_rect_fill(pb_rgba *pb, unsigned int x1, unsigned int y1, unsigned int width, unsigned int height, int value)
{
	size_t counter = 1;
	while (counter < height) {
		raster_rgba_hline(pb, x1, y1 + counter - 1, width, value);
		counter++;
	}
}

void raster_rgba_blit(pb_rgba *pb, unsigned int x, unsigned int y, pb_rgba *src)
{
	unsigned int *dstPtr = (unsigned int *)pb->data;
	unsigned int *srcPtr = (unsigned int *)src->data;

	dstPtr += y*pb->pixelpitch + x;

	for (size_t srcrow = 0; srcrow < src->frame.height; srcrow++)
	{
		// for each row of the source
		// copy to the destination
		memcpy(dstPtr, srcPtr, src->pixelpitch * 4);
		dstPtr += pb->pixelpitch;
		srcPtr += src->pixelpitch;
	}
}
