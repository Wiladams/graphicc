#pragma once

#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <string.h>

enum pixellayouts {
	rgba
};

/*
	A framebuffer is an array of pixels.

	The pixelformat determines the layout of the
	individual elements.

	data - pointer to the beginning of the data
	pformat - some indication of the pixel layout
	width - number of pixels wide
	height - number of pixels high
	pitch - number of bytes between rows
*/

typedef struct _fb_rgba {
	unsigned char *		data;
	int					pformat;
	unsigned int		width;
	unsigned int		height;
	unsigned int		pitch;
} fb_rgba;

typedef struct _pix_rgba { 
	unsigned char r, g, b, a; 
} pix_rgba;

#define RGBA(r,g,b,a) ((unsigned int)(r<<24|g<<16|b<<8|a))

#ifdef __cplusplus
extern "C" {
#endif

int fb_rgba_init(fb_rgba *fb, const int width, const int height);
int fb_rgba_free(fb_rgba *fb);

int fb_rgba_set_pixel(fb_rgba *fb, unsigned int x, unsigned int y, unsigned int value);
int fb_rgba_get_pixel(fb_rgba *fb, unsigned int x, unsigned int y, unsigned int *value);

#ifdef __cplusplus
}
#endif

#endif // FRAMEBUFFER_H
