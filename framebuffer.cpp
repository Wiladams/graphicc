#include "framebuffer.h"
#include <stdlib.h>

int fb_rgba_init(fb_rgba *fb, const int width, const int height)
{
	if (!fb) return -1;

	size_t pitch = width * 4;
	size_t datasize = height * pitch;

	fb->data = (unsigned char *)calloc(datasize, 1);
	fb->height = height;
	fb->width = width;
	fb->pitch = pitch;
	fb->pformat = rgba;

	return 0;
}

int fb_rgba_free(fb_rgba *fb)
{
	if (!fb) return -1;
	if (!fb->data) return -1;

	free(fb->data);
	fb->data = 0;
	fb->height = 0;
	fb->width = 0;
	fb->pitch = 0;

	return 0;
}

int fb_rgba_set_pixel(fb_rgba *fb, unsigned int x, unsigned int y, unsigned int value)
{
	unsigned int * data = (unsigned int *)&fb->data[y*fb->pitch];
	data[x] = value;

	return 0;
}

int fb_rgba_get_pixel(fb_rgba *fb, unsigned int x, unsigned int y, unsigned int *value)
{
	unsigned int * data = (unsigned int *)&fb->data[y*fb->pitch];
	*value = data[x];

	return 0;
}