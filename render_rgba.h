#pragma once

#ifndef RENDER_RGBA_H
#define RENDER_RGBA_H 1

#include "pixelbuffer.h"

#ifdef __cplusplus
extern "C" {
#endif
	
int render_rgba_get_pixel(pb_rgba *fb, unsigned int x, unsigned int y, unsigned int *value);
	
int render_rgba_set_pixel(pb_rgba *fb, unsigned int x, unsigned int y, unsigned int value);
int render_rgba_hline(pb_rgba *pb, unsigned int x, unsigned int y, unsigned int length, int value);

#ifdef __cplusplus
}
#endif

#endif
