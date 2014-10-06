#pragma once

#ifndef RASTER_RGBA_H
#define RASTER_RGBA_H 1

#include "pixelbuffer.h"

#ifdef __cplusplus
extern "C" {
#endif

int raster_rgba_hline(pb_rgba *pb, unsigned int x, unsigned int y, unsigned int length, int value);
int raster_rgba_vline(pb_rgba *pb, unsigned int x, unsigned int y, unsigned int length, int value);
void raster_rgba_line(pb_rgba *pb, unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, int value);

void raster_rgba_rect_fill(pb_rgba *pb, unsigned int x1, unsigned int y1, unsigned int width, unsigned int height, int value);

void raster_rgba_blit(pb_rgba *pb, unsigned int x, unsigned int y, pb_rgba *src);

#ifdef __cplusplus
}
#endif

#endif
