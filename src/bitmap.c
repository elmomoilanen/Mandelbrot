#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "bitmap.h"


void bitmap_set_pixel(struct BitmapData *bitmap, RGBbits *color_bits, i32 x, i32 y) {
    // Pixel array [x,y]: x determines column and y row, each pixel takes three bytes
    i32 const pixel_offset = x * 3 + bitmap->width * y * 3;

    *(bitmap->pixels + pixel_offset) = color_bits->blue;
    *(bitmap->pixels + pixel_offset + 1) = color_bits->green;
    *(bitmap->pixels + pixel_offset + 2) = color_bits->red;
}

void bitmap_write(struct BitmapData *bitmap, const char *filename) {
    struct BitmapHeader header = {
        .header_field = {'B', 'M'},
        .file_size = 0,
        .reserved = 0,
        .data_offset = 0,
        .header_size = 40,
        .width = bitmap->width,
        .height = bitmap->height,
        .color_planes = 1,
        .bits_per_pixel = 24,
        .compression = 0,
        .image_size = 0,
        .horizontal_resolution = 5000,
        .vertical_resolution = 4000,
        .colors = 0,
        .important_colors = 0
    };

    header.data_offset = sizeof(struct BitmapHeader);
    header.file_size = header.data_offset + header.width * header.height * 3;

    FILE *file;
    file = fopen(filename, "wb");

    if (file == NULL) {
        fprintf(stderr, "Error when opening file `%s`\n", filename);
        return;
    }

    fwrite(&header, sizeof(struct BitmapHeader), 1, file);
    fwrite(bitmap->pixels, header.width * header.height * 3, 1, file);

    if (fclose(file) != 0) {
        if (errno) {
            fprintf(stderr, "Error when closing the file `%s`: %s\n", filename, strerror(errno));
        } else {
            fprintf(stderr, "Unknown error when closing the file `%s`\n", filename);
        }
    }
}
