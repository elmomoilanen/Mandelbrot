#ifndef __BITMAP__
#define __BITMAP__

#include "common.h"

#pragma pack(push, 2)

struct BitmapHeader {
    char header_field[2];
    i32 file_size;
    i32 reserved; // set to 0
    i32 data_offset;
    i32 header_size; // 40 bytes, rest of the fields this included
    i32 width;
    i32 height;
    i16 color_planes; // set to 1
    i16 bits_per_pixel; // 24 for RGB
    i32 compression; // set to 0 if no compression
    i32 image_size; // set to 0 for RGB bitmaps
    i32 horizontal_resolution; // print resolution, pixels/metre
    i32 vertical_resolution;
    i32 colors; // set to 0
    i32 important_colors; // set to 0
};

#pragma pack(pop)

struct BitmapData {
    i32 width;
    i32 height;
    u8 pixels[];
};

void bitmap_set_pixel(struct BitmapData *bitmap, RGBbits *color_bits, i32 x, i32 y);
bool bitmap_write(struct BitmapData *bitmap, const char *filename);

#endif // __BITMAP__
