#include <string.h>

#include "palette.h"

RGB palette_uf[] = {
    {.red = 66.0, .green = 30.0, .blue = 15.0},
    {.red = 25.0, .green = 7.0, .blue = 26.0},
    {.red = 9.0, .green = 1.0, .blue = 47.0},
    {.red = 4.0, .green = 4.0, .blue = 73.0},
    {.red = 0.0, .green = 7.0, .blue = 100.0},
    {.red = 12.0, .green = 44.0, .blue = 138.0},
    {.red = 24.0, .green = 82.0, .blue = 177.0},
    {.red = 57.0, .green = 125.0, .blue = 209.0},
    {.red = 134.0, .green = 181.0, .blue = 229.0},
    {.red = 211.0, .green = 236.0, .blue = 248.0},
    {.red = 241.0, .green = 233.0, .blue = 191.0},
    {.red = 248.0, .green = 201.0, .blue = 95.0},
    {.red = 255.0, .green = 170.0, .blue = 0.0},
    {.red = 204.0, .green = 128.0, .blue = 0.0},
    {.red = 153.0, .green = 87.0, .blue = 0.0},
    {.red = 106.0, .green = 52.0, .blue = 3.0},
};

RGB palette_or[] = {
    {.red = 180.0, .green = 64.0, .blue = 16.0},
    {.red = 229.0, .green = 83.0, .blue = 0.0},
    {.red = 255.0, .green = 105.0, .blue = 0.0},
    {.red = 255.0, .green = 43.0, .blue = 0.0},
    {.red = 255.0, .green = 64.0, .blue = 35.0},
    {.red = 255.0, .green = 92.0, .blue = 0.0},
    {.red = 255.0, .green = 129.0, .blue = 0.0},
    {.red = 255.0, .green = 159.0, .blue = 0.0},
    {.red = 255.0, .green = 195.0, .blue = 77.0},
    {.red = 235.0, .green = 190.0, .blue = 15.0},
    {.red = 220.0, .green = 195.0, .blue = 15.0},
    {.red = 215.0, .green = 252.0, .blue = 0.0},
};

RGB palette_lb[] = {
    {.red = 47.0, .green = 86.0, .blue = 233.0},
    {.red = 45.0, .green = 100.0, .blue = 245.0},
    {.red = 47.0, .green = 141.0, .blue = 255.0},
    {.red = 51.0, .green = 171.0, .blue = 249.0},
    {.red = 52.0, .green = 204.0, .blue = 255.0},
    {.red = 82.0, .green = 219.0, .blue = 255.0},
    {.red = 23.0, .green = 236.0, .blue = 236.0},
    {.red = 110.0, .green = 255.0, .blue = 255.0},
    {.red = 168.0, .green = 255.0, .blue = 255.0},
    {.red = 149.0, .green = 212.0, .blue = 122.0},
    {.red = 169.0, .green = 255.0, .blue = 47.0},
    {.red = 255.0, .green = 173.0, .blue = 47.0},
};

RGB palette_re[] = {
    {.red = 139.0, .green = 0.0, .blue = 0.0},
    {.red = 178.0, .green = 34.0, .blue = 34.0},
    {.red = 255.0, .green = 99.0, .blue = 71.0},
    {.red = 255.0, .green = 127.0, .blue = 80.0},
    {.red = 233.0, .green = 150.0, .blue = 122.0},
    {.red = 255.0, .green = 160.0, .blue = 122.0},
    {.red = 184.0, .green = 134.0, .blue = 11.0},
    {.red = 218.0, .green = 165.0, .blue = 32.0},
    {.red = 240.0, .green = 230.0, .blue = 140.0},
};

RGB palette_gr[] = {
    {.red = 154.0, .green = 205.0, .blue = 50.0},
    {.red = 128.0, .green = 128.0, .blue = 0.0},
    {.red = 85.0, .green = 107.0, .blue = 47.0},
    {.red = 107.0, .green = 142.0, .blue = 35.0},
    {.red = 0.0, .green = 100.0, .blue = 0.0},
    {.red = 0.0, .green = 128.0, .blue = 0.0},
    {.red = 34.0, .green = 139.0, .blue = 34.0},
    {.red = 0.0, .green = 255.0, .blue = 0.0},
    {.red = 50.0, .green = 205.0, .blue = 50.0},
    {.red = 143.0, .green = 188.0, .blue = 143.0},
};

palette_t get_palette(char *name) {
    palette_t palette = {0};
    
    if (strncmp(name, "or", 2) == 0) {
        palette.colors = COLORS_PALETTE_OR;
        palette.palette = &palette_or[0];

    } else if (strncmp(name, "lb", 2) == 0) {
        palette.colors = COLORS_PALETTE_LB;
        palette.palette = &palette_lb[0];

    } else if (strncmp(name, "re", 2) == 0) {
        palette.colors = COLORS_PALETTE_RE;
        palette.palette = &palette_re[0];

    } else if (strncmp(name, "gr", 2) == 0) {
        palette.colors = COLORS_PALETTE_GR;
        palette.palette = &palette_gr[0];
        
    } else {
        palette.colors = COLORS_PALETTE_UF;
        palette.palette = &palette_uf[0];
    }

    return palette;
}
