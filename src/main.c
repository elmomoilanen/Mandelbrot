#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "common.h"
#include "argparser.h"
#include "mandelbrot.h"


int main(int argc, char **argv)
{
    fractal_config cfg_from_args = parse_cmdline_args(argc, argv);

    // define default settings that might be overriden
    fractal_config cfg = {
        .width = 800,
        .height = 600,
        .max_iters = 100000,
        .escape_bound = 256,
        .color_algorithm = 0,
        .color_palette = {"uf"},
    };

    if (cfg_from_args.width > 0) cfg.width = cfg_from_args.width;
    if (cfg_from_args.height > 0) cfg.height = cfg_from_args.height;

    if (cfg_from_args.max_iters > 0) cfg.max_iters = cfg_from_args.max_iters;
    if (cfg_from_args.escape_bound > 0) cfg.escape_bound = cfg_from_args.escape_bound;
    
    if (cfg_from_args.color_algorithm > 0) cfg.color_algorithm = cfg_from_args.color_algorithm;

    if (strncmp(cfg_from_args.color_palette, cfg.color_palette, 2) != 0) {
        // color palette differs from default "uf"
        strcpy(cfg.color_palette, cfg_from_args.color_palette);
    }

    fprintf(stdout, "Plotting Mandelbrot set with following parameters:\n");
    fprintf(stdout, "width: %i\n", cfg.width);
    fprintf(stdout, "heigth: %i\n", cfg.height);
    fprintf(stdout, "max iterations: %u\n", cfg.max_iters);
    fprintf(stdout, "escape bound: %u\n", cfg.escape_bound);

    draw_mandelbrot_fractal(&cfg);
}
