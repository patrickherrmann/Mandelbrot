#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "render.h"
#include "hsv2rgb.h"

/* Fractal parameters */
#define BAILOUT 9.1
#define SEEDX 0
#define SEEDY 0
#define DEFAULT_WIDTH 4
#define INIT_PERIOD 8

/* Coloring parameters */
#define COLORS 255
#define MAX_HUE 360
#define CYCLE 75
#define HUE_SHIFT 0.5
#define BRIGHTNESS_RANGE 0.7
#define BRIGHTNESS_MIN 0.1

/* Constants */
#define LN_2 0.693147181

/* Checks if point is in main cardioid or period 2 bulb */
int isInMainBulbs(long double a, long double b2) {
   double q = (a - 0.25) * (a - 0.25) + b2;
   if ((4 * q * (q + (a - 0.25)) < b2) 
    || (16 * ((a + 1) * (a + 1) + b2) < 1)) {
      return 1;
   }
   return 0;
}

/* The algorithm */
void renderFrame(bitmap_t *bitmap, frame_t *frame) {

   int x, y, i, iters, lastPixelBlack = 0, period;
   float degree, clrR, clrG, clrB, hue, brightness, logBailout2, checkX, checkY;
   long double cx, cy, zx, zy, xstep, ystep, zx2, zy2, width, height;
   pixel_t *pixel;
   
   iters = 1000 + 50 * sqrt(frame->magn);
   
   width = DEFAULT_WIDTH / frame->magn;
   height = (DEFAULT_WIDTH / frame->magn  * bitmap->height) / bitmap->width;

   xstep = width / bitmap->width;
   ystep = height / bitmap->height;
   
   logBailout2 = log(BAILOUT) * 2;
   
   printf("Rendering...\n");

   for (y = 0; y < bitmap->height; y++) {
      for (x = 0; x < bitmap->width; x++) {

         cx = frame->x - width / 2 + x * xstep;
         cy = frame->y - height / 2 + (bitmap->height - y) * ystep;
         zx = checkX = SEEDX;
         zy = checkY = SEEDY;
         clrR = clrG = clrB = 0;
         period = 8;

         if (!isInMainBulbs(cx, cy * cy)) {
            for (i = 1; i <= iters; i++) {

               zx2 = zx * zx;
               zy2 = zy * zy;

               if (zx2 + zy2 > BAILOUT) {

                  degree = i - log(log(zx2 + zy2) / logBailout2) / LN_2;
                  degree = (sin(degree / CYCLE) + 1) / 2;
                  hue = degree + HUE_SHIFT;
                  hue = fmod(hue, 1.0);
                  brightness = degree * BRIGHTNESS_RANGE + BRIGHTNESS_MIN;
                  HSVtoRGB(&clrR, &clrG, &clrB, hue * MAX_HUE, 1, brightness);
                  break;
               }

               zy = 2 * zx * zy + cy;
               zx = zx2 - zy2 + cx;

               if (lastPixelBlack) {

                  if (checkX == zx && checkY == zy)
                     break;

                  if (i == period) {
                     checkX = zx;
                     checkY = zy;
                     period += period * 2;
                  }
               }
            }
         }
         
         lastPixelBlack = clrR + clrG + clrB == 0;

         pixel = getPixel(bitmap, x, y);
         pixel->r = clrR * COLORS;
         pixel->g = clrG * COLORS;
         pixel->b = clrB * COLORS;
      }

      printf("%d%%\r", (int)(100.0 * y / bitmap->height));
      fflush(stdout);
   }

   printf("Done\n");
   
   return;
}