/*****************************/
/*  Mandelbrot Set Renderer  */
/*   Patrick Herrmann 2010   */
/*****************************/

#include <stdio.h>
#include <math.h>

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

void renderFrame(float magn, int xres, int yres,
 long double target_x, long double target_y);
int isInMainBulbs(long double a, long double b2);
unsigned char averageValue(unsigned char a,
 unsigned char b, unsigned char c, unsigned char d);
void HSVtoRGB( float *r, float *g, float *b, float h, float s, float v );

int main() {
   int xres, yres;
   float magn;
   long double target_x, target_y;
   scanf("%f%d%d%Lf%Lf", &magn, &xres, &yres, &target_x, &target_y);
   renderFrame(magn, xres, yres, target_x, target_y);
   return 0;
}

void renderFrame(float magn, int xres, int yres,
 long double target_x, long double target_y) {
   int x, y, i, iters, lastPixelBlack, period;
   float degree, clrR, clrG, clrB, hue, brightness, logBailout2, checkX, checkY;
   long double cx, cy, zx, zy, xstep, ystep, 
    zx2, zy2, width, height;
   unsigned char image[xres * 2][yres * 2][3];

   printf("P6\n");
   printf("%d %d\n", xres, yres);
   printf("%d\n", COLORS);
   
   xres += xres;
   yres += yres;
   
   iters = 1000 + 50 * sqrt(magn);
   
   width = DEFAULT_WIDTH * 1.0 / magn;
   height = DEFAULT_WIDTH * (1.0 * yres / xres) / magn;

   xstep = width / xres;
   ystep = height / yres;
   
   logBailout2 = log(BAILOUT) * 2;

   for (y = 0; y < yres; y++) {
      for (x = 0; x < xres; x++) {
         cx = target_x - width / 2 + x * xstep;
         cy = target_y - height / 2 + (yres - y) * ystep;
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
                  if (checkX == zx && checkY == zy) break;
                  if (i == period) {
                     checkX = zx;
                     checkY = zy;
                     period += period * 2;
                  }
               }
            }
         }
         lastPixelBlack = clrR + clrG + clrB == 0;
         image[x][y][0] = clrR * COLORS;
         image[x][y][1] = clrG * COLORS;
         image[x][y][2] = clrB * COLORS;
      }
   }
   
   /* Resample and output */
   for (y = 0; y < yres; y += 2) {
      for (x = 0; x < xres; x += 2) {
         for (i = 0; i <= 2; i++) {
            printf("%c", averageValue(image[x][y][i], image[x + 1][y][i],
             image[x][y + 1][i], image[x + 1][y + 1][i]));
         }
      }
   }
   
   return;
}

/* Checks if point is in main cardioid or period 2 bulb */
int isInMainBulbs(long double a, long double b2) {
   double q = (a - 0.25) * (a - 0.25) + b2;
   if ((4 * q * (q + (a - 0.25)) < b2) 
    || (16 * ((a + 1) * (a + 1) + b2) < 1)) {
      return 1;
   }
   return 0;
}

/* Returns average of four floats for resampling */
unsigned char averageValue(unsigned char a,
 unsigned char b, unsigned char c, unsigned char d) {
   return (a + b + c + d) / 4;
}

/* HSV to RGB from internet */
void HSVtoRGB( float *r, float *g, float *b, float h, float s, float v ) {
	int i;
	float f, p, q, t;
	
	if(s == 0) {
		*r = *g = *b = v;
		return;
	}

	h /= 60;
	i = floor(h);
	f = h - i;
	p = v * (1 - s);
	q = v * (1 - s * f);
	t = v * (1 - s * (1 - f));

	switch(i) {
		case 0:
			*r = v;
			*g = t;
			*b = p;
			break;
		case 1:
			*r = q;
			*g = v;
			*b = p;
			break;
		case 2:
			*r = p;
			*g = v;
			*b = t;
			break;
		case 3:
			*r = p;
			*g = q;
			*b = v;
			break;
		case 4:
			*r = t;
			*g = p;
			*b = v;
			break;
		default:
			*r = v;
			*g = p;
			*b = q;
			break;
	}
}
