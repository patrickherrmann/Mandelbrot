/*****************************/
/*  Mandelbrot Set Renderer  */
/*   Patrick Herrmann 2010   */
/*****************************/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>

#include "render.h"
#include "bitmap.h"

#define DEFAULT_WIDTH 800
#define DEFAULT_HEIGHT 600

void printUsage() {
   printf("Usage: mandelbrot [options] filename\n");
   printf("\tr:\tReal component of center of image\n");
   printf("\ti:\tImaginary component of center of image\n");
   printf("\tm:\tMagnification\n");
   printf("\tw:\tImage width\n");
   printf("\th:\tImage height\n");
   printf("\ts:\tResampling box size\n");
}

int main(int argc, char **argv) {

   frame_t frame;
   bitmap_t *bitmap;
   bitmap_t *resampled;
   int width = DEFAULT_WIDTH, height = DEFAULT_HEIGHT;
   int s = 1;
   int opt;

   frame.x = 0;
   frame.y = 0;
   frame.magn = 1;

   while ((opt = getopt(argc, argv, "w:h:y:r:i:m:s:")) != -1) {
      switch (opt) {
         case 'w':
            sscanf(optarg, "%d", &width);
            break;
         case 'h':
            sscanf(optarg, "%d", &height);
            break;
         case 'r':
            sscanf(optarg, "%Lf", &frame.x);
            break;
         case 'i':
            sscanf(optarg, "%Lf", &frame.y);
            break;
         case 'm':
            sscanf(optarg, "%f", &frame.magn);
            break;
         case 's':
            sscanf(optarg, "%d", &s);
            break;
         case '?':
         default:
            printUsage();
            return 1;
      }
   }

   if (optind != argc - 1) {
      printUsage();
      return 1;
   }

   bitmap = createBitmap(width * s, height * s);
   renderFrame(bitmap, &frame);

   if (s > 1) {
      printf("Resampling...\n");
      resampled = createBitmap(width, height);
      resample(bitmap, resampled, s);
      bitmapGC(bitmap);
      printf("Done\n");
   } else {
      resampled = bitmap;
   }

   saveAsPPM(resampled, argv[optind]);

   bitmapGC(resampled);

   return 0;
}