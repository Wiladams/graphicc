graphicc
========

graphics library in C

This library is an experiment in compactful usefulness.  I want
to create a library that satisfies a few criteria.

1) Works well enough for embedded devices
2) Can interact with higher level acceleration
3) Can be used for basic 2D drawing
4) Can be used for basic 3D rendering
5) Can work well with scripting languages

The library sticks with the most basic of C interfaces.  There
is no usage of C++ constructs such as classes, templates, overloading
or anything of that sort.

It should probably be "cross platform" where 'platform' os not
limited to desktop operating systems.

These might not be the fastest routines on the planet, but they
should be correct, and fairly compact.