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

#ifndef RASTER_RGBA_H
#define RASTER_RGBA_H

#include "pixelbuffer.h"


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
