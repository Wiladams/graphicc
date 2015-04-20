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
#include "graphicc.h"


#ifdef __cplusplus
extern "C" {
#endif

// MUST BE IMPLEMENTED BY TEST CASE
void draw();
void setup();


// Called by client code
void * GetPixelBuffer(const int width, const int height);
void * SetWindowSize(const int width, const int height);

void quit();
double seconds();
void resettime();

void setDrawInLoop(bool);
void forceDraw();

// Callback functions
typedef LRESULT(CALLBACK* WinProcHandler)(HWND, UINT, WPARAM, LPARAM);
typedef LRESULT(CALLBACK* KeyboardHandler)(HWND, UINT, WPARAM, LPARAM);
typedef LRESULT(CALLBACK*MouseHandler)(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
typedef void(CALLBACK *EventObserverHandler)();

void setOnIdleHandler(EventObserverHandler handler);

void setKeyboardHandler(KeyboardHandler handler);
void setOnKeyPressed(KeyboardHandler handler);
void setOnKeyReleasedHandler(KeyboardHandler handler);
void setOnKeyTypedHandler(KeyboardHandler handler);

void setMouseHandler(MouseHandler handler);
void setOnMousePressedHandler(EventObserverHandler handler);
void setOnMouseWheelHandler(MouseHandler handler);
void setOnMouseMovedHandler(MouseHandler handler);
void setOnMouseDraggedHandler(MouseHandler handler);


#ifdef __cplusplus
}
#endif
