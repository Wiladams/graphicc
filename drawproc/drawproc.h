#pragma once

#define BGR_DOMINANT 1

#include "animwin32.h"

#include "graphicc.h"
#include "linearalgebra.h"
#include "raster_rgba.h"

#include "agg_embedded_raster_fonts.h"

#ifdef _WIN32
#pragma comment(lib,"drawproc.lib")
	#ifdef _MSC_VER
		// This is here so that we can simply use a 'main()' 
		#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")
	#endif
#endif

typedef void(CALLBACK *EventObserverHandler)();


// IO Event Handlers
void setOnKeyPressedHandler(KeyboardHandler handler);
void setOnKeyReleasedHandler(KeyboardHandler handler);
void setOnKeyTypedHandler(KeyboardHandler handler);

void setOnMousePressedHandler(EventObserverHandler handler);
void setOnMouseWheelHandler(MouseHandler handler);
void setOnMouseMovedHandler(EventObserverHandler handler);
void setOnMouseDraggedHandler(MouseHandler handler);


void redraw();

/*
Drawing API
*/
typedef enum RECTMODE{
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
	STROKE,
	CLOSE
};

typedef float coord;

struct Vector2d
{
	Vector2d(coord x, coord y)
	:_x(x), _y(y)
	{};

	coord _x;
	coord _y;
};

struct Vector3d
{
	union {
		struct {
			coord _x, _y, _z;
		};
		coord _v[3];
	};
};

// Global variables
extern size_t width;
extern size_t height;

extern pb_rgba *gpb;
extern pb_rect pixelFrame;



extern uint64_t startcount;	// for time keeping
extern uint64_t frequency;

extern uint32_t bgColor;
extern pb_rgba *bgImage;

extern RECTMODE grectMode;
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
// pmouseX
// pmouseY

// Keyboard
extern int key;
extern int keyCode;
extern bool isKeyPressed;
// keyPressed()
// keyReleased()
// keyTyped()

// Setting colors
void background(const uint32_t value);
void backgroundImage(pb_rgba *bg);
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
void rectMode(const RECTMODE mode);
//void smooth();
//void strokeCap();
//void strokeJoin();
void strokeWeight(const float weight);


// 2D primitives
void bezier(const int x1, const int y1, const int x2, const int y2, const int x3, const int y3, const int segments=60);
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
void bezierVertex(const int x1, const int y1, const int x2, const int y2, const int x3, const int y3);
void endShape(const int kindOfClose = STROKE);

// Math
int random(const int);

// Some color manipulation routines
inline uint32_t darker(const uint32_t acolor, const float factor = 0.60)
{
	uint8_t red = GET_R(acolor) *factor;
	uint8_t green = GET_G(acolor) * factor;
	uint8_t blue = GET_B(acolor) * factor;
	uint8_t alpha = GET_A(acolor);

	return RGBA(red, green, blue, alpha);
}

inline uint32_t brighter(const uint32_t acolor, const float factor=0.80)
{
	uint8_t red = (min(GET_R(acolor) *(1.0 / factor), 255));
	uint8_t green = (min(GET_G(acolor) * (1.0 / 0.85), 255));
	uint8_t blue = (min(GET_B(acolor) * (1.0 / factor), 255));
	uint8_t alpha = GET_A(acolor);

	return RGBA(red, green, blue, alpha);
}


#define pRed		RGBA(255, 0, 0, 255)
#define pGreen		RGBA(0, 255, 0, 255)
#define pBlue		RGBA(0, 0, 255, 255)

#define pBlack		RGBA(0, 0, 0, 255)
#define pWhite		RGBA(255, 255, 255, 255)
#define pYellow		RGBA(255, 255, 0, 255)
#define pTurquoise	RGBA(0, 255, 255, 255)
#define pDarkGray	RGBA(93, 93, 93, 255)
#define pGray		RGBA(127,127,127,255)
#define pLightGray	RGBA(163, 163, 163, 255)

#define aliceblue	RGBA(240, 248, 255, 255)
#define cornsilk	RGBA(255, 248, 220, 255)

