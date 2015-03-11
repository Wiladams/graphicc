This test directory contains a few unit tests, as well as larger
API experiments which use the core graphics library for drawing pixels.

*3D Modeling*
** zoom	- camera's zoom values
** winPhys (x,y)	- physical window size
** winRes (x,y)		- resolution of window in pixels
** pixPhys (x,y)	- physical dimensions of a pixel
** devPhys (x,y)	- physical dimensions of the output device. (ratios)
** devRes (x,y)		- resolution of the output device

Model Space
===========

World Space
============

Camera Space
============

// Clip Matrix
// Prepare for projection
// Apply zoom and prepare for clipping

// View frustum in clip space
// Bottom	y < -w
// Top		y > w
// Left		x < -w
// Right	x > w
// Near		z < -w
// Far		z > w

