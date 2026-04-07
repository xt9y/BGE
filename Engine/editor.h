#ifndef EDITOR_H
#define EDITOR_H

#include "level.h"
#include "cam.h"

typedef struct {
    level_data_t *level;
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
void editor_render_look_at_info(const level_data_t *level, const camera_t *cam);
editor_look_at_info_t editor_get_look_at_info(const level_data_t *level, const camera_t *cam, f32 max_distance);

#endif

