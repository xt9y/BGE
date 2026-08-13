#include "Engine/app_hooks.h"
#include "Engine/text.h"
#include "game_internal.h"
void bge_app_render_scene_overlay(i32 w,i32 h){game_gun_render(&g_game.gun,w,h);}void bge_app_render_ui(i32 w,i32 h){if(w>0&&h>0)text_draw((vec2s){(f32)w*.5f-5,(f32)h*.5f-10},"+");}
