#pragma once

#ifndef transformer_h
#define transformer_h 1

#include "graphicc.h"

#ifdef __cplusplus
extern "C" {
#endif

void trans3d_rotatex(mat4 &c, const REAL radians);
void trans3d_rotatey(mat4 &c, const REAL radians);
void trans3d_rotatez(mat4 &c, const REAL radians);
void trans3d_rotate_around_axis(mat4 &c, const real3 n);

void trans3d_scale(mat4 &c, const REAL sx, const REAL sy, const REAL sz);


// Some render pipeline matrices
void trans3d_clip_perspective(mat4 &c, const REAL zoomx, const REAL zoomy, const REAL near, const REAL far);
void trans3d_clip_orthographic(mat4 &c, const REAL zoomx, const REAL zoomy, const REAL near, const REAL far);

void trans3d_map_to_window(REAL &screenx, REAL &screeny,
	const REAL clipx, const REAL clipy, const REAL clipw,
	const REAL winResx, const REAL winResy,
	const REAL winCenterx, const REAL winCentery);


#ifdef __cplusplus
}
#endif

#endif
