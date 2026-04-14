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
    vec3s light;
    level_quad_t* quads;
    i32 quad_count;
    i32 quad_capacity;
} level_sector_data_t;

typedef struct
{
    const char* name;
    const char* path;
    level_sector_data_t* sectors;
    i32 sector_count;
    vec3s cam_pos;
    f32 cam_yaw;
    f32 cam_pitch;
} level_data_t;

void level_render(const level_data_t *level);

#endif // LEVEL_H
