#ifndef LEVEL_H
#define LEVEL_H

#include "util/types.h"
#include "util/math.h"

#define MAX_WALL_VERTICES 4096
#define MAX_SECTORS_PER_LEVEL 64

typedef struct
{
    vec3s pos;
    vec3s rot;
    vec2s size;
    i32 tex_idx;
    bool is_solid;
    bool is_invisible;
    vec3s color;
} level_quad_t;

typedef struct
{
    i32 id;
    f32 light_intensity;
    const level_quad_t* quads;
    i32 quad_count;
} level_sector_data_t;

typedef struct
{
    const char* name;
    const char* path;
    const level_sector_data_t* sectors;
    i32 sector_count;
} level_data_t;

void level_render(const level_data_t *level);

#endif // LEVEL_H
