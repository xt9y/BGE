#include "gun.h"
#include "render.h"
#include "state.h"
#include "text.h"

void engine_register_resources(void)
{
    texture_registry_init(state.text);
    texture_create("Engine/res/ground.png", TEX_FILTER_LINEAR, TEX_WRAP_REPEAT);
    texture_create("Engine/res/stone.png", TEX_FILTER_LINEAR, TEX_WRAP_REPEAT);
    texture_create("Engine/res/awesomeface.png", TEX_FILTER_LINEAR, TEX_WRAP_REPEAT);
    texture_create_solid(255, 255, 255);
    texture_create("Engine/res/metal_a.png", TEX_FILTER_LINEAR, TEX_WRAP_REPEAT);
    texture_create("Engine/res/metal_b.png", TEX_FILTER_LINEAR, TEX_WRAP_REPEAT);
    texture_create("Engine/res/grate.png", TEX_FILTER_LINEAR, TEX_WRAP_REPEAT);
    texture_create("Engine/res/spider.png", TEX_FILTER_LINEAR, TEX_WRAP_REPEAT);
    texture_create("Engine/res/banana.png", TEX_FILTER_LINEAR, TEX_WRAP_REPEAT);
    texture_create("Engine/res/water.png", TEX_FILTER_LINEAR, TEX_WRAP_REPEAT);
    texture_create("Engine/res/gun_doom.png", TEX_FILTER_LINEAR, TEX_WRAP_REPEAT);
    texture_create("Engine/res/gun_portal.png", TEX_FILTER_LINEAR, TEX_WRAP_REPEAT);
    texture_create("Engine/res/hand_shoot_flash.png", TEX_FILTER_LINEAR, TEX_WRAP_REPEAT);
    texture_create("Engine/res/hand_shoot_3.png", TEX_FILTER_LINEAR, TEX_WRAP_REPEAT);
    texture_create("Engine/res/hand_shoot_4.png", TEX_FILTER_LINEAR, TEX_WRAP_REPEAT);
    texture_create("Engine/res/hand_shoot_5.png", TEX_FILTER_LINEAR, TEX_WRAP_REPEAT);
    text_init();
    render_init();

    gun_reg_init(state.gun);
    state.gun->defs[state.gun->count++] = (weapon_def_t){
        .tex_names = { "Engine/res/gun_doom.png" }, .tex_count = 1,
        .flash_tex_name = "Engine/res/hand_shoot_flash.png",
        .gun_xy = {0.5f, 1.0f}, .gun_size = 0.43f,
        .flash_size = 0.12f, .flash_xy = {0.53f, 0.44f}, .frame_duration = 0.0f,
    };
    state.gun->defs[state.gun->count++] = (weapon_def_t){
        .tex_names = { "Engine/res/gun_portal.png" }, .tex_count = 1,
        .flash_tex_name = "Engine/res/hand_shoot_flash.png",
        .gun_xy = {0.78f, 1.0f}, .gun_size = 0.33f,
        .flash_size = 0.22f, .flash_xy = {0.32f, 0.39f}, .frame_duration = 0.0f,
    };
    state.gun->defs[state.gun->count++] = (weapon_def_t){
        .tex_names = { "Engine/res/hand_shoot_3.png", "Engine/res/hand_shoot_4.png", "Engine/res/hand_shoot_5.png" },
        .tex_count = 3,
        .flash_tex_name = "Engine/res/hand_shoot_flash.png",
        .gun_xy = {0.67f, 1.0f}, .gun_size = 0.52f,
        .flash_size = 0.07f, .flash_xy = {0.32f, 0.26f}, .frame_duration = 0.04f,
    };
    gun_init();
}
