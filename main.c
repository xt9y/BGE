#define LEVEL_RENDERING
#include "level.h"
#include "Engine/App.h"
#include "Engine/util.h"

#include "Engine/res/level1.h"
#include "Engine/res/level2.h"

void RUN()
{
    GL_START();

    {   // Camera
        state.fb->w = WIDTH, state.fb->h = HEIGHT;
        glfwGetFramebufferSize(state.win, &state.fb->w, &state.fb->h);
        state.cam->pos = (vec3s){0.0f, 1.5f, 0.0f};
        state.cam->front = (vec3s){0.0f, 0.0f, -1.0f};
        state.cam->up = (vec3s){0.0f, 1.0f, 0.0f};
        state.cam->yaw = 0.0f;
        state.cam->pitch = 0.0f;
        state.cam->lastX = (f32)state.fb->w * 0.5f;
        state.cam->lastY = (f32)state.fb->h * 0.5f;
        state.cam->firstMouse = true;
        update_camera_vectors(state.cam);
    }

    {   // Textures
        texture_registry_init(state.text);
        state.text->textures[state.text->count++] = *texture_create("Engine/res/ground.png", TEX_FILTER_LINEAR, TEX_WRAP_REPEAT);
        state.text->textures[state.text->count++] = *texture_create("Engine/res/stone.png", TEX_FILTER_LINEAR, TEX_WRAP_REPEAT);
        state.text->textures[state.text->count++] = *texture_create("Engine/res/awesomeface.png", TEX_FILTER_LINEAR, TEX_WRAP_REPEAT);
        state.text->textures[state.text->count++] = *texture_create_solid(255, 255, 255);
        state.text->textures[state.text->count++] = *texture_create("Engine/res/font.png", TEX_FILTER_NEAREST, TEX_WRAP_CLAMP_TO_EDGE);
        text_init(&state.text->textures[state.text->count - 1]);
    }

    {   // Levels 
        state.level_count = 0;
        state.levels[state.level_count++] = load_1();
        state.levels[state.level_count++] = load_2();

        state.current_sector = level_find_player_sector(&state.levels[0], state.cam->pos);
    }

    while (GL_FRAME()) RENDER();

#define END() do { GL_END(); for (int i = 0; i < state.level_count; i++) level_cleanup(&state.levels[i]); } while (0)
    END();
}

void RENDER()
{
    state.fb->w = 0, state.fb->h = 0;
    glfwGetFramebufferSize(state.win, &state.fb->w, &state.fb->h);
    glViewport(0, 0, state.fb->w, state.fb->h);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(state.data->program);

    f32 model[16], view[16], proj[16];
    mat4_identity(model);
    mat4_lookat(view, state.cam->pos, vec3_add(state.cam->pos, state.cam->front), state.cam->up);
    mat4_perspective(proj, DEG2RAD(45.0f), (f32)state.fb->w / (f32)state.fb->h, 0.1f, 100.0f);

    glUniformMatrix4fv(glGetUniformLocation(state.data->program, "view"), 1, GL_FALSE, view);
    glUniformMatrix4fv(glGetUniformLocation(state.data->program, "projection"), 1, GL_FALSE, proj);

    // Render level
    level_render(&state.levels[state.level_id]);

    // Draw UI text
    text_begin();
    text_draw((vec2s){10.0f, 10.0f}, ":;<=>? 0123456789 ABCDEFGHIJKLMNOPQRSTUVWXYZ _ abcdefghijklmnopqrstuvwxyz. ");
    text_drawf((vec2s){10.0f, 26.0f}, "FPS %.1f", GL_GETFPS());
    text_drawf((vec2s){10.0f, 42.0f}, "current_level: %d max_levels: %d", state.level_id + 1, state.level_count);
    text_flush(state.fb->w, state.fb->h);
    const vec3s old_pos = state.cam->pos;
    level_check_collision(&state.levels[state.level_id], &state.cam->pos, old_pos);
    state.current_sector = level_find_player_sector(&state.levels[state.level_id], state.cam->pos);
}

void INPUT()
{
    if (glfwGetKey(state.win, GLFW_KEY_ESCAPE) == GLFW_PRESS) state.id = STATE_EXIT;

    const bool shift_active = glfwGetKey(state.win, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS;
    if (shift_active && state.cursor_locked) {
        glfwSetInputMode(state.win, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        state.cursor_locked = false;
    } else if (!shift_active && !state.cursor_locked) {
        glfwSetInputMode(state.win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        state.cursor_locked = true;
        state.cam->firstMouse = true;
    }

    if (state.cursor_locked)
    {
        if (!state.cam || !state.win) return;
        const f32 speed = 2.5f * state.dt;
        const vec3s right = vec3_normalize(vec3_cross(state.cam->front, state.cam->up));

        if (glfwGetKey(state.win, GLFW_KEY_W) == GLFW_PRESS) state.cam->pos = vec3_add(state.cam->pos, vec3_scale(state.cam->front, speed));
        if (glfwGetKey(state.win, GLFW_KEY_S) == GLFW_PRESS) state.cam->pos = vec3_sub(state.cam->pos, vec3_scale(state.cam->front, speed));
        if (glfwGetKey(state.win, GLFW_KEY_A) == GLFW_PRESS) state.cam->pos = vec3_sub(state.cam->pos, vec3_scale(right, speed));
        if (glfwGetKey(state.win, GLFW_KEY_D) == GLFW_PRESS) state.cam->pos = vec3_add(state.cam->pos, vec3_scale(right, speed));
    }

    static bool b_pressed = false;
    if (glfwGetKey(state.win, GLFW_KEY_B) == GLFW_PRESS)
    {
        if (!b_pressed)
        {
            state.level_id = (state.level_id + 1) % state.level_count;
            b_pressed = true;
        }
    }
    else b_pressed = false;
}

ENGINE_ENTRY_POINT
