#include "pixelbuffer.h"

#include <stdlib.h>

int pb_rgba_init(pb_rgba *fb, const int width, const int height)
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

int pb_rgba_free(pb_rgba *fb)
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

