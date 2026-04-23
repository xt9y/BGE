#ifndef LEVEL_H
#define LEVEL_H

#include "util/types.h"
#include "util/math.h"
#include "cam.h"

#define MAX_WALL_VERTICES 4096
#define MAX_SECTORS_PER_LEVEL 64

typedef struct {
    vec3s pos, rot, color;
    vec2s size;
    i32 tex_id, sector_id, portal_id;
    bool is_solid, is_invisible, is_billboard, portal_side_flip;
    const camera_t* billboard_cam;
} level_quad_t;

typedef struct {
    i32 id, quad_count, quad_capacity;
    vec3s light;
    level_quad_t* quads;
} level_sector_data_t;

typedef struct {
    const char* name; 
    const char* path;
    level_sector_data_t* sectors;
    i32 sector_count, sector_capacity;
    camera_t cam;
} level_data_t;

typedef struct {
    const level_quad_t* skip_quad;
} level_render_options_t;

void ensure_vao();
void level_render_quad(const level_quad_t* quad, vec4s color);
void level_render(const level_data_t *level, const camera_t *cam, const level_quad_t *skip_quad);
bool level_ray_intersects_quad(const vec3s ray_origin, const vec3s ray_dir, const level_quad_t* quad, f32* out_t, vec3s* out_hit, vec3s* out_local_hit);
bool level_get_height(const level_data_t* level, vec3s pos, f32* h);

#endif // LEVEL_H
