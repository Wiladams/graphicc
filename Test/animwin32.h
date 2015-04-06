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

#define BGR_DOMINANT 1
#include "test_common.h"

#ifdef __cplusplus
extern "C" {
#endif

// MUST BE IMPLEMENTED BY TEST CASE
void draw();
void setup();

void quit();

// Callback functions
typedef int (*KeyboardHandler)(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
typedef int (*MouseHandler)(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

void setKeyboardHandler(KeyboardHandler handler);
void setOnKeyPressed(KeyboardHandler handler);
void setOnKeyReleased(KeyboardHandler handler);
void setOnKeyTyped(KeyboardHandler handler);

void setMouseHandler(MouseHandler handler);
void setOnMouseDownHandler(MouseHandler handler);
void setOnMouseWheelHandler(MouseHandler handler);

#ifdef __cplusplus
}
#endif



/*
	Drawing API
*/
enum {
	CORNER,
	CORNERS,
	RADIUS,
	CENTER
};

typedef enum GEOMETRY {
	GR_POINTS,
	GR_LINES,
	GR_LINE_STRIP,
	GR_LINE_LOOP,
	GR_POLYGON,
	GR_QUADS,
	GR_QUAD_STRIP,
	GR_TRIANGLES,
	GR_TRIANGLE_STRIP,
	GR_TRIANGLE_FAN
};

// Global variables
extern size_t width;
extern size_t height;

extern pb_rect pixelFrame;



extern uint64_t startcount;	// for time keeping
extern uint64_t frequency;

extern uint32_t bgColor;

extern int grectMode;
extern int gellipseMode;

// Drawing API
void size(const size_t width, const size_t height);

double seconds();
void resettime();

// Mouse
// Mouse
extern int mouseX;
extern int mouseY;
// mouseButton
// mouseClicked()
// mouseDragged()
// mouseMoved()
// mousePressed()
// mousePressed
// mouseReleased()
// mouseWheel()
// mouseX
// mouseY
// pmouseX
// pmouseY

// Keyboard
extern int key;
extern int keyCode;
extern int isKeyPressed;
// keyPressed()
// keyReleased()
// keyTyped()

// Setting colors
void background(const uint32_t value);
//void clear();
//void colorMode();
void fill(const uint32_t value);
void fill(const uint8_t gray);
void noFill();
void noStroke();
void stroke(const uint32_t value);
void stroke(const uint8_t gray);

// attributes
void ellipseMode(const int mode);
//void noSmooth();
void rectMode(const int mode);
//void smooth();
//void strokeCap();
//void strokeJoin();
//void strokeWeight();


// 2D primitives
void bezier(const int x1, const int y1, const int x2, const int y2, const int x3, const int y3);
void ellipse(const int a, const int b, const int c, const int d);
void line(const int x1, const int y1, const int x2, const int y2);
void lineloop(const size_t nPtr, const int *pts);
void point(const int x, const int y);
void rect(const int a, const int b, const int c, const int d);
void quad(const int x1, const int y1, const int x2, const int y2, const int x3, const int y3, const int x4, const int y4); 
void triangle(const int x1, const int y1, const int x2, const int y2, const int x3, const int y3);