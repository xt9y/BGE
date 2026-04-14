#ifndef EDITOR_H
#define EDITOR_H

#include "level.h"
#include "cam.h"

typedef enum {
    EDGE_NONE = 0,
    EDGE_TOP,
    EDGE_BOTTOM,
    EDGE_LEFT,
    EDGE_RIGHT
} editor_edge_t;

typedef struct {
    level_data_t *level;
    level_quad_t *selected_quad;
    level_sector_data_t *selected_sector;
    i32 selected_wall_idx;
    bool is_dragging;
    editor_edge_t drag_edge;
    editor_edge_t hover_edge;
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
    vec3s local_hit;
    level_quad_t* quad;
    level_sector_data_t* sector;
} editor_look_at_info_t;

void editor_render();
void editor_update();
void editor_save(level_data_t* level);

#endif

