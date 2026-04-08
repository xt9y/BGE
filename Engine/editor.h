#ifndef EDITOR_H
#define EDITOR_H

#include "level.h"
#include "cam.h"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

typedef enum {
    EDITOR_FREE = 0,
    EDITOR_WALL_SELECTED,
    EDITOR_EDITING_POINT
} editor_mode_t;

typedef enum {
    POINT_TOP = 0,
    POINT_TOP_RIGHT,
    POINT_RIGHT,
    POINT_BOTTOM_RIGHT,
    POINT_BOTTOM,
    POINT_BOTTOM_LEFT,
    POINT_LEFT,
    POINT_TOP_LEFT,
    POINT_COUNT
} wall_point_t;

typedef struct {
    level_data_t *level;
    editor_mode_t mode;
    i32 selected_sector;
    i32 selected_wall;
    wall_point_t selected_point;
    f32 move_speed;
    level_quad_t *edit_quads;
    i32 edit_quad_count;
    bool _prev_keys[GLFW_KEY_LAST + 1];
} editor_t;

typedef struct {
    bool hit;
    i32 sector_id;
    i32 wall_id;
    f32 distance;
    vec3s hit_position;
    const level_quad_t* quad;
    const level_sector_data_t* sector;
} editor_look_at_info_t;

void editor_render(const level_data_t *level);
void editor_render_look_at_info(const level_data_t *level, const camera_t *cam, const editor_t *ed);
editor_look_at_info_t editor_get_look_at_info(const level_data_t *level, const camera_t *cam, f32 max_distance);

void editor_handle_input(editor_t *ed, const camera_t *cam);
void editor_render_wireframe(const level_data_t *level, const editor_t *ed);
void get_world_point(const level_quad_t *quad, wall_point_t pt, vec3s *out);
void editor_init_quads(editor_t *ed, level_data_t *level);
void editor_free_quads(editor_t *ed);

#endif

