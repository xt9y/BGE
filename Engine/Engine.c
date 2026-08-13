#include "Engine.h"
#include "App.h"
#include "editor.h"
#include "gun.h"
#include "render.h"
#include "runtime.h"
#include "state.h"
#include "text.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

engine_runtime* bge_runtime = NULL;

static bool path_exists(const char* path)
{
    FILE* f = fopen(path, "rb");
    if (!f) return false;
    fclose(f);
    return true;
}

static bool parse_loader_name(const char* expr, char* out, size_t cap)
{
    if (!expr || !out || cap < 2) return false;
    while (isspace((unsigned char)*expr)) ++expr;
    if (!(isalpha((unsigned char)*expr) || *expr == '_')) return false;

    size_t n = 0;
    while (isalnum((unsigned char)*expr) || *expr == '_') {
        if (n + 1 >= cap) return false;
        out[n++] = *expr++;
    }
    out[n] = '\0';

    while (isspace((unsigned char)*expr)) ++expr;
    if (*expr++ != '(') return false;
    while (isspace((unsigned char)*expr)) ++expr;
    if (*expr++ != ')') return false;
    while (isspace((unsigned char)*expr)) ++expr;
    return *expr == '\0';
}

static void register_textures(void)
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
}

static void register_weapons(void)
{
    gun_reg_init(state.gun);

    state.gun->defs[state.gun->count++] = (weapon_def_t){
        .tex_names = { "Engine/res/gun_doom.png" },
        .tex_count = 1,
        .flash_tex_name = "Engine/res/hand_shoot_flash.png",
        .gun_xy = {0.5f, 1.0f},
        .gun_size = 0.43f,
        .flash_size = 0.12f,
        .flash_xy = {0.53f, 0.44f},
        .frame_duration = 0.0f,
    };

    state.gun->defs[state.gun->count++] = (weapon_def_t){
        .tex_names = { "Engine/res/gun_portal.png" },
        .tex_count = 1,
        .flash_tex_name = "Engine/res/hand_shoot_flash.png",
        .gun_xy = {0.78f, 1.0f},
        .gun_size = 0.33f,
        .flash_size = 0.22f,
        .flash_xy = {0.32f, 0.39f},
        .frame_duration = 0.0f,
    };

    state.gun->defs[state.gun->count++] = (weapon_def_t){
        .tex_names = { "Engine/res/hand_shoot_3.png", "Engine/res/hand_shoot_4.png", "Engine/res/hand_shoot_5.png" },
        .tex_count = 3,
        .flash_tex_name = "Engine/res/hand_shoot_flash.png",
        .gun_xy = {0.67f, 1.0f},
        .gun_size = 0.52f,
        .flash_size = 0.07f,
        .flash_xy = {0.32f, 0.26f},
        .frame_duration = 0.04f,
    };

    gun_init();
}

bool engine_init_impl(Engine* app, const char* level_path, const char* loader_expr, level_data_t level)
{
    if (!app || app->impl || !level_path || !level_path[0] || bge_runtime) return false;

    engine_runtime* runtime = (engine_runtime*)calloc(1, sizeof(*runtime));
    if (!runtime) return false;

    if (!parse_loader_name(loader_expr, runtime->loader_name, sizeof(runtime->loader_name))) {
        fprintf(stderr, "BGE: engine_init expects a zero-argument loader call, got '%s'\n",
            loader_expr ? loader_expr : "");
        free(runtime);
        return false;
    }

    if (!level_clone_owned(&level, &runtime->level) ||
        !level_set_owned_path(&runtime->level, level_path)) {
        level_free_owned(&runtime->level);
        free(runtime);
        return false;
    }

    if (!path_exists(level_path) &&
        !level_save_header(&runtime->level, level_path, runtime->loader_name)) {
        fprintf(stderr, "BGE: failed to create level header '%s'\n", level_path);
        level_free_owned(&runtime->level);
        free(runtime);
        return false;
    }

    if (!GL_START()) {
        level_free_owned(&runtime->level);
        free(runtime);
        return false;
    }

    runtime->gl_started = true;
    app->impl = runtime;
    bge_runtime = runtime;

    state.level = &runtime->level;
    state.editor->level = state.level;

    register_textures();
    register_weapons();
    editor_init();

    state.cam->front = (vec3s){0.0f, 0.0f, -1.0f};
    state.cam->up = (vec3s){0.0f, 1.0f, 0.0f};
    state.cam->lastX = (f32)state.fb->ww * 0.5f;
    state.cam->lastY = (f32)state.fb->wh * 0.5f;
    state.cursor_locked = false;
    apply_level_camera(state.cam, state.level);

    return true;
}

void engine_run(Engine* app)
{
    if (!app || !app->impl || app->impl != bge_runtime) return;

    while (GL_FRAME()) {
        if (state.id == STATE_EDITOR) editor_update();
    }
}

void engine_destroy(Engine* app)
{
    if (!app || !app->impl) return;

    engine_runtime* runtime = (engine_runtime*)app->impl;
    if (runtime->gl_started) {
        if (state.cam) {
            runtime->level.cam.pos = state.cam->pos;
            runtime->level.cam.yaw = state.cam->yaw;
            runtime->level.cam.pitch = state.cam->pitch;
        }

        if (!level_save_header(&runtime->level, runtime->level.path, runtime->loader_name))
            fprintf(stderr, "BGE: failed to save level '%s'\n",
                runtime->level.path ? runtime->level.path : "");

        editor_shutdown();
        gun_shutdown();
        render_shutdown();
        GL_END();
    }

    state.level = NULL;
    bge_runtime = NULL;
    level_free_owned(&runtime->level);
    free(runtime);
    app->impl = NULL;
}
