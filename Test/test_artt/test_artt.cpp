
#include "drawproc.h"
#include "artt.h"

void setup()
{
	size(640, 480);

	AVector v(0, 0, 50);
	AMatrix m(10, 20, 30, 100, 200, 300, 9, 5, 3);
	AVector vresult = v * m;
}

void draw()
{

}