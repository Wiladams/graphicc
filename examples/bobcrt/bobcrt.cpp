#include "drawproc.h"

#include "Tracer.h"

Tracer onetrace;
static bool finishedFrame = false;

void setup()
{
	size(512, 512);
	
	onetrace.width = width;
	onetrace.height = height;
}


//void(*setpixel)(int x, int y, unsigned int value);
void setpixel(int x, int y, int r, int g, int b)
{
	pb_rgba_cover_pixel(gpb, x, y, RGBA(r, g, b, 255));
}

void draw() 
{
	// draw frames until we have a complete image
	if (!finishedFrame)
		finishedFrame = onetrace.ContinueFrame(setpixel);

}
