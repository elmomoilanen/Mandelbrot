#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include <unistd.h>
#include <getopt.h>

#include "argparser.h"
#include "palette.h"

#define COLOR_ALGORITHMS 3
#define COLOR_PALETTES 5

palette_map pmap[] = {
    {.name = "or", .long_name = "orange"},
    {.name = "lb", .long_name = "lightblue"},
    {.name = "re", .long_name = "red"},
    {.name = "gr", .long_name = "green"},
    {.name = "uf", .long_name = "ultrafractal"},
};

static u64 _parse_to_numeric(char *arg, char *name) {
    char *extra;
    errno = 0;

    u64 num = strtol(arg, &extra, 10);

    if (errno) {
        fprintf(stderr, "Error when reading command line argument: %s\n", strerror(errno));
        return 0;
    }

    if (strncmp(name, "w", 1) == 0 || strncmp(name, "h", 1) == 0) {
        u32 num_min = 300, num_max = 3000;

        if (num < num_min || num > num_max) {
            fprintf(stderr, "Error when reading argument `%s` ", name);
            fprintf(stderr, "it must be within [%u, %u]\n", num_min, num_max);
            return 0;
        }
    } else if (strncmp(name, "m", 1) == 0) {
        u32 num_min = 100, num_max = 10000000;

        if (num < num_min || num > num_max) {
            fprintf(stderr, "Error when reading argument `%s` ", name);
            fprintf(stderr, "it must be within [%u, %u]\n", num_min, num_max);
            return 0;
        }
    } else if (strncmp(name, "e", 1) == 0) {
        u32 num_min = 2, num_max = U16_MAX - 1;

        if (num < num_min || num > num_max) {
            fprintf(stderr, "Error when reading argument `%s` ", name);
            fprintf(stderr, "it must be within [%u, %u]\n", num_min, num_max);
            return 0;
        }
    } else {
        // should not end up here
        fprintf(stderr, "Unknown option `%s`\n", name);
        return 0;
    }

    return num;
}

static u32 _parse_color_algorithm(char *arg) {
    u32 const options = COLOR_ALGORITHMS;
    const char *opts[] = {"continuous", "histogram", "simple"};

    for (u32 i=0; i<options; ++i) {
        if (strncmp(arg, opts[i], strlen(opts[i])) == 0) {
            return i; 
        }
    }

    fprintf(stderr, "Error when reading coloring algorithm argument, accepted values are: ");

    for (u32 i=0; i<options; ++i) {
        fprintf(stderr, "`%s`", opts[i]);

        if (i < options - 1) fprintf(stderr, ", ");
        else fprintf(stderr, ".\n");
    }

    return COLOR_ALGORITHMS;
}


static char* _parse_color_palette(char *arg) {
    u32 const options = COLOR_PALETTES;

    for (u32 i=0; i<options; ++i) {
        if (strncmp(arg, pmap[i].long_name, strlen(pmap[i].long_name)) == 0) {
            return pmap[i].name;
        }
    }

    fprintf(stderr, "Error when reading color palette argument, accepted values are: ");

    for (u32 i=0; i<options; ++i) {
        fprintf(stderr, "`%s`", pmap[i].long_name);

        if (i < options - 1) fprintf(stderr, ", ");
        else fprintf(stderr, ".\n");
    }

    return NULL;
}

static void _print_args_description() {
    fprintf(stdout, "Allowed command line options and descriptions of their arguments (default value)\n\n");
    fprintf(stdout, "[-w width]: width of the image, in pixels (800)\n");
    fprintf(stdout, "[-h height]: height of the image, in pixels (600)\n");
    fprintf(stdout, "[-m max_iters]: max iterations for escape time algorithm, non-negative integer (100 000)\n");
    fprintf(stdout, "[-e escape_bound]: escape radius for escape time algorithm, non-negative integer (256)\n");
    fprintf(stdout, "[-a coloring_algorithm]: continuous, histogram or simple (continuous)\n");
    fprintf(stdout, "[-p color_palette]: orange, lightblue, red, green or ultrafractal (ultrafractal)\n");
}

fractal_config parse_cmdline_args(int argc, char **argv) {
    i32 width = 0, height = 0;
    u32 max_iters = 0, escape_bound = 0;
    u32 color_algorithm = 0;
    char *palette = NULL;

    i32 c = 0;

    while ((c = getopt(argc, argv, "w:h:m:e:a:p:s")) != -1) {
        switch (c) {
        case 'w':
            width = (i32) _parse_to_numeric(optarg, "w");
            if (width == 0) exit(EXIT_FAILURE);
            break;
        case 'h':
            height = (i32) _parse_to_numeric(optarg, "h");
            if (height == 0) exit(EXIT_FAILURE);
            break;
        case 'm':
            max_iters = (u32) _parse_to_numeric(optarg, "m");
            if (max_iters == 0) exit(EXIT_FAILURE);
            break;
        case 'e':
            escape_bound = (u32) _parse_to_numeric(optarg, "e");
            if (escape_bound == 0) exit(EXIT_FAILURE);
            break;
        case 'a':
            color_algorithm = _parse_color_algorithm(optarg);
            if (color_algorithm == COLOR_ALGORITHMS) exit(EXIT_FAILURE);
            break;
        case 'p':
            palette = _parse_color_palette(optarg);
            if (!palette) exit(EXIT_FAILURE);
            break;

        case 's':
            _print_args_description();
            exit(EXIT_SUCCESS);

        case '?':
            if (optopt == 'w' || optopt == 'h' || optopt == 'm' || optopt == 'e') {
                fprintf(stderr, "Option -%c requires an argument.\n", optopt);
            }
            else if (optopt == 'a' || optopt == 'p') {
                fprintf(stderr, "Option -%c requires an argument.\n", optopt);
            }
            else if (isprint(optopt)) {
                fprintf(stderr, "Unknown option `-%c`.\n", optopt);
            }
            else {
                fprintf(stderr, "Unknown option char `\\x%x`.\n", optopt);
            }

            fprintf(stderr, "\nUsage: %s [-w width] [-h height] [-m max_iters] ", argv[0]);
            fprintf(stderr, "[-e escape_bound] [-a coloring_algorithm] [-p color_palette]\n");

            exit(EXIT_FAILURE);

        default:
            // should not end up here
            exit(EXIT_FAILURE);
        }
    }

    fractal_config cfg = {
        .width = width,
        .height = height,
        .max_iters = max_iters,
        .escape_bound = escape_bound,
        .color_algorithm = color_algorithm,
        .color_palette = {"uf"},
    };
    if (palette) strcpy(cfg.color_palette, palette);

    return cfg;
}
