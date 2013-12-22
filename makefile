CC=gcc
FLAGS=-O3 -Wall -pedantic
OBJS=mandelbrot

all: $(OBJS)

mandelbrot: mandelbrot.c
	$(CC) $(FLAGS) mandelbrot.c -lm -o mandelbrot

clean:
	rm $(OBJS)

again: clean all
