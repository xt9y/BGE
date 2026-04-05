#include "Engine/level.h"
#include "Engine/editor.h"
#include "Engine/App.h"

#include "Engine/res/level1.h"
#include "Engine/res/level2.h"
#include "Engine/res/level3.h"
#include "Engine/res/level4.h"

/*
 * TL:DR
 * > start with 'make'
 * > press b to switch between all levels
 * > press e to enter editor mode
 * > walk look around w a s d + mouse
 */

void RUN()
{
    GL_START();

    {   // Camera
        state.fb->w = WIDTH, state.fb->h = HEIGHT;
        glfwGetFramebufferSize(state.win, &state.fb->w, &state.fb->h);
        state.cam->pos = (vec3s){9.8f, 6.4f, 13.6f};
        state.cam->front = (vec3s){0.0f, 0.0f, -1.0f};
        state.cam->up = (vec3s){0.0f, 1.0f, 0.0f};
        state.cam->yaw = 235.0f;
        state.cam->pitch = -16.0f;
        state.cam->lastX = (f32)state.fb->w * 0.5f;
        state.cam->lastY = (f32)state.fb->h * 0.5f;
        state.cursor_locked = false;
        update_camera_vectors(state.cam);
    }

    {   // Textures
        texture_registry_init(state.text);
        state.text->textures[state.text->count++] = *texture_create("Engine/res/ground.png", TEX_FILTER_LINEAR, TEX_WRAP_REPEAT);
        state.text->textures[state.text->count++] = *texture_create("Engine/res/stone.png", TEX_FILTER_LINEAR, TEX_WRAP_REPEAT);
        state.text->textures[state.text->count++] = *texture_create("Engine/res/awesomeface.png", TEX_FILTER_LINEAR, TEX_WRAP_REPEAT);
        state.text->textures[state.text->count++] = *texture_create_solid(255, 255, 255);
        text_init(NULL);
    }

    {   // Levels 
        state.level_count = 0;
        state.levels[state.level_count++] = load_1();
        state.levels[state.level_count++] = load_2();
        state.levels[state.level_count++] = load_3();
        state.levels[state.level_count++] = load_4();
    }

    {   // Editor
        state.editor->level = &state.levels[0];
    }

    while (GL_FRAME()) 
    {
        state.editor->level = &state.levels[state.level_id];
        // Do whatever
    }

#define END() do { GL_END(); } while (0)
    END();
}

void RENDER()
{
    state.fb->w = 0, state.fb->h = 0;
    glfwGetFramebufferSize(state.win, &state.fb->w, &state.fb->h);
    glViewport(0, 0, state.fb->w, state.fb->h);

    // glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(state.data->program);
    
    if (state.id == STATE_PLAYING)
    {
        f32 model[16], view[16], proj[16];
        mat4_identity(model);
        mat4_lookat(view, state.cam->pos, vec3_add(state.cam->pos, state.cam->front), state.cam->up);
        mat4_perspective(proj, DEG2RAD(45.0f), (f32)state.fb->w / (f32)state.fb->h, 0.1f, 100.0f);

        glUniformMatrix4fv(glGetUniformLocation(state.data->program, "model"), 1, GL_FALSE, model);
        glUniformMatrix4fv(glGetUniformLocation(state.data->program, "view"), 1, GL_FALSE, view);
        glUniformMatrix4fv(glGetUniformLocation(state.data->program, "projection"), 1, GL_FALSE, proj);

        level_render(state.editor->level);
    }
    
    if (state.id == STATE_EDITOR)
    {
        f32 min_x = 1000.0f, max_x = -1000.0f, min_z = 1000.0f, max_z = -1000.0f;

        for (i32 s = 0; s < state.editor->level->sector_count; s++)
        {
            for (i32 i = 0; i < state.editor->level->sectors[s].quad_count; i++)
            {
                f32 h = state.editor->level->sectors[s].quads[i].size.x * 0.5f;
                if (state.editor->level->sectors[s].quads[i].pos.x - h < min_x) min_x = state.editor->level->sectors[s].quads[i].pos.x - h;
                if (state.editor->level->sectors[s].quads[i].pos.x + h > max_x) max_x = state.editor->level->sectors[s].quads[i].pos.x + h;
                if (state.editor->level->sectors[s].quads[i].pos.z - h < min_z) min_z = state.editor->level->sectors[s].quads[i].pos.z - h;
                if (state.editor->level->sectors[s].quads[i].pos.z + h > max_z) max_z = state.editor->level->sectors[s].quads[i].pos.z + h;
            }
        }

        f32 pad = 5.0f; min_x -= pad; max_x += pad; min_z -= pad; max_z += pad;

        f32 cx = (min_x + max_x) * 0.5f;
        f32 cz = (min_z + max_z) * 0.5f;
        f32 dx = (max_x - min_x) * 0.5f;
        f32 dz = (max_z - min_z) * 0.5f;

        f32 model[16], view[16], proj[16];
        mat4_identity(model);
        mat4_lookat(view, (vec3s){cx, (dx > dz ? dx : dz / tanf(DEG2RAD(45.0f))), cz}, (vec3s){cx, 0.0f, cz}, (vec3s){0.0f, 0.0f, -1.0f});
        mat4_perspective(proj, DEG2RAD(90.0f), (f32)state.fb->w / (f32)state.fb->h, 0.1f, 500.0f);

        glUniformMatrix4fv(glGetUniformLocation(state.data->program, "model"), 1, GL_FALSE, model);
        glUniformMatrix4fv(glGetUniformLocation(state.data->program, "view"), 1, GL_FALSE, view);
        glUniformMatrix4fv(glGetUniformLocation(state.data->program, "projection"), 1, GL_FALSE, proj);

        editor_render(state.editor->level);
    }

    // Draw UI text
    text_begin();
    text_draw((vec2s){10.0f, 10.0f}, ":;<=>? 0123456789 ABCDEFGHIJKLMNOPQRSTUVWXYZ _ abcdefghijklmnopqrstuvwxyz. ");
    text_draw((vec2s){10.0f, 30.0f}, "FPS %.1f", GL_GETFPS());
    text_draw((vec2s){10.0f, 50.0f}, "POS: %.1f %.1f %.1f ; YAW %.1f ; PITCH %.1f", state.cam->pos.x, state.cam->pos.y, state.cam->pos.z, state.cam->yaw, state.cam->pitch);
    text_draw((vec2s){10.0f, 70.0f}, "CURRENT_LVL: %d ; MAX_LVLS: %d", state.level_id + 1, state.level_count);
    text_draw((vec2s){10.0f, 90.0f}, "STATE: %s", state.toString[state.id]);
    text_flush(state.fb->w, state.fb->h);
}

void INPUT()
{
    glfwSetInputMode(state.win, GLFW_CURSOR, state.cursor_locked ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
    
    {   // Free mouse
        static bool shift_pressed = false;
        if (glfwGetKey(state.win, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
            if (!shift_pressed) 
            {
                state.cursor_locked = !state.cursor_locked;
                if (state.cursor_locked) {
                    glfwSetCursorPos(state.win, state.fb->w * 0.5f, state.fb->h * 0.5f);
                    state.cam->firstMouse = true;
                }
                shift_pressed = true;
            }
        }
        else shift_pressed = false;
    }

    {   // Toggle editor
        static bool e_pressed = false;
        if (glfwGetKey(state.win, GLFW_KEY_E) == GLFW_PRESS) {
            if (!e_pressed)
            {
                state.id = state.id == STATE_EDITOR ? STATE_PLAYING : STATE_EDITOR;
                e_pressed = true;
            }
        }
        else e_pressed = false;
    }

    {   // Next level
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

    if (glfwGetKey(state.win, GLFW_KEY_ESCAPE) == GLFW_PRESS) state.id = STATE_EXIT;

    if (state.cursor_locked && state.id == STATE_PLAYING)
    {
        const f32 speed = 2.5f * state.dt;
        const vec3s right = vec3_normalize(vec3_cross(state.cam->front, state.cam->up));

        if (glfwGetKey(state.win, GLFW_KEY_W) == GLFW_PRESS) state.cam->pos = vec3_add(state.cam->pos, vec3_scale(state.cam->front, speed));
        if (glfwGetKey(state.win, GLFW_KEY_S) == GLFW_PRESS) state.cam->pos = vec3_sub(state.cam->pos, vec3_scale(state.cam->front, speed));
        if (glfwGetKey(state.win, GLFW_KEY_A) == GLFW_PRESS) state.cam->pos = vec3_sub(state.cam->pos, vec3_scale(right, speed));
        if (glfwGetKey(state.win, GLFW_KEY_D) == GLFW_PRESS) state.cam->pos = vec3_add(state.cam->pos, vec3_scale(right, speed));
    }

    if (state.cursor_locked && state.id == STATE_EDITOR)
    {

    }
}

ENGINE_ENTRY_POINT
