CC=gcc
FLAGS=-O3 -Wall -pedantic
OBJS=render.o hsv2rgb.o bitmap.o
EXES=mandelbrot

all: $(EXES)

%.o: %.c
	$(CC) $(FLAGS) -c $^ -lm

mandelbrot: mandelbrot.c $(OBJS)
	$(CC) $(FLAGS) mandelbrot.c $(OBJS) -lm -o mandelbrot

clean:
	rm $(OBJS) $(EXES)

again: clean all
