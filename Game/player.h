#ifndef BGE_DEMO_PLAYER_H
#define BGE_DEMO_PLAYER_H

#include "Engine/level.h"

typedef struct {
    f32 move_speed;
    f32 eye_height;
    f32 collision_radius;
} game_player_t;

void game_player_init(game_player_t* player);
void game_player_update(game_player_t* player, level_data_t* level);
void game_player_shoot(level_data_t* level);

#endif
