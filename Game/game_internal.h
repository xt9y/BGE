#ifndef BGE_DEMO_GAME_INTERNAL_H
#define BGE_DEMO_GAME_INTERNAL_H
#include "gun.h"
#include "player.h"
typedef struct{game_player_t player;game_gun_t gun;bool shoot_pressed,gun_pressed;}demo_game_t;
extern demo_game_t g_game;
#endif
