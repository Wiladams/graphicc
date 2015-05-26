#pragma once

#include <SDKDDKVer.h>

//#define VC_EXTRALEAN 1
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers

// Windows Header Files:
#include <tchar.h>
#include <windows.h>

//#pragma comment(lib,"drawproc.lib")


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
typedef void(*CallToHandler)();

typedef LRESULT(CALLBACK* WinProcHandler)(HWND, UINT, WPARAM, LPARAM);
typedef LRESULT(CALLBACK* KeyboardHandler)(HWND, UINT, WPARAM, LPARAM);
typedef LRESULT(CALLBACK*MouseHandler)(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

void setKeyboardHandler(KeyboardHandler handler);
void setMouseHandler(MouseHandler handler);
CallToHandler setSetupRoutine(CallToHandler handler);
CallToHandler setLoopRoutine(CallToHandler handler);
#ifdef __cplusplus
}
#endif
