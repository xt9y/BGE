#ifndef LEVEL_H
#define LEVEL_H

#include "util/types.h"
#include "util/math.h"
#include "cam.h"

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
    camera_t cam;
} level_data_t;

void level_render(const level_data_t *level);
bool level_ray_intersects_quad(const vec3s ray_origin, const vec3s ray_dir, const level_quad_t* quad, f32* out_t, vec3s* out_hit, vec3s* out_local_hit);
bool level_get_height(const level_data_t* level, vec3s pos, f32* h);

#endif // LEVEL_H
