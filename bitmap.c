#include <stdio.h>
#include <stdlib.h>
#include "bitmap.h"

bitmap_t *createBitmap(int width, int height) {

    bitmap_t *bitmap;

    if ((bitmap = malloc(sizeof(bitmap_t))) == NULL) {
        perror("malloc");
        exit(1);
    }

    bitmap->width = width;
    bitmap->height = height;

    if ((bitmap->pixels = malloc(width * height * sizeof(pixel_t))) == NULL) {
        perror("malloc");
        exit(1);
    }

    return bitmap;
}

pixel_t *getPixel(bitmap_t *bitmap, int x, int y) {
    return bitmap->pixels + y * bitmap->width + x;
}

void resample(bitmap_t *image, bitmap_t *resampled, int boxSize) {
    int r, g, b;
    int x, y, ix, iy, bs2 = boxSize * boxSize;
    pixel_t *p;

    for (y = 0; y < resampled->height; y++) {
        for (x = 0; x < resampled->width; x++) {
            r = g = b = 0;
            for (iy = 0; iy < boxSize; iy++) {
                for (ix = 0; ix < boxSize; ix++) {
                    p = getPixel(image, x * boxSize + ix, y * boxSize + iy);
                    r += p->r;
                    g += p->g;
                    b += p->b;
                }
            }
            p = getPixel(resampled, x, y);
            p->r = r / bs2;
            p->g = g / bs2;
            p->b = b / bs2;
        }
    }
}

void saveAsPPM(bitmap_t *bitmap, const char *filename) {

    FILE *file = fopen(filename, "wb");

    fprintf(file, "P6\n%d %d\n255\n", bitmap->width, bitmap->height);

    fwrite(bitmap->pixels, sizeof(pixel_t), bitmap->width * bitmap->height, file);

    fclose(file);
}

void bitmapGC(bitmap_t *bitmap) {
    free(bitmap->pixels);
    free(bitmap);
}