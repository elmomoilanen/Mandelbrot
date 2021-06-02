#ifndef __MANDELBROT__
#define __MANDELBROT__

#include "common.h"
#include "bitmap.h"


typedef struct {
    i32 width;
    i32 height;
    u32 max_iters;
    u32 escape_bound;
    u32 color_algorithm;
    char color_palette[4];
} fractal_config;

struct Vector {
    u32 *data;
    u32 size;
};

typedef struct {
    f64 x;
    f64 y;
} CPoint;


void draw_mandelbrot_fractal(fractal_config *cfg);

#endif // __MANDELBROT__
