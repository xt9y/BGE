#include "Engine/app_hooks.h"
#include "Engine/state.h"
#include "game_internal.h"
void bge_app_update(level_data_t*l){if(!g_state||!l||state.id!=STATE_PLAYING||!state.win){g_game.shoot_pressed=false;g_game.gun_pressed=false;return;}game_player_update(&g_game.player,l);if(glfwGetMouseButton(state.win,GLFW_MOUSE_BUTTON_LEFT)==GLFW_PRESS){if(!g_game.shoot_pressed){game_player_shoot(l);game_gun_shot(&g_game.gun);g_game.shoot_pressed=true;}}else g_game.shoot_pressed=false;if(glfwGetKey(state.win,GLFW_KEY_G)==GLFW_PRESS){if(!g_game.gun_pressed){game_gun_next(&g_game.gun);g_game.gun_pressed=true;}}else g_game.gun_pressed=false;}
