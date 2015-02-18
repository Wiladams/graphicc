
#include "gfx.h"



static pb_rgba gfx_gc;

static int g_MouseX = 0;
static int g_MouseY = 0;

static uint32_t g_ForegroundColor;
static uint32_t g_BackgroundColor;
static int g_Width;
static int g_Height;
static int g_xpos = 0;
static int g_ypos = 0;


// Initialize the graphics context
int gfx_open(const size_t width, const size_t height, const char *title)
{
	g_Width = width;
	g_Height = height;

	pb_rgba_init(&gfx_gc, g_Width, g_Height);

	// Set the foreground and background colors
	g_ForegroundColor = RGBA(255, 255, 255, 255);
	g_BackgroundColor = RGBA(0, 0, 0, 255);


	// Clear background to black
	gfx_clear();
}

// Draw a single point at (x,y)
void gfx_point(const int x, const int y)
{
	pb_rgba_set_pixel(&gfx_gc, x, y, g_ForegroundColor);
}

// Draw a line from (x1,y1) to (x2,y2) 
void gfx_line(const int x1, const int y1, const int x2, const int y2)
{
	raster_rgba_line(&gfx_gc, x1, y1, x2, y2, g_ForegroundColor);
}

// Change the current drawing color.
void gfx_color(const uint8_t r, const uint8_t g, const uint8_t b)
{
	g_ForegroundColor = RGBA(r, g, b, 255);
}

// Clear the graphics window to the background color.
void gfx_clear()
{
	raster_rgba_rect_fill(&gfx_gc, 0, 0, g_Width, g_Height, g_BackgroundColor);
}

// Change the current background color.
void gfx_clear_color(int r, int g, int b)
{
	g_BackgroundColor = RGBA(r, g, b, 255);
}

int gfx_event_waiting()
{
}

/* Wait for the user to press a key or mouse button. */

char gfx_wait()
{
}

/* Return the X and Y coordinates of the last event. */

int gfx_xpos()
{
	return g_xpos;
}

int gfx_ypos()
{
	return g_ypos;
}

// Flush all previous output to the window.
void gfx_flush()
{
}
