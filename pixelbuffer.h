/*
Copyright 2015 William A Adams

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http ://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#pragma once

#ifndef PIXELBUFFER_H
#define PIXELBUFFER_H

#include "graphicc.h"



typedef struct _pix_rgba {
	uint8_t r, g, b, a;
} pix_rgba;

// On a little endian machine
// Stuff it such that 
// byte 0 == red
// byte 1 == green
// byte 2 == blue
// byte 3 == alpha
#ifdef BGR_DOMINANT
#define RGBA(r,g,b,a) ((uint32_t)(a<<24|r<<16|g<<8|b))
#define GET_B(value) ((uint32_t)value &0xff)
#define GET_G(value) (((uint32_t)value &0xff00) >> 8)
#define GET_R(value) (((uint32_t)value &0xff0000) >> 16)
#define GET_A(value) (((uint32_t)value &0xff000000) >> 24)
#else
#define RGBA(r,g,b,a) ((uint32_t)(a<<24|b<<16|g<<8|r))
#define GET_R(value) ((uint32_t)value &0xff)
#define GET_G(value) (((uint32_t)value &0xff00) >> 8)
#define GET_B(value) (((uint32_t)value &0xff0000) >> 16)
#define GET_A(value) (((uint32_t)value &0xff000000) >> 24)
#endif


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

typedef struct _pb_rgba {
	uint8_t *		data;
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

void pb_rgba_cover_pixel(pb_rgba *pb, const unsigned int x, const unsigned int y, const uint32_t value);

#ifdef __cplusplus
}
#endif

#define pb_rgba_get_pixel(pb, x, y, value) *value = ((uint32_t *)(pb)->data)[(y*(pb)->pixelpitch)+x]
#define pb_rgba_set_pixel(pb, x, y, value) ((uint32_t *)(pb)->data)[(y*(pb)->pixelpitch)+x] = value



#endif // PIXELBUFFER_H
