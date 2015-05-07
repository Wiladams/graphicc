#pragma once

#define BGR_DOMINANT 1

#include "animwin32.h"
#include "test_common.h"
#include "agg_embedded_raster_fonts.h"


void redraw();

/*
Drawing API
*/
enum {
	CORNER,
	CORNERS,
	RADIUS,
	CENTER
};

typedef enum ALIGNMENT {
	TX_CENTER	= 0x00,
	TX_LEFT		= 0x01,
	TX_RIGHT	= 0x04,
	TX_TOP		= 0x10,
	TX_BOTTOM	= 0x40

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

typedef enum KindOfClose {
	OPEN,
	CLOSE
};

class Vector2d
{
public:
	Vector2d(float x, float y)
	{
		Set(x, y);
	};

	float GetX(void) const { return mX; };

	float GetY(void) const { return mY; };

	void  Set(float x, float y)
	{
		mX = x;
		mY = y;
	};
private:
	float mX;
	float mY;
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
void noLoop();
void loop();

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
//void fill(const uint8_t gray);
void fillGray(const uint8_t gray);
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
void strokeWeight(const float weight);


// 2D primitives
void bezier(const int x1, const int y1, const int x2, const int y2, const int x3, const int y3);
void ellipse(const int a, const int b, const int c, const int d);
void line(const int x1, const int y1, const int x2, const int y2);
void lineloop(const size_t nPtr, const int *pts);
void point(const int x, const int y);
void rect(const int a, const int b, const int c, const int d);
void quad(const int x1, const int y1, const int x2, const int y2, const int x3, const int y3, const int x4, const int y4);
void triangle(const int x1, const int y1, const int x2, const int y2, const int x3, const int y3);
void polygon(int nverts, int *a);

// Text
extern font_t gfont;

// createFont
// loadFont()
void setFont(const uint8_t *fontdata);
void text(const char *str, const int x, const int y);
//void textFont(font_t *font);
void textAlign(const int alignX=TX_LEFT, const int alignY = TX_BOTTOM);
// textLeading()
// textMode()
void textSize(const int size);
// textWidth()
// textAscent()
// textDescent()

// Shape
void beginShape(const int shapeKind = GR_POLYGON);
void vertex(const int x, const int y);
void endShape(const int kindOfClose = OPEN);

// Math
int random(static const int);