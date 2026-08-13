#ifndef BGE_LEVEL_H
#define BGE_LEVEL_H

#include <stddef.h>

#include "cam.h"
#include "util/math.h"
#include "util/types.h"

#define MAX_QUADS_PER_SECTOR 4096
#define MAX_SECTORS_PER_LEVEL 64
/* Compatibility for generated headers/tests created before the rename. */
#define MAX_WALL_VERTICES MAX_QUADS_PER_SECTOR

typedef struct {
    vec3s pos, rot, color;
    vec2s size;
    i32 tex_id, sector_id, portal_id;
    bool is_solid, is_invisible, is_billboard, portal_side_flip;
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

bool level_render_quad(const level_quad_t* quad, vec4s color, const camera_t* cam);
void level_render(const level_data_t* level, const camera_t* cam);
void level_renderer_shutdown(void);
void level_set_frustum(const f32* view, const f32* proj);
bool quad_visible(const level_quad_t* quad);

bool level_ray_intersects_quad(vec3s ray_origin, vec3s ray_dir, const level_quad_t* quad,
    f32* out_t, vec3s* out_hit, vec3s* out_local_hit);
bool level_get_height(const level_data_t* level, vec3s pos, f32* h);

bool level_clone_owned(const level_data_t* src, level_data_t* out);
void level_free_owned(level_data_t* level);
bool level_set_owned_path(level_data_t* level, const char* path);
bool level_save_header(const level_data_t* level, const char* path, const char* loader_name);

#endif
