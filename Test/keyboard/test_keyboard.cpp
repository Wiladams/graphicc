/*
test_keyboard

Do some simple mouse and keyboard tracking

*/
#include "drawproc.h"
#include "guistyle.h"
#include <stdio.h>

static GUIStyle styler;
static const int gMaxMode = 3;
static int gMode = 0;
static const int gridSize = 8;

static bool gShowKeyRect = false;
static bool showGrid = false;
static bool gShowCrosshair = true;

PImage *kbImage;
pb_rgba fb;
pb_rect keyRect = { 0, 0, 34, 34 };


/*
Key coordinates

*/
struct keyloc {
	pb_rect loc;
	int vkey;
};

/*
28	Function Keys
100 Numerics
140 QWERTY
*160	+ keypad
180	Caps Lock
220	Shift Keys
*240	Enter Keys
260 Space
*/

struct keyloc locations[] = {
	// Row 1
	{ { 0, 0, 0, 0 }, KC_ESCAPE },
	{ { 0, 0, 0, 0 }, KC_F1 },
	{ { 0, 0, 0, 0 }, KC_F2 },
	{ { 0, 0, 0, 0 }, KC_F3 },
	{ { 0, 0, 0, 0 }, KC_F4 },
	{ { 0, 0, 0, 0 }, KC_F5 },
	{ { 0, 0, 0, 0 }, KC_F6 },
	{ { 0, 0, 0, 0 }, KC_F7 },
	{ { 0, 0, 0, 0 }, KC_F8 },
	{ { 0, 0, 0, 0 }, KC_F9 },
	{ { 0, 0, 0, 0 }, KC_F10 },
	{ { 0, 0, 0, 0 }, KC_F11 },
	{ { 0, 0, 0, 0 }, KC_F12 },
	{ { 0, 0, 0, 0 }, KC_PRINT },
	{ { 0, 0, 0, 0 }, KC_SCROLL },
	{ { 0, 0, 0, 0 }, KC_PAUSE },

	// Row 2
	{ { 0, 0, 0, 0 }, KC_OEM_3 },	// ~
	{ { 0, 0, 0, 0 }, KC_1 },
	{ { 0, 0, 0, 0 }, KC_2 },
	{ { 0, 0, 0, 0 }, KC_3 },
	{ { 0, 0, 0, 0 }, KC_4 },
	{ { 0, 0, 0, 0 }, KC_5 },
	{ { 0, 0, 0, 0 }, KC_6 },
	{ { 0, 0, 0, 0 }, KC_7 },
	{ { 0, 0, 0, 0 }, KC_8 },
	{ { 0, 0, 0, 0 }, KC_9 },
	{ { 0, 0, 0, 0 }, KC_0 },
	{ { 0, 0, 0, 0 }, KC_OEM_MINUS },
	{ { 0, 0, 0, 0 }, KC_OEM_PLUS },
	{ { 0, 0, 0, 0 }, KC_BACK },
	{ { 0, 0, 0, 0 }, KC_INSERT },
	{ { 0, 0, 0, 0 }, KC_HOME },
	{ { 0, 0, 0, 0 }, KC_PRIOR },		// Page Up
	{ { 0, 0, 0, 0 }, KC_NUMLOCK },
	{ { 0, 0, 0, 0 }, KC_DIVIDE },
	{ { 0, 0, 0, 0 }, KC_MULTIPLY },
	{ { 0, 0, 0, 0 }, KC_SUBTRACT },

	// Row 3 - QWERTY
	{ { 0, 0, 0, 0 }, KC_TAB },
	{ { 0, 0, 0, 0 }, KC_Q },
	{ { 0, 0, 0, 0 }, KC_W },
	{ { 0, 0, 0, 0 }, KC_E },
	{ { 0, 0, 0, 0 }, KC_R },
	{ { 0, 0, 0, 0 }, KC_T },
	{ { 0, 0, 0, 0 }, KC_Y },
	{ { 0, 0, 0, 0 }, KC_U },
	{ { 0, 0, 0, 0 }, KC_I },
	{ { 0, 0, 0, 0 }, KC_O },
	{ { 0, 0, 0, 0 }, KC_P },
	{ { 0, 0, 0, 0 }, KC_OEM_4 },	// [{
	{ { 0, 0, 0, 0 }, KC_OEM_6 },	// ]}
	{ { 0, 0, 0, 0 }, KC_OEM_5 },	// \|
	{ { 0, 0, 0, 0 }, KC_DELETE },
	{ { 0, 0, 0, 0 }, KC_END },
	{ { 0, 0, 0, 0 }, KC_NEXT },	// Page Down
	{ { 0, 0, 0, 0 }, KC_NUMPAD7 },	// or Home
	{ { 0, 0, 0, 0 }, KC_NUMPAD8 },	// or Up
	{ { 0, 0, 0, 0 }, KC_NUMPAD9 },	// or PgUp

	// Row - Keypad Plus
	{ { 0, 0, 0, 0 }, KC_ADD },

	// Row 4 - ASDF
	{ { 0, 0, 0, 0 }, KC_CAPITAL },
	{ { 0, 0, 0, 0 }, KC_A },
	{ { 0, 0, 0, 0 }, KC_S },
	{ { 0, 0, 0, 0 }, KC_D },
	{ { 0, 0, 0, 0 }, KC_F },
	{ { 0, 0, 0, 0 }, KC_G },
	{ { 0, 0, 0, 0 }, KC_H },
	{ { 0, 0, 0, 0 }, KC_J },
	{ { 0, 0, 0, 0 }, KC_K },
	{ { 0, 0, 0, 0 }, KC_L },
	{ { 0, 0, 0, 0 }, KC_OEM_1 },	// ;:
	{ { 0, 0, 0, 0 }, KC_OEM_7 },	// '"
	{ { 0, 0, 0, 0 }, KC_RETURN },
	{ { 0, 0, 0, 0 }, KC_NUMPAD4 },	// or LEFT
	{ { 0, 0, 0, 0 }, KC_NUMPAD5 },
	{ { 0, 0, 0, 0 }, KC_NUMPAD6 },	// or RIGHT

	// Row 5 - ZXCV
	{ { 0, 0, 0, 0 }, KC_LSHIFT },
	{ { 0, 0, 0, 0 }, KC_Z },
	{ { 0, 0, 0, 0 }, KC_X },
	{ { 0, 0, 0, 0 }, KC_C },
	{ { 0, 0, 0, 0 }, KC_V },
	{ { 0, 0, 0, 0 }, KC_B },
	{ { 0, 0, 0, 0 }, KC_N },
	{ { 0, 0, 0, 0 }, KC_M },
	{ { 0, 0, 0, 0 }, KC_OEM_COMMA },
	{ { 0, 0, 0, 0 }, KC_OEM_PERIOD },
	{ { 0, 0, 0, 0 }, KC_OEM_2 },		// /?
	{ { 0, 0, 0, 0 }, KC_RSHIFT },
	{ { 0, 0, 0, 0 }, KC_UP },
	{ { 0, 0, 0, 0 }, KC_NUMPAD1 },
	{ { 0, 0, 0, 0 }, KC_NUMPAD2 },
	{ { 0, 0, 0, 0 }, KC_NUMPAD3 },

	// Row - Keypad ENTER
	{ { 0, 0, 0, 0 }, KC_OEM_NEC_EQUAL },

	// Row 5 - SPACE
	{ { 0, 0, 0, 0 }, KC_CONTROL },		// LCONTROL
	{ { 0, 0, 0, 0 }, KC_LWIN },
	{ { 0, 0, 0, 0 }, KC_SPACE },		// LALT
	{ { 0, 0, 0, 0 }, KC_SPACE },
	{ { 0, 0, 0, 0 }, KC_SPACE },		// RALT
	{ { 0, 0, 0, 0 }, KC_RWIN },
	{ { 0, 0, 0, 0 }, KC_MENU },		// RMENU
	{ { 0, 0, 0, 0 }, KC_CONTROL },		// RCONTROL
	{ { 0, 0, 0, 0 }, KC_LEFT },
	{ { 0, 0, 0, 0 }, KC_DOWN },
	{ { 0, 0, 0, 0 }, KC_RIGHT },
	{ { 0, 0, 0, 0 }, KC_NUMPAD0 },
	{ { 0, 0, 0, 0 }, KC_DECIMAL },
};


void  mousePressed()
{
	gMode++;
	if (gMode >= gMaxMode) {
		gMode = 0;
	}
}

void  mouseMoved()
{
	keyRect.x = mouseX - keyRect.width / 2;
	keyRect.y = mouseY - keyRect.height / 2;
}

void  keyReleased()
{
	switch (keyCode)
	{
	case KC_SPACE:
		write_PPM_binary("test_keyboard.ppm", gpb);
		break;

	case KC_F1:
		showGrid = !showGrid;
		break;
	case KC_F2:
		gShowKeyRect = !gShowKeyRect;
		break;
	case KC_F3:
		gShowCrosshair = !gShowCrosshair;
		break;

	case KC_RIGHT: // increase width of keyRect
		keyRect.width += 1;
		break;

	case KC_LEFT:
		keyRect.width -= 1;
		if (keyRect.width < 4) keyRect.width = 4;
		break;
	case KC_UP:
		keyRect.height += 1;
		break;
	case KC_DOWN:
		keyRect.height -= 1;
		if (keyRect.height < 4) keyRect.height = 4;
		break;
	}

	keyRect.x = mouseX - keyRect.width / 2;
	keyRect.y = mouseY - keyRect.height / 2;

}

void drawCrossHairs()
{
	if (!gShowCrosshair) {
		return;
	}

	stroke(84);

	// horizontal line
	line(0, mouseY, width - 1, mouseY);
	line(mouseX, 0, mouseX, height - 1);
}

void drawGrid()
{
	if (!showGrid) {
		return;
	}

	stroke(132, 86);
	for (int x = 0; x < width; x++)
	{
		if (x % gridSize == 0) {
			line(x, 0, x, height - 1);
		}
	}

	for (int y = 0; y < height; y++)
	{
		if (y % gridSize == 0) {
			line(0, y, width-1, y);
		}
	}
}
// Draw information about the mouse
// location, buttons pressed, etc
void drawMouseInfo()
{
	// draw a white banner across the bottom
	noStroke();
	fill(pWhite);
	rect(0, height - 1 - 34, width, 34);

	if (gShowKeyRect) {
		// draw the key rectangle
		fill(COLOR(255, 238, 200, 180));
		stroke(pDarkGray);
		rect(keyRect.x, keyRect.y, keyRect.width, keyRect.height);
	}

	// select verdana font
	setFont(verdana17);
	char infobuff[256];
	sprintf_s(infobuff, "Mouse X: %3d Y: %3d    Key: (%3f, %3f)(%3.0f, %3.0f)  Key Code: 0x%x", mouseX, mouseY, 
		keyRect.x, keyRect.y, keyRect.width, keyRect.height,
		keyCode);
	fill(pBlack);
	textAlign(TX_LEFT, TX_TOP);
	text(infobuff, 0, height-34);

}

void draw()
{
	background(pLightGray);
	image(kbImage, 0, 0);

	drawCrossHairs();
	drawGrid();

	drawMouseInfo();
}

void setup()
{
	// Adjust this path to match where the keyboard image file is
	kbImage = loadImage("c:/repos/graphicc/Test/windows-keyboard-60-keys.ppm", nullptr);
	
	size(kbImage->fb.frame.width, kbImage->fb.frame.height+4+30);
	background(pLightGray);
}


