#ifndef EDITOR_H
#define EDITOR_H

#include "level.h"
#include "cam.h"

typedef struct {
    level_data_t *level;
    level_quad_t *selected_quad;
    level_sector_data_t *selected_sector;
    i32 selected_wall_idx;
    bool is_dragging;
    vec3s drag_start_hit;
    vec3s drag_quad_start_pos;
    vec3s drag_quad_start_rot;
    vec2s drag_quad_start_size;
    vec3s drag_plane_normal;
    vec3s drag_cam_start_pos;
} editor_t;

typedef struct {
    bool hit;
    i32 sector_id;
    i32 wall_id;
    f32 distance;
    vec3s hit_position;
    level_quad_t* quad;
    level_sector_data_t* sector;
} editor_look_at_info_t;

void editor_render(void);
void editor_render_look_at_info(void);
editor_look_at_info_t editor_get_look_at_info(const level_data_t *level, const camera_t *cam, f32 max_distance);
void editor_update(void);
void editor_save(const level_data_t* level);

#endif

