#include "pbm.h"
#include <stdio.h>

#pragma warning(push)
#pragma warning(disable: 4996)	// _CRT_SECURE_NO_WARNINGS (fopen) 


int write_PPM(const char *filename, fb_rgba *fb)
{
	FILE * fp = fopen(filename, "wb");
	
	if (!fp) return -1;

	// write out the image header
	fprintf(fp, "P6\n%d %d\n255\n", fb->width, fb->height);

	// Now write binary for each row
	for (size_t row = 0; row < fb->height; row++) {
		pix_rgba * pixPtr = (pix_rgba *)&fb->data[fb->pitch*row];
		for (size_t col = 0; col < fb->width; col++){
			fwrite(&pixPtr[col], 3, 1, fp);
		}
	}

	fclose(fp);

	return 0;
}

#pragma warning(pop)