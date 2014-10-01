#pragma once

#ifndef PPM_H
#define PPM_H

#include "framebuffer.h"

int write_PPM(const char *filename, fb_rgba *fb);

#endif
