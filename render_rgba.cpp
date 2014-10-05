#include <math.h>
#include "render_rgba.h"



int render_rgba_hline(pb_rgba *pb, unsigned int x, unsigned int y, unsigned int length, int value)
{
	unsigned int * data = &((unsigned int *)pb->data)[y*pb->width+x];
	size_t count = 1;
	while (count < length) {
		*data = value;
		data++;
		count++;
	}

	return 0;
}

int render_rgba_vline(pb_rgba *pb, unsigned int x, unsigned int y, unsigned int length, int value)
{
	unsigned int * data = &((unsigned int *)pb->data)[y*pb->width + x];
	size_t count = 1;
	while (count < length) {
		*data = value;
		data += pb->width;
		count++;
	}

	return 0;
}

//#define sgn(x) x < 0 ? -1 : x==0?0 : 1
#define sgn(val) ((0 < val) - (val < 0))

// Bresenham simple line drawing
void render_rgba_line(pb_rgba *pb, unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, int color)
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

	render_rgba_set_pixel(pb, x1, y1, color);

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
			render_rgba_set_pixel(pb, px, py, color);
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
			render_rgba_set_pixel(pb, px, py, color);
		}
	}
}

void render_rgba_rect_fill(pb_rgba *pb, unsigned int x1, unsigned int y1, unsigned int width, unsigned int height, int value)
{
	size_t counter = 1;
	while (counter < height) {
		render_rgba_hline(pb, x1, y1 + counter - 1, width, value);
		counter++;
	}
}