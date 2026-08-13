#include "Engine.h"

#include "platform.h"
#include "app_hooks.h"
#include "editor.h"
#include "render.h"
#include "runtime.h"
#include "state.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

engine_runtime* bge_runtime = NULL;

bool engine_resources_init(void);
void engine_resources_shutdown(void);

static bool path_exists(const char* path)
{
    FILE* file = fopen(path, "rb");
    if (!file) return false;
    fclose(file);
    return true;
}

static bool parse_loader_name(const char* expression, char* out, const size_t capacity)
{
    if (!expression || !out || capacity < 2) return false;
    while (isspace((unsigned char)*expression)) ++expression;
    if (!(isalpha((unsigned char)*expression) || *expression == '_')) return false;

    size_t length = 0;
    while (isalnum((unsigned char)*expression) || *expression == '_') {
        if (length + 1 >= capacity) return false;
        out[length++] = *expression++;
    }
    out[length] = '\0';

    while (isspace((unsigned char)*expression)) ++expression;
    if (*expression++ != '(') return false;
    while (isspace((unsigned char)*expression)) ++expression;
    if (*expression++ != ')') return false;
    while (isspace((unsigned char)*expression)) ++expression;
    return *expression == '\0';
}

static void runtime_bind_state(engine_runtime* runtime)
{
    runtime->state_storage.fb = &runtime->framebuffer;
    runtime->state_storage.data = &runtime->data;
    runtime->state_storage.cam = &runtime->camera;
    runtime->state_storage.editor = &runtime->editor;
    runtime->state_storage.text = &runtime->textures;
    runtime->editor.level = &runtime->level;
}

static void runtime_cleanup(engine_runtime* runtime, const bool save_level)
{
    if (!runtime) return;

    if (save_level && runtime->platform_started && runtime->level.path && state.cam) {
        runtime->level.cam.pos = state.cam->pos;
        runtime->level.cam.yaw = state.cam->yaw;
        runtime->level.cam.pitch = state.cam->pitch;
        if (!level_save_header(&runtime->level, runtime->level.path, runtime->loader_name))
            fprintf(stderr, "BGE: could not save level '%s'\n", runtime->level.path);
    }

    if (runtime->app_started) {
        bge_app_shutdown();
        runtime->app_started = false;
    }
    if (runtime->editor_started) {
        editor_shutdown();
        runtime->editor_started = false;
    }
    if (runtime->resources_started) {
        engine_resources_shutdown();
        runtime->resources_started = false;
    }
    if (runtime->platform_started) {
        app_platform_shutdown();
        runtime->platform_started = false;
    }

    level_free_owned(&runtime->level);
}

bool engine_init_impl(Engine* app, const char* level_path, const char* loader_expr, level_data_t initial_level)
{
    if (!app || app->impl || bge_runtime || !level_path || !level_path[0]) return false;

    engine_runtime* runtime = (engine_runtime*)calloc(1, sizeof(*runtime));
    if (!runtime) return false;

    if (!parse_loader_name(loader_expr, runtime->loader_name, sizeof(runtime->loader_name))) {
        fprintf(stderr, "BGE: expected a zero-argument level loader, got '%s'\n",
            loader_expr ? loader_expr : "");
        free(runtime);
        return false;
    }

    if (!level_clone_owned(&initial_level, &runtime->level) ||
        !level_set_owned_path(&runtime->level, level_path)) {
        level_free_owned(&runtime->level);
        free(runtime);
        return false;
    }

    if (!path_exists(level_path) &&
        !level_save_header(&runtime->level, level_path, runtime->loader_name)) {
        fprintf(stderr, "BGE: could not create level '%s'\n", level_path);
        level_free_owned(&runtime->level);
        free(runtime);
        return false;
    }

    runtime_bind_state(runtime);
    g_state = &runtime->state_storage;
    bge_runtime = runtime;

    if (!app_platform_init()) goto fail;
    runtime->platform_started = true;

    if (!engine_resources_init()) goto fail;
    runtime->resources_started = true;

    state.cam->front = (vec3s){0,0,-1};
    state.cam->up = (vec3s){0,1,0};
    state.cam->lastX = (f32)state.fb->ww * 0.5f;
    state.cam->lastY = (f32)state.fb->wh * 0.5f;
    apply_level_camera(state.cam, &runtime->level);

    editor_init();
    runtime->editor_started = true;

    if (!bge_app_init(&runtime->level)) {
        fprintf(stderr, "BGE: application initialization failed\n");
        goto fail;
    }
    runtime->app_started = true;

    app->impl = runtime;
    return true;

fail:
    runtime_cleanup(runtime, false);
    bge_runtime = NULL;
    g_state = NULL;
    free(runtime);
    return false;
}

void engine_run(Engine* app)
{
    if (!app || !app->impl || app->impl != bge_runtime) return;

    while (app_frame_begin()) {
        engine_input();
        bge_app_update(&bge_runtime->level);
        if (state.id == STATE_EDITOR) editor_update();
        engine_render();
        app_frame_end();
    }
}

void engine_destroy(Engine* app)
{
    if (!app || !app->impl) return;
    engine_runtime* runtime = (engine_runtime*)app->impl;
    if (runtime != bge_runtime) return;

    runtime_cleanup(runtime, true);
    bge_runtime = NULL;
    g_state = NULL;
    free(runtime);
    app->impl = NULL;
}
