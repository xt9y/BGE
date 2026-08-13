#ifndef BGE_DEMO_GUN_H
#define BGE_DEMO_GUN_H
#include "Engine/text.h"
#define GAME_MAX_WEAPONS 8
#define GAME_MAX_GUN_FRAMES 8
typedef struct{const char*tex_names[GAME_MAX_GUN_FRAMES];i32 tex_count;const char*flash_tex_name;vec2s gun_xy;f32 gun_size,flash_size;vec2s flash_xy;f32 frame_duration;}game_weapon_def_t;
typedef struct{game_weapon_def_t defs[GAME_MAX_WEAPONS];i32 count,current;u32 vao,vbo;f32 swing,swing_velocity;bool a_previous,d_previous;f32 flash_timer,animation_time,elapsed;}game_gun_t;
void game_gun_reset(game_gun_t*);bool game_gun_init(game_gun_t*);void game_gun_shutdown(game_gun_t*);void game_gun_render(game_gun_t*,i32,i32);void game_gun_shot(game_gun_t*);void game_gun_next(game_gun_t*);
#endif
