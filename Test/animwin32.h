#pragma once

#include <SDKDDKVer.h>

//#define VC_EXTRALEAN 1
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers

// Windows Header Files:
#include <windows.h>

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#include "test_common.h"

#ifdef __cplusplus
extern "C" {
#endif

// MUST BE IMPLEMENTED BY TEST CASE
void step(pb_rgba *pb);
void setup();

#ifdef __cplusplus
}
#endif


// Global variables
extern size_t width;
extern size_t height;

extern uint64_t startcount;	// for time keeping
extern uint64_t frequency;

extern uint32_t bgColor;


// Drawing API
void size(const size_t width, const size_t height);

double seconds();
void resettime();

void background(const uint32_t value);
void stroke(const uint32_t value);
void stroke(const uint8_t gray);

// 2D primitives
void ellipse(const int a, const int b, const int c, const int d);
void line(const int x1, const int y1, const int x2, const int y2);
void point(const int x, const int y);