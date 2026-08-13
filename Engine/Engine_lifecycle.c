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
void engine_register_resources(void);

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

bool engine_init_impl(Engine* app, const char* level_path, const char* loader_expr, level_data_t level)
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

    if (!level_clone_owned(&level, &runtime->level) ||
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

    GL_START();
    runtime->gl_started = true;
    app->impl = runtime;
    bge_runtime = runtime;

    state.editor->level = &runtime->level;
    engine_register_resources();
    editor_init();

    state.cam->front = (vec3s){0.0f, 0.0f, -1.0f};
    state.cam->up = (vec3s){0.0f, 1.0f, 0.0f};
    state.cam->lastX = (f32)state.fb->ww * 0.5f;
    state.cam->lastY = (f32)state.fb->wh * 0.5f;
    state.cursor_locked = false;
    apply_level_camera(state.cam, &runtime->level);
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
            fprintf(stderr, "BGE: could not save level '%s'\n", runtime->level.path);

        editor_shutdown();
        gun_shutdown();
        render_shutdown();

        texture_t* private_font = texture_get_by_name("Engine/res/font.png");
        if (private_font) texture_destroy(private_font);

        GL_END();
    }

    bge_runtime = NULL;
    level_free_owned(&runtime->level);
    free(runtime);
    app->impl = NULL;
}
