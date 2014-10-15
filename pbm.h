#pragma once

#ifndef PBM_H
#define PBM_H

#include "pixelbuffer.h"

#ifdef __cplusplus
extern "C" {
#endif

int write_PPM(const char *filename, pb_rgba *fb);

#ifdef __cplusplus
}
#endif

#endif
