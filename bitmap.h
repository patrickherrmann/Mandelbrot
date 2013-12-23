#ifndef BITMAP_H
#define BITMAP_H

#include <stdint.h>

typedef struct {
   uint8_t r;
   uint8_t g;
   uint8_t b;
} pixel_t;

typedef struct {
	pixel_t *pixels;
	int width;
	int height;
} bitmap_t;

bitmap_t *createBitmap(int width, int height);

pixel_t *getPixel(bitmap_t *bitmap, int x, int y);

void saveAsPPM(bitmap_t *bitmap, const char *filename);

void bitmapGC(bitmap_t *bitmap);

#endif