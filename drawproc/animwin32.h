#pragma once

#include <SDKDDKVer.h>

//#define VC_EXTRALEAN 1
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers

// Windows Header Files:
#include <tchar.h>
#include <windows.h>

#ifdef _WIN32
#pragma comment(lib,"drawproc.lib")
#ifdef _MSC_VER
// This is here so that we can simply use a 'main()' 
#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")
#endif
#endif


#define DPROC_API		__declspec(dllexport)

#ifdef __cplusplus
extern "C" {
#endif



// Called by client code
void * GetPixelBuffer(const int width, const int height);
void * SetWindowSize(const int width, const int height);

void quit();
double seconds();
void resettime();

void setDrawInLoop(bool);
void forceDraw();

// Callback functions
typedef void(*LoopHandler)();
typedef void(*SetupHandler)();
typedef void(*CallToHandler)(); 

DPROC_API void draw();
DPROC_API void setup();

SetupHandler setSetupRoutine(SetupHandler handler);
CallToHandler setLoopRoutine(LoopHandler handler);




typedef LRESULT(CALLBACK* WinProcHandler)(HWND, UINT, WPARAM, LPARAM);
typedef LRESULT(CALLBACK* KeyboardHandler)(HWND, UINT, WPARAM, LPARAM);
typedef LRESULT(CALLBACK*MouseHandler)(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

void setKeyboardHandler(KeyboardHandler handler);
void setMouseHandler(MouseHandler handler);
#ifdef __cplusplus
}
#endif
