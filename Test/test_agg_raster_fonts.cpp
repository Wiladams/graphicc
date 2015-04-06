#include "test_common.h"
#include "agg_embedded_raster_fonts.h"

//
// agg_glyph_raster_bin
//

typedef unsigned char cover_type;    //----cover_type
enum cover_scale_e
{
	cover_shift = 8,                 //----cover_shift
	cover_size = 1 << cover_shift,  //----cover_size 
	cover_mask = cover_size - 1,    //----cover_mask 
	cover_none = 0,                 //----cover_none 
	cover_full = cover_mask         //----cover_full 
};

bool isBigEndian() {
	int t = 1;
	return (*(char*)&t == 0);
}

/*
font[0]		height
font[1]		baseline
font[2]		start_char
font[3]		num_chars
*/

typedef struct _font {
	uint8_t	height;
	uint8_t baseline;
	uint8_t start_char;
	size_t num_chars;

	bool bigendian;
	uint8_t *charbits;

} font_t;

struct glyph_rect
{
	int x1, y1, x2, y2;
	double dx, dy;
}; 

typedef struct _glyph {
	size_t width;
	size_t byte_width;

	uint8_t *data;

	struct glyph_rect rect;
} glyph_t;




void font_t_init(font_t *f, const uint8_t *data)
{
	f->bigendian = isBigEndian();
	f->height = data[0];
	f->baseline = data[1];
	f->start_char = data[2];
	f->num_chars = data[3];
	f->charbits = (uint8_t *)(data + 4);
}

// Create a 16-bit value, taking into account
// the endianness of the host.
// The data is in little endian format
uint16_t font_t_value(const font_t *f, const uint8_t* p)
{
	uint16_t v;
	if (!f->bigendian)
	{
		v = p[0] + (p[1] << 8);
		//*(uint8_t*)&v = p[0];
		//*((uint8_t*)&v + 1) = p[1];
	}
	else
	{
		v = p[1] + (p[0] << 8);
		//*(uint8_t*)&v = p[1];
		//*((uint8_t*)&v + 1) = p[0];
	}

	return v;
}

uint8_t * font_t_glyph_pointer(const font_t *f, const unsigned int glyph)
{
	return (f)->charbits + (f)->num_chars * 2 + font_t_value(f, (f)->charbits + (glyph - (f)->start_char) * 2);
}

// Fill in the meta information for the specified glyph
void glyph_t_init(const font_t *f, glyph_t *ginfo, const unsigned int glyph)
{
	uint8_t * glyphptr = font_t_glyph_pointer(f, glyph);

	ginfo->width = glyphptr[0];
	ginfo->data = glyphptr + 1;
	ginfo->byte_width = (ginfo->width + 7) >> 3;
}

size_t font_t_glyph_width(const font_t *f, const unsigned int glyph)
{
	glyph_t ginfo;
	glyph_t_init(f, &ginfo, glyph);

	return ginfo.width;
}

// Prepare a glyph to be written to a specific
// position
void glyph_t_prepare(const font_t *font, const glyph_t *ginfo, struct glyph_rect* r, double x, double y, bool flip)
{
	r->x1 = (int)x;
	r->x2 = r->x1 + ginfo->width - 1;
	if (flip)
	{
		r->y1 = int(y) - font->height + font->baseline;
		r->y2 = r->y1 + font->height - 1;
	}
	else
	{
		r->y1 = int(y) - font->baseline + 1;
		r->y2 = r->y1 + font->height - 1;
	}
	r->dx = ginfo->width;
	r->dy = 0;
}



// Create a single scanline of the glyph
cover_type* glyph_t_span(const font_t *f, glyph_t *g, unsigned i, cover_type *m_span)
{
	//i = m_font[0] - i - 1;
	i = f->height - i - 1;

	//const uint8_t * bits = m_bits + i * g->byte_width;
	const uint8_t *bits = g->data + i * g->byte_width;
	unsigned int j;
	unsigned int val = *bits;
	unsigned int nb = 0;

	for (j = 0; j < g->width; ++j)
	{
		m_span[j] = (cover_type)((val & 0x80) ? cover_full : cover_none);
		val <<= 1;
		if (++nb >= 8)
		{
			val = *++bits;
			nb = 0;
		}
	}
	
	return m_span;
}


// Figure out the width of a string in a given font
size_t font_t_str_width(const font_t *f, const char * str)
{
	size_t w = 0;

	while (*str)
	{
		w += font_t_glyph_width(f, *str);
		++str;
	}

	return w;
}




void test_font_info()
{
	font_t vd12;

	// Read a font and print out info
	font_t_init(&vd12, verdana12);


	printf("FONT (Verdana 12) Height: %d\tbaseline: %d\tstart_char: %d\tnum_chars: %d\n", 
		vd12.height, vd12.baseline, 
		vd12.start_char,
		vd12.num_chars);

	size_t strwidth = font_t_str_width(&vd12, "Hello, World!");
	printf("string width ==> 'Hello, World!' : %d\n", strwidth);

	strwidth = font_t_str_width(&vd12, ".");
	printf("string width ==> '.': %d\n", strwidth);

	strwidth = font_t_str_width(&vd12, "+");
	printf("string width ==> '+': %d\n", strwidth);
}

void test_prepare_glyph()
{
	font_t vd12;
	glyph_t ginfo;
	struct glyph_rect r;
	double x = 0;
	double y = 12;
	bool flip = true;

	font_t_init(&vd12, verdana12);
	glyph_t_init(&vd12, &ginfo, 'H');
	glyph_t_prepare(&vd12, &ginfo, &r, x, y, flip);

	printf("PREPARED ('H'): %d, %d  %d, %d\n", r.x1, r.y1, r.x2, r.y2);
}



int main(int argc, char **argv)
{
	size_t pbwidth = 640;
	size_t pbheight = 480;

	pb_rgba pb;
	pb_rgba_init(&pb, pbwidth, pbheight);

	raster_rgba_rect_fill(&pb, 0, 0, pbwidth-1, pbheight-1, pBlue);

	//test_font_info();
	test_prepare_glyph();

	int err = write_PPM("test_agg_raster_fonts.ppm", &pb);

	return 0;
}
