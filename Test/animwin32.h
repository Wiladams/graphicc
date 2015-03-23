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

HWND CreateWindowHandle(int width, int height);

// Change global variables of environment
void size(const size_t width, const size_t height);

// MUST BE IMPLEMENTED BY TEST CASE
void step(pb_rgba *pb);
void setup();



#ifdef __cplusplus
}
#endif

// Global variables
extern size_t width;
extern size_t height;