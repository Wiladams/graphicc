#include "pixelbuffer.h"

#include <stdlib.h>
#include <string.h>

int pb_rgba_init(pb_rgba *fb, const unsigned int width, const unsigned int height)
{
	if (!fb) return -1;

	size_t pitch = width * 4;
	size_t datasize = height * pitch;

	fb->data = (unsigned char *)calloc(datasize, 1);
	fb->owndata = 1;

	fb->frame.x = 0;
	fb->frame.y = 0;
	fb->frame.width = width;
	fb->frame.height = height;

	fb->pixelpitch = width;

	return 0;
}

int pb_rgba_free(pb_rgba *fb)
{
	if (!fb) return -1;
	if (!fb->data) return -1;

	if (fb->owndata) {
		free(fb->data);
	}

	fb->data = (unsigned char *)0;
	pb_rect_clear(fb->frame);

	return 0;
}

int pb_rgba_get_frame(pb_rgba *pb, const unsigned int x, const unsigned int y, const unsigned int width, const unsigned int height, pb_rgba *pf)
{
	// assume what's being asked for is already constrained
	pf->data = pb->data+((y*pb->pixelpitch+x)*sizeof(int));
	pf->owndata = 0;

	pf->frame.x = x;
	pf->frame.y = y;
	pf->frame.width = width;
	pf->frame.height = height;
	pf->pixelpitch = pb->pixelpitch;

	return 0;
}

void pb_rgba_cover_pixel(pb_rgba *pb, const unsigned int x, const unsigned int y, const uint32_t value)
{

	// Quick reject if the foreground pixel has both 0 opacity
	// and 0 for color component values
	if (0 == value) {
		return;
	}

	if (0 == GET_A(value)) {
		// Combine the colors, but don't
		// change the alpha of the background
	} else if (255 == GET_A(value)) {
		// The foreground opacity is full, so set
		// the color
		// and set the background alpha to full as well
		pb_rgba_set_pixel(pb, x, y, value);
	} else {
		// All other cases where doing a cover of something
		// other than full opacity
		uint8_t alpha = GET_A(value);
		pix_rgba * B = (pix_rgba *)&pb->data[(y*(pb)->pixelpitch) + x];
		B->r = lerp255(B->r, GET_R(value), alpha);
		B->g = lerp255(B->g, GET_R(value), alpha);
		B->b = lerp255(B->b, GET_R(value), alpha);
		B->a = lerp255(B->a, GET_R(value), alpha);
	}
}



