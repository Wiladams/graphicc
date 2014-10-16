#include "linearalgebra.h"

#include "test_common.h"

void test_pixelvalues()
{
	int value = RGBA(230, 63, 127, 255);

	printf("RED: %d (230)\tGREEN: %d (63)\tBLUE: %d (127)\tALPHA: %d (255)\n",
		GET_R(value), GET_G(value), GET_B(value), GET_A(value));
}



int main(int argc, char **argv)
{
	test_pixelvalues();

	return 0;
}