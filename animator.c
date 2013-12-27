/*****************************/
/*  Mandelbrot Set Animator  */
/*   Patrick Herrmann 2013   */
/*****************************/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <string.h>

#include "render.h"
#include "bitmap.h"

#define DEFAULT_WIDTH 800
#define DEFAULT_HEIGHT 600
#define DEFAULT_FRAMES 90
#define DEFAULT_ZOOM 1.01
#define DEFAULT_ROTATION 0

#define MAX_FILENAME_LEN 64

void printUsage() {
   printf("Usage: animator [options] directory\n");
   printf("\tr:\tReal component of center of image\n");
   printf("\ti:\tImaginary component of center of image\n");
   printf("\tm:\tMagnification per frame as ratio\n");
   printf("\tt:\tRotation per frame in radians");
   printf("\tw:\tImage width\n");
   printf("\th:\tImage height\n");
   printf("\ts:\tResampling box size\n");
   printf("\tf:\tThe number of frames to render\n");
}

int main(int argc, char **argv) {

   frame_t frame;
   bitmap_t *bitmap;
   bitmap_t *resampled;

   int frames = DEFAULT_FRAMES, f;
   long double zoom = DEFAULT_ZOOM;
   long double rotation = DEFAULT_ROTATION;

   char filename[MAX_FILENAME_LEN];
   int dir_len;
   int width = DEFAULT_WIDTH, height = DEFAULT_HEIGHT;
   int s = 1;
   int opt;

   frame.x = 0;
   frame.y = 0;
   frame.magn = 1;
   frame.rotation = 0;

   while ((opt = getopt(argc, argv, "w:h:y:r:i:m:t:s:f:")) != -1) {
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
            sscanf(optarg, "%Lf", &zoom);
            break;
         case 't':
            sscanf(optarg, "%Lf", &rotation);
            break;
         case 's':
            sscanf(optarg, "%d", &s);
            break;
         case 'f':
            sscanf(optarg, "%d", &frames);
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

   strcpy(filename, argv[optind]);
   dir_len = strlen(filename);

   bitmap = createBitmap(width * s, height * s);
   resampled = createBitmap(width, height);

   for (f = 0; f < frames; f++) {
      sprintf(filename + dir_len, "frame%04d.ppm", f);
      printf("--- Frame %d ---\n", f);
      renderFrame(bitmap, &frame);
      resample(bitmap, resampled, s);
      saveAsPPM(resampled, filename);
      frame.magn *= zoom;
      frame.rotation += rotation;
   }

   bitmapGC(bitmap);
   bitmapGC(resampled);

   return 0;
}