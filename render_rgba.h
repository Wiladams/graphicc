#pragma once

#ifndef RENDER_RGBA_H
#define RENDER_RGBA_H 1

#include "pixelbuffer.h"

#ifdef __cplusplus
extern "C" {
#endif
	
//int render_rgba_get_pixel(pb_rgba *fb, unsigned int x, unsigned int y, unsigned int *value);
#define render_rgba_get_pixel(pb, x, y, value) *value = ((unsigned int *)(pb)->data)[(y*(pb)->width)+x]

//void render_rgba_set_pixel(pb_rgba *fb, unsigned int x, unsigned int y, unsigned int value);
#define render_rgba_set_pixel(pb, x, y, value) ((unsigned int *)(pb)->data)[(y*(pb)->width)+x] = value

int render_rgba_hline(pb_rgba *pb, unsigned int x, unsigned int y, unsigned int length, int value);
int render_rgba_vline(pb_rgba *pb, unsigned int x, unsigned int y, unsigned int length, int value);
void render_rgba_line(pb_rgba *pb, unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, int value);

#ifdef __cplusplus
}
#endif

#endif
