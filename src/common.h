#ifndef __COMMON__
#define __COMMON__

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

typedef int8_t i8;
typedef uint8_t u8;
typedef int16_t i16;
typedef uint16_t u16;
typedef int32_t i32;
typedef uint32_t u32;
typedef int64_t i64;
typedef uint64_t u64;

typedef float f32;
typedef double f64;

typedef struct {
    u8 red;
    u8 green;
    u8 blue;
} RGBbits;

typedef struct {
    f64 red;
    f64 green;
    f64 blue;
} RGB;

#define U16_MAX UINT16_MAX
#define U32_MAX UINT32_MAX

#endif // __COMMON__
