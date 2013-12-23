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
   printf("Usage: mandelbrot [options] filename");
   printf("\tr:\tReal component to center image\n");
   printf("\ti:\tImaginary component to center image\n");
   printf("\tm:\tMagnification");
   printf("\tw:\tImage width");
   printf("\th:\tImage height");
}

int main(int argc, char **argv) {

   frame_t frame;
   bitmap_t *bitmap;
   int width = DEFAULT_WIDTH, height = DEFAULT_HEIGHT;
   int opt;

   frame.x = 0;
   frame.y = 0;
   frame.magn = 1;

   while ((opt = getopt(argc, argv, "w:h:y:r:i:m:")) != -1) {
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

   bitmap = createBitmap(width, height);
   
   renderFrame(bitmap, &frame);

   saveAsPPM(bitmap, argv[optind]);

   bitmapGC(bitmap);

   return 0;
}