#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <pthread.h>

#include "mandelbrot.h"
#include "palette.h"

static bool _alloc_vec_data(struct Vector **vec, u32 data_size) {
    u32 const alloc_size = data_size * sizeof(data_size);
    (*vec)->data = malloc(alloc_size);

    if ((*vec)->data == NULL) {
        fprintf(
            stderr,
            "Error: alloc memory to data section of a `Vector` struct failed\n"
        );
        return false;
    }

    (*vec)->size = data_size;
    memset((*vec)->data, 0, alloc_size);

    return true;
}


static u32 _compute_escape_time(CPoint *c, CPoint *z, fractal_config *cfg) {
    f64 const x_off = c->x - 0.25;
    f64 const y_squared = c->y * c->y;
    f64 const q = x_off * x_off + y_squared;

    if (q * (q + x_off) <= 0.25 * y_squared) {
        // x+iy in period-2 bulb
        return cfg->max_iters;
    }

    u32 esc_bound_squared = cfg->escape_bound * cfg->escape_bound;
    f64 x = 0, y = 0;
    u32 iter_count = 0;

    for (; iter_count<cfg->max_iters; ++iter_count) {
        y = 2 * x * y + c->y;
        x = z->x - z->y + c->x;
        z->x = x * x;
        z->y = y * y;

        if (z->x + z->y > esc_bound_squared) break;
    }

    if (cfg->color_algorithm == 0) {
        // Continuous coloring: make few further iterations
        for (u32 j=0; j<4; ++j) {
            y = 2 * x * y + c->y;
            x = z->x - z->y + c->x;
            z->x = x * x;
            z->y = y * y;
        }
    }

    return iter_count;
}


struct ThreadDataCont {
    fractal_config *cfg;
    struct BitmapData *bitmap;
    i32 start_y;
};

static void* _cont_color_worker(void *thread_args) {
    struct ThreadDataCont *args = thread_args;

    f64 const reciprocal_of_ln2 = 1.0 / log(2.0);
    f64 const multip = log(0.5) * reciprocal_of_ln2;

    palette_t color_palette = get_palette(args->cfg->color_palette);

    i32 end_y = args->cfg->height;
    if (args->start_y == 0) {
        // Thread 1: fix end boundary
        end_y /= 2;
    }

    for (i32 y=args->start_y; y<end_y; ++y) {
        for (i32 x=0; x<args->cfg->width; ++x) {
            // Scale x to [-2, 0.5], y to [-1, 1]
            f64 x_scaled = (x - (4 * args->cfg->width / 5)) * 5.0 / (2.0 * args->cfg->width);
            f64 y_scaled = (y - args->cfg->height / 2) * 2.0 / args->cfg->height;

            CPoint c = {.x = x_scaled, .y = y_scaled};
            CPoint z = {0};

            u32 const iter_count = _compute_escape_time(&c, &z, args->cfg);

            RGBbits color_bits = {0};

            if (iter_count < args->cfg->max_iters) {
                f64 const iter_f = 5 + iter_count - log(log(z.x + z.y)) * reciprocal_of_ln2 - multip;

                u32 const iter_u = floor(iter_f);
                f64 const iter_p = iter_f - (u64)iter_f;

                RGB start_color = color_palette.palette[iter_u % color_palette.colors];
                RGB end_color = color_palette.palette[(iter_u + 1) % color_palette.colors];

                color_bits.red = (end_color.red - start_color.red) * iter_p + start_color.red;
                color_bits.green = (end_color.green - start_color.green) * iter_p + start_color.green;
                color_bits.blue = (end_color.blue - start_color.blue) * iter_p + start_color.blue;
            }

            // Threads never write to the same memory
            bitmap_set_pixel(args->bitmap, &color_bits, x, y);
        }
    }

    return NULL;
}

static bool _compute_with_continuous_coloring(fractal_config *cfg, struct BitmapData *bitmap) {
    pthread_t thread_1, thread_2;

    pthread_create(&thread_1, NULL, _cont_color_worker,
        &(struct ThreadDataCont){.cfg=cfg, .bitmap=bitmap, .start_y=0});

    pthread_create(&thread_2, NULL, _cont_color_worker,
        &(struct ThreadDataCont){.cfg=cfg, .bitmap=bitmap, .start_y=cfg->height / 2});

    pthread_join(thread_1, NULL);
    pthread_join(thread_2, NULL);

    return true;
}


struct ThreadDataHist {
    fractal_config *cfg;
    struct Vector *iters_per_pixel;
    struct Vector *frequency;
    i32 start_y;
};

pthread_mutex_t mutex_hist = PTHREAD_MUTEX_INITIALIZER;

static void* _hist_worker(void *thread_args) {
    struct ThreadDataHist *args = thread_args;

    i32 end_y = args->cfg->height;
    if (args->start_y == 0) {
        // Thread 1: fix end boundary
        end_y /= 2;
    }

    for (i32 y=args->start_y; y<end_y; ++y) {
        for (i32 x=0; x<args->cfg->width; ++x) {
            // Scale x to [-2, 0.5], y to [-1,1]
            f64 x_scaled = (x - (4 * args->cfg->width / 5)) * 5.0 / (2.0 * args->cfg->width);
            f64 y_scaled = (y - args->cfg->height / 2) * 2.0 / args->cfg->height;

            CPoint c = {.x = x_scaled, .y = y_scaled};
            CPoint z = {0};

            u32 const iter_count = _compute_escape_time(&c, &z, args->cfg);

            args->iters_per_pixel->data[x + y * args->cfg->width] = iter_count;

            if (iter_count < args->cfg->max_iters) {
                pthread_mutex_lock(&mutex_hist);

                args->frequency->data[iter_count] += 1;
                pthread_mutex_unlock(&mutex_hist);
            }
        }
    }

    return NULL;
}

static void _compute_mandelbrot_set_hist(
    fractal_config *cfg,
    struct Vector *iters_per_pixel,
    struct Vector *frequency)
{
    pthread_t thread_1, thread_2;

    pthread_create(&thread_1, NULL, _hist_worker,
        &(struct ThreadDataHist)
        {.cfg=cfg, .iters_per_pixel=iters_per_pixel, .frequency=frequency, .start_y=0});

    pthread_create(&thread_2, NULL, _hist_worker,
        &(struct ThreadDataHist)
        {.cfg=cfg, .iters_per_pixel=iters_per_pixel, .frequency=frequency, .start_y=cfg->height / 2});

    pthread_join(thread_1, NULL);
    pthread_join(thread_2, NULL);
}

static void _render_coloring_hist(
    fractal_config *cfg,
    struct Vector *iters_per_pixel,
    struct Vector *frequency,
    struct BitmapData *bitmap)
{
    u32 total_escaped_pixels = 0;

    for (u32 i=0; i<frequency->size; ++i) {
        total_escaped_pixels += frequency->data[i];
    }

    for (i32 y=0; y<cfg->height; ++y) {
        for (i32 x=0; x<cfg->width; ++x) {

            u32 const iter_count = iters_per_pixel->data[x + y * cfg->width];
        
            RGBbits color_bits = {0};

            if (iter_count < cfg->max_iters) {
                u32 pixel_count = 0;

                for (u32 i=0; i<=iter_count; ++i) {
                    pixel_count += frequency->data[i];
                }

                f64 const hue = (f64) pixel_count / total_escaped_pixels;

                if (strncmp(cfg->color_palette, "lb", 2) == 0) {
                    color_bits.blue = pow(255, hue);
                } else if (strncmp(cfg->color_palette, "re", 2) == 0) {
                    color_bits.red = pow(255, hue);
                } else {
                    color_bits.green = pow(255, hue);
                }
            }

            bitmap_set_pixel(bitmap, &color_bits, x, y);
        }
    }

}

static bool _compute_with_histogram_coloring(fractal_config *cfg, struct BitmapData *bitmap) {
    u32 const elems_iters = cfg->height * cfg->width;
    struct Vector *iters_per_pixel = malloc(sizeof *iters_per_pixel);

    if (iters_per_pixel == NULL || !_alloc_vec_data(&iters_per_pixel, elems_iters)) {
        fprintf(stderr, "Error: alloc memory for struct `iters_per_pixel` failed\n");
        free(iters_per_pixel);
        return false;
    }

    u32 const elems_freq = cfg->max_iters;
    struct Vector *frequency = malloc(sizeof *frequency);

    if (frequency == NULL || !_alloc_vec_data(&frequency, elems_freq)) {
        fprintf(stderr, "Error: alloc memory for struct `frequency` failed\n");
        free(frequency);
        free(iters_per_pixel->data);
        free(iters_per_pixel);
        return false;
    }
    
    _compute_mandelbrot_set_hist(cfg, iters_per_pixel, frequency);
    _render_coloring_hist(cfg, iters_per_pixel, frequency, bitmap);

    free(iters_per_pixel->data);
    free(iters_per_pixel);
    free(frequency->data);
    free(frequency);

    return true;
}


static void* _simple_worker(void *thread_args) {
    struct ThreadDataCont *args = thread_args;

    i32 end_y = args->cfg->height;
    if (args->start_y == 0) {
        // Thread 1: fix end boundary
        end_y /= 2;
    }

    for (i32 y=args->start_y; y<end_y; ++y) {
        for (i32 x=0; x<args->cfg->width; ++x) {
            // Scale x to [-2, 0.5], y to [-1,1]
            f64 x_scaled = (x - (4 * args->cfg->width / 5)) * 5.0 / (2.0 * args->cfg->width);
            f64 y_scaled = (y - args->cfg->height / 2) * 2.0 / args->cfg->height;

            CPoint c = {.x = x_scaled, .y = y_scaled};
            CPoint z = {0};

            u32 const iter_count = _compute_escape_time(&c, &z, args->cfg);

            RGBbits color_bits = {0};

            if (iter_count < args->cfg->max_iters) {
                color_bits.red = 255;
                color_bits.green = 255;
                color_bits.blue = 255;
            } else {
                color_bits.red = 96;
                color_bits.green = 96;
                color_bits.blue = 96;
            }

            // Threads never write to the same memory
            bitmap_set_pixel(args->bitmap, &color_bits, x, y);
        }
    }

    return NULL;
}

static bool _compute_with_simple_coloring(fractal_config *cfg, struct BitmapData *bitmap) {
    pthread_t thread_1, thread_2;

    pthread_create(&thread_1, NULL, _simple_worker,
        &(struct ThreadDataCont){.cfg=cfg, .bitmap=bitmap, .start_y=0});

    pthread_create(&thread_2, NULL, _simple_worker,
        &(struct ThreadDataCont){.cfg=cfg, .bitmap=bitmap, .start_y=cfg->height / 2});

    pthread_join(thread_1, NULL);
    pthread_join(thread_2, NULL);

    return true;
}


bool draw_mandelbrot_fractal(fractal_config *cfg) {
    u32 const b_pixels = cfg->width * cfg->height * 3; // 3 bytes for each pixel
    struct BitmapData *bitmap = malloc(sizeof *bitmap + b_pixels * sizeof(u8));

    if (bitmap == NULL) {
        fprintf(stderr, "Error: alloc memory for struct `BitmapData` failed\n");
        exit(EXIT_FAILURE);
    }

    bitmap->height = cfg->height;
    bitmap->width = cfg->width;
    memset(bitmap->pixels, 0, b_pixels * sizeof(u8));

    bool compute_success = false;

    if (cfg->color_algorithm == 0) {
        fprintf(stdout, "drawing fractal with continuous coloring\n");
        compute_success = _compute_with_continuous_coloring(cfg, bitmap);

    } else if (cfg->color_algorithm == 1) {
        fprintf(stdout, "drawing fractal with histogram coloring\n");
        compute_success = _compute_with_histogram_coloring(cfg, bitmap);

    } else {
        fprintf(stdout, "drawing the Mandelbrot set with simple coloring\n");
        compute_success = _compute_with_simple_coloring(cfg, bitmap);
    }

    if (compute_success && bitmap_write(bitmap, "fractal.bmp")) {
        fprintf(stdout, "fractal drawn in file `fractal.bmp`\n");
    } else {
        fprintf(stderr, "Error: fractal computation or drawning failed\n");
        compute_success = false;
    }

    free(bitmap);

    return compute_success;
}
