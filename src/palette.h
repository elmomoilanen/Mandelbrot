#ifndef __PALETTE__
#define __PALETTE__

#include "common.h"

#define COLORS_PALETTE_UF 16
extern RGB palette_uf[];

#define COLORS_PALETTE_OR 12
extern RGB palette_or[];

#define COLORS_PALETTE_LB 12
extern RGB palette_lb[];

#define COLORS_PALETTE_RE 9
extern RGB palette_re[];

#define COLORS_PALETTE_GR 10
extern RGB palette_gr[];

typedef struct {
    u32 colors;
    RGB *palette;
} palette_t;

typedef struct {
    char *name;
    char *long_name;
} palette_map;

palette_t get_palette(char *name);

#endif // __PALETTE__
