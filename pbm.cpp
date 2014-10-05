#include "pbm.h"

#include <stdio.h>

#pragma warning(push)
#pragma warning(disable: 4996)	// _CRT_SECURE_NO_WARNINGS (fopen) 


int write_PPM(const char *filename, pb_rgba *fb)
{
	FILE * fp = fopen(filename, "wb");
	
	if (!fp) return -1;

	// write out the image header
	fprintf(fp, "P6\n%d %d\n255\n", fb->frame.width, fb->frame.height);
	
	// write the individual pixel values in binary form
	unsigned int * pixelPtr = (unsigned int *)fb->data;

	for (size_t row = 0; row < fb->frame.height; row++) {
		for (size_t col = 0; col < fb->frame.width; col++){
			fwrite(&pixelPtr[col], 3, 1, fp);
		}
		pixelPtr += fb->pixelpitch;
	}


	fclose(fp);

	return 0;
}

#pragma warning(pop)