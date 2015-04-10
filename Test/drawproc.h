#pragma once

#define BGR_DOMINANT 1

#include "animwin32.h"
#include "test_common.h"
#include "agg_embedded_raster_fonts.h"

/*
#ifdef __cplusplus
extern "C" {
#endif
	// MUST BE IMPLEMENTED BY TEST CASE
	void draw();
	void setup();

#ifdef __cplusplus
}
#endif
*/

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

extern pb_rgba *gpb;
extern pb_rect pixelFrame;



extern uint64_t startcount;	// for time keeping
extern uint64_t frequency;

extern uint32_t bgColor;

extern int grectMode;
extern int gellipseMode;

// Initialization
void init();

// Drawing API
void size(const size_t width, const size_t height);



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

// Text
extern font_t gfont;

// createFont
// loadFont()
void setFont(const uint8_t *fontdata);
void text(const char *str, const int x, const int y);
//void textFont(font_t *font);
// textAlign()
// textLeading()
// textMode()
// textSize()
// textWidth()
// textAscent()
// textDescent()


// Math
int random(static const int);