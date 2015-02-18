#pragma once

#ifndef transformer_h
#define transformer_h 1

#include "graphicc.h"
#include <math.h>

#ifdef __cplusplus
extern "C" {
#endif

void trans3d_transform_rows(const size_t nrows, REAL *res, const REAL *inpts, mat4 &tmat);

void trans3d_translate(mat4 &c, const REAL dx, const REAL dy, const REAL dz);

void trans3d_rotatex(mat4 &c, const REAL radians);
void trans3d_rotatey(mat4 &c, const REAL radians);
void trans3d_rotatez(mat4 &c, const REAL radians);
void trans3d_rotate_around_axis(mat4 &c, const real3 n);

void trans3d_scale(mat4 &c, const REAL sx, const REAL sy, const REAL sz);


void trans3d_set_rotation(mat4 &c, const mat3 &rot);

// Some render pipeline matrices
void trans3d_clip_perspective(mat4 &c, const REAL zoomx, const REAL zoomy, const REAL near, const REAL far);
void trans3d_clip_orthographic(mat4 &c, const REAL zoomx, const REAL zoomy, const REAL near, const REAL far);

void trans3d_lookat(mat4 &c, const real3 eye, const real3 lookAt, const real3 up);

void trans3d_map_to_window(REAL &screenx, REAL &screeny,
	const REAL clipx, const REAL clipy, const REAL clipw,
	const REAL winResx, const REAL winResy,
	const REAL winCenterx, const REAL winCentery);


#ifdef __cplusplus
}
#endif

#define zoom_persp(fovrad) (1.0f / (tan(fovrad / 2.0f)))
#define fov_persp(zoom) (2.0 * atan(1.0f / zoom))

#define zoom_ortho(size) (2.0f/size)
#define size_ortho(zoom) (2.0f/zoom)
#endif
