#pragma once

#ifndef PIXELBUFFER_H
#define PIXELBUFFER_H


enum pixellayouts {
	rgba
};

typedef struct _pix_rgba {
	unsigned char r, g, b, a;
} pix_rgba;

// On a little endian machine
#define RGBA(r,g,b,a) ((unsigned int)(a<<24|b<<16|g<<8|r))
#define GET_R(value) (unsigned int)value &0xff
#define GET_G(value) ((unsigned int)value &0xff00) >> 8
#define GET_B(value) ((unsigned int)value &0xff0000) >> 16
#define GET_A(value) ((unsigned int)value &0xff000000) >> 24

/*
A pixel buffer is an array of pixels.

The pixelformat determines the layout of the
individual elements.

data - pointer to the beginning of the data
pformat - some indication of the pixel layout
width - number of pixels wide
height - number of pixels high
pitch - number of bytes between rows
*/

typedef struct _pb_rgba {
	unsigned char *		data;
	int					pformat;
	unsigned int		width;
	unsigned int		height;
	unsigned int		pitch;
} pb_rgba;



#ifdef __cplusplus
extern "C" {
#endif

int pb_rgba_init(pb_rgba *fb, const int width, const int height);
int pb_rgba_free(pb_rgba *fb);

#ifdef __cplusplus
}
#endif

#endif // PIXELBUFFER_H
