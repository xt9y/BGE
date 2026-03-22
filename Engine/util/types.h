#ifndef TYPES_H
#define TYPES_H

#include <stdint.h>
#include <stdbool.h>

typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef int8_t   i8;
typedef int16_t  i16;
typedef int32_t  i32;
typedef int64_t  i64;
typedef float    f32;
typedef double   f64;

typedef struct vec2s { float x, y; } vec2s;
typedef struct vec3s { float x, y, z; } vec3s;
typedef struct vec4s { float x, y, z, w; } vec4s;
typedef struct ivec2s { int x, y; } ivec2s;

#endif
