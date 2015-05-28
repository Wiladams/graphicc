
#include "drawproc.h"
#include "artt.h"

APolyVertex squareScreen[4];
AVector squareView[4];
APolygon square = {
	pBlue,
	FLAT,	// shading 
	4,
	squareScreen,
	{ 0, 1, 2, 3, 0, 0, 0, 0 }
};


float squareWorld[] = {
	50, -50, -400,
	50, 50, -400,
	-50, 50, -400,
	-50, -50, -400,
};

int viewWidth = 640;
int viewHeight = 480;

AVector camLoc(0, 0, 0);
AVector camDir(-0.2, 0, 0);
real fov = FOV_90;
ACamera cam(camLoc, camDir, viewWidth, viewHeight, fov);
ACanvas *canvas = nullptr;

void setup()
{
	size(viewWidth, viewHeight);
	canvas = new ACanvas(gpb);
	background(pLightGray);

	// draw polygon
	APolyVertex tsc[3];
	tsc[0].x = 10; tsc[0].y = 10;
	tsc[1].x = 100; tsc[1].y = 100;
	tsc[2].x = 10; tsc[2].y = 200;

	APolygon tri = {
		pBlue,
		FLAT,	// shading 
		3,
		tsc,
		{ 0, 2, 1, 0, 0, 0, 0, 0 }
	};

	canvas->drawFlatConvexPolygon(tri);

	//noLoop();
}

void draw()
{
	background(pLightGray);

	// rotate camera around z axis
	AVector camDir = cam.getDirection();
	camDir.z += 0.025;
	cam.setDirection(camDir);
	AVector camLoc = cam.getLocation();
	camLoc.z -= 0.0250;
	cam.setLocation(camLoc);

	// transform and project polygon to screen coordinates
	for (int i = 0; i < 4; i++)
	{
		cam.worldToView(*(AVector *)&squareWorld[i * 3], squareView[i]);
		if (!cam.project(squareView[i], squareScreen[i])) {
			quit();
		}
	}
	canvas->drawFlatConvexPolygon(square);
}