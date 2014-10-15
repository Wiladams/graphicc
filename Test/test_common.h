#include "linearalgebra.h"
#include "pixelbuffer.h"
#include "raster_rgba.h"
#include "pbm.h"

#include <stdio.h>

#define pRed		RGBA(255, 0, 0, 255)
#define pGreen		RGBA(0, 255, 0, 255)
#define pBlue		RGBA(0, 0, 255, 255)

#define pBlack		RGBA(0, 0, 0, 255)
#define pWhite		RGBA(255, 255, 255, 255)
#define pYellow		RGBA(255, 255, 0, 255)
#define pTurquoise	RGBA(0, 255, 255, 255)
#define pDarkGray	RGBA(93, 93, 93, 255)
#define pLightGray	RGBA(163, 163, 163, 255)
