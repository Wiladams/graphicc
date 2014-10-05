#include "render_rgba.h"

int render_rgba_get_pixel(pb_rgba *fb, unsigned int x, unsigned int y, unsigned int *value)
{
	unsigned int * data = (unsigned int *)&fb->data[y*fb->pitch];
	*value = data[x];

	return 0;
}

int render_rgba_set_pixel(pb_rgba *fb, unsigned int x, unsigned int y, unsigned int value)
{
	unsigned int * data = (unsigned int *)&fb->data[y*fb->pitch];
	data[x] = value;

	return 0;
}



int render_rgba_hline(pb_rgba *pb, unsigned int x, unsigned int y, unsigned int length, int value)
{
	for (int offset = x; offset < x + length; offset++)
	{
		render_rgba_set_pixel(pb, x + offset, y, value);
	}

	return 0;
}
