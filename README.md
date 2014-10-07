graphicc
========

graphics library in C

This library is an experiment in compactful usefulness.  I want
to create a library that satisfies a few criteria.

1. Works well enough for embedded devices
2. Can interact with higher level acceleration
3. Can be used for basic 2D drawing
4. Can be used for basic 3D rendering
5. Can work well with scripting languages

The library sticks with the most basic of C interfaces.  There
is no usage of C++ constructs such as classes, templates, overloading
or anything of that sort.

It should probably be "cross platform" where 'platform' os not
limited to desktop operating systems.

These might not be the fastest routines on the planet, but they
should be correct, and fairly compact.

The render_rgba routines are meant to be base drawing routines.  
set_pixel, get_pixel, hline, vline, rect_fill, triangle_fill, ellipse, roundedrect
And that's it.  Things such as polygons are meant to be broken down into these various
primitives.  Multi-line drawing, curves, etc.

In addition, there will be various forms of pixel blit.

These routines will come in two flavors.  The 'solid' version, which are the defaults, 
have an alpha copy routine of SRCOPY, which means there is no alpha blending.  Second
forms will include basic alpha blending.

The 3D rendering routines will make heavy usage of the triangle rendering primitive.

Interesting libraries
=====================
1. **LibGD** - http://libgd.bitbucket.org/
2. **Netpbm** - http://netpbm.sourceforge.net/
3. **AntiGrain** - http://antigrain.com/
4. Paul Heckbert's **Zoom** - http://user.xmission.com/~legalize/zoom.html
