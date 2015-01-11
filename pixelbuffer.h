#pragma once

#ifndef PIXELBUFFER_H
#define PIXELBUFFER_H

#include "graphicc.h"



typedef struct _pix_rgba {
	unsigned char r, g, b, a;
} pix_rgba;

// On a little endian machine
// Stuff it such that 
// byte 0 == red
// byte 1 == green
// byte 2 == blue
// byte 3 == alpha
#define RGBA(r,g,b,a) ((unsigned int)(a<<24|b<<16|g<<8|r))
#define GET_R(value) ((unsigned int)value &0xff)
#define GET_G(value) (((unsigned int)value &0xff00) >> 8)
#define GET_B(value) (((unsigned int)value &0xff0000) >> 16)
#define GET_A(value) (((unsigned int)value &0xff000000) >> 24)


/*
A pixel buffer is an array of pixels.

The pixelformat determines the layout of the
individual elements.

data - pointer to the beginning of the data
pformat - some indication of the pixel layout
width - number of pixels wide
height - number of pixels high
pixelpitch - number of pixels between rows
*/

// pixel buffer rectangle
typedef struct _pb_rect {
	unsigned int x, y, width, height;
} pb_rect;

typedef struct _pb_rgba {
	unsigned char *		data;
	unsigned int		pixelpitch;
	int					owndata;
	pb_rect				frame;
} pb_rgba;






#ifdef __cplusplus
extern "C" {
#endif

int pb_rgba_init(pb_rgba *pb, const unsigned int width, const unsigned int height);
int pb_rgba_free(pb_rgba *pb);

int pb_rgba_get_frame(pb_rgba *pb, const unsigned int x, const unsigned int y, const unsigned int width, const unsigned int height, pb_rgba *pf);


#ifdef __cplusplus
}
#endif

#define pb_rgba_get_pixel(pb, x, y, value) *value = ((unsigned int *)(pb)->data)[(y*(pb)->pixelpitch)+x]
#define pb_rgba_set_pixel(pb, x, y, value) ((unsigned int *)(pb)->data)[(y*(pb)->pixelpitch)+x] = value


#define pb_rect_contains(rect, x, y) ((x>=(rect)->x && x<= (rect)->x+(rect)->width) && ((y>=(rect)->y) && (y<=(rect)->y+(rect)->height)))
#define pb_rect_clear(rect) memset((rect), 0, sizeof(pb_rect))

#endif // PIXELBUFFER_H
