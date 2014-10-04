#pragma once

#ifndef PPM_H
#define PPM_H

#include "pixelbuffer.h"

int write_PPM(const char *filename, pb_rgba *fb);

#endif
