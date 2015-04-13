/*
Copyright 2015 William A Adams

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#include "graphicc.h"

#include <stdio.h>

#pragma warning(push)
#pragma warning(disable: 4996)	// _CRT_SECURE_NO_WARNINGS (fopen) 

int read_PPM(const char *filename, pb_rgba *fb)
{
	return -1;
}

int write_PPM_ascii(const char *filename, pb_rgba *fb)
{
	FILE * fp = fopen(filename, "wb");

	if (!fp) return -1;

	// write out the image header
	fprintf(fp, "P3\n%d %d\n255\n", fb->frame.width, fb->frame.height);

	// write the individual pixel values in binary form
	uint32_t * pixelPtr = (uint32_t *)fb->data;
	uint8_t rgb[3];

	for (int row = 0; row < fb->frame.height; row++) {
		for (int col = 0; col < fb->frame.width; col++){
			rgb[0] = GET_R(*pixelPtr);
			rgb[1] = GET_G(*pixelPtr);
			rgb[2] = GET_B(*pixelPtr);
			fprintf(fp, "%3d %3d %3d  ", rgb[0], rgb[1], rgb[2]);
		}
		pixelPtr += fb->pixelpitch;
		fprintf(fp,"\n");
	}


	fclose(fp);

	return 0;
}


int write_PPM_binary(const char *filename, pb_rgba *fb)
{
	FILE * fp = fopen(filename, "wb");
	
	if (!fp) return -1;

	// write out the image header
	fprintf(fp, "P6\n%d %d\n255\n", fb->frame.width, fb->frame.height);
	
	// write the individual pixel values in binary form
	uint32_t * pixelPtr = (uint32_t *)fb->data;
	uint8_t rgb[3];

	for (int row = 0; row < fb->frame.height; row++) {
		for (int col = 0; col < fb->frame.width; col++){
			rgb[0] = GET_R(pixelPtr[col]);
			rgb[1] = GET_G(pixelPtr[col]);
			rgb[2] = GET_B(pixelPtr[col]);
			fwrite((void *)&rgb[0], 3, 1, fp);
			//fwrite(&pixelPtr[col], 3, 1, fp);
		}
		pixelPtr += fb->pixelpitch;
	}


	fclose(fp);

	return 0;
}

#pragma warning(pop)