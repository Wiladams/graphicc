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

// MUST BE IMPLEMENTED BY TEST CASE
void step(pb_rgba *pb);
void setup(size_t &width, size_t &height);

#ifdef __cplusplus
}
#endif
