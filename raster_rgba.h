#pragma once

#ifndef RASTER_RGBA_H
#define RASTER_RGBA_H

#include "pixelbuffer.h"

// turn a division by 255 into something 
// much cheaper to calculate
// for values between 0 and 65534
#define div255(num) ((num + (num >> 8)) >> 8)


#ifdef __cplusplus
extern "C" {
#endif

void raster_rgba_span(pb_rgba *pb, const uint32_t x, const uint32_t y, const size_t len, const uint32_t *data);

// SRCCOPY
int raster_rgba_hline(pb_rgba *pb, unsigned int x, unsigned int y, unsigned int length, int value);
int raster_rgba_vline(pb_rgba *pb, unsigned int x, unsigned int y, unsigned int length, int value);
void raster_rgba_line(pb_rgba *pb, unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, int value);

void raster_rgba_hline_fade(pb_rgba *pb, int x1, int color1, int x2, int color2, int y);
void raster_rgba_vline_fade(pb_rgba *pb, int y1, int color1, int y2, int color2, int x);

// SRCOVER
int raster_rgba_hline_blend(pb_rgba *pb, unsigned int x, unsigned int y, unsigned int length, int value);



void raster_rgba_triangle_fill(pb_rgba *pb,
	const unsigned int x1, const unsigned int  y1,
	const unsigned int  x2, const unsigned int  y2,
	const unsigned int  x3, const unsigned int  y3,
	int color);

void raster_rgba_blit(pb_rgba *pb, unsigned int x, unsigned int y, pb_rgba *src);

#ifdef __cplusplus
}
#endif


#define raster_rgba_rect_fill(pb, x1, y1, width, height, value) for (size_t idx = 0; idx < height; idx++){raster_rgba_hline(pb, x1, y1 + idx, width, value);	}															
#define raster_rgba_rect_fill_blend(pb, x1, y1, width, height, value) for (size_t idx = 0; idx < height; idx++){raster_rgba_hline_blend(pb, x1, y1 + idx, width, value);	}															


#endif
