#ifndef RENDER_H
#define RENDER_H

#include "bitmap.h"

typedef struct {
   long double x;
   long double y;
   long double rotation;
   float magn;
} frame_t;

void renderFrame(bitmap_t *bitmap, frame_t *frame);

#endif