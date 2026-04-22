#ifndef EDITOR_H
#define EDITOR_H

#include "level.h"
#include "cam.h"

typedef enum {
    EDITOR_IDLE = 0,
    EDITOR_DRAG,
    EDITOR_RESIZE_TOP,
    EDITOR_RESIZE_RIGHT,
    EDITOR_PAINT
} editor_e;

typedef enum {
    EDITOR_MOD_NONE      = 0,
    EDITOR_MOD_SECTOR    = 1 << 0,
    EDITOR_MOD_TEXTURE   = 1 << 1,
    EDITOR_MOD_COLOR     = 1 << 2,
    EDITOR_MOD_ROTATION  = 1 << 3,
    EDITOR_MOD_ALL       = 0xFF
} editor_mod_e;

typedef struct {
    level_data_t *level;
    level_quad_t *selected_quad;
    level_sector_data_t *selected_sector;
    i32 selected_wall_idx;
    
    editor_e id;
    editor_e hover_id;
    
    vec3s drag_start_hit,
          drag_quad_start_pos,
          drag_quad_start_rot,
          drag_plane_normal,
          drag_cam_start_pos;

    vec2s drag_quad_start_size;
    
    level_quad_t template_quad;
    u32 template_mods;
} editor_t;

typedef struct {
    bool hit;
    i32 sector_id, wall_id;
    vec3s hit_position, local_hit;
    f32 distance;
    level_quad_t* quad;
    level_sector_data_t* sector;
} editor_look_at_info_t;

void editor_render();
void editor_update();
void editor_save(level_data_t* level);

void editor_add_quad(level_sector_data_t* sector, const level_quad_t* template);
void editor_delete_quad(level_sector_data_t* sector, i32 idx);
void editor_add_sector(level_data_t* level);
void editor_move_quad_to_sector(level_sector_data_t* from, level_sector_data_t* to, i32 idx);
int count_portal_quads(level_data_t* level, int portal_id);

static level_quad_t get_default_quad(camera_t *cam) 
{
    return (level_quad_t) { 
        .pos = {roundf(cam->pos.x + cam->front.x * 3.0f), roundf(cam->pos.y + cam->front.y * 3.0f), roundf(cam->pos.z + cam->front.z * 3.0f)}, 
        .rot = {0, 0, 0}, .size = {2, 2}, .tex_id = 0, .color = {1, 1, 1}, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .sector_id = 0
    };
}

#endif
