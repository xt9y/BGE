#include "Engine/level.h"
#include "Engine/editor.h"
#include "Engine/App.h"
#include "Engine/portal.h"
#include "Engine/imgui_c.h"

#include "Engine/res/level1.h"
#include "Engine/res/level2.h"
#include "Engine/res/level3.h"
#include "Engine/res/level4.h"
#include "Engine/res/level5.h"
#include "Engine/res/level6.h"
#include "Engine/res/level7.h"
#include "Engine/res/level8.h"
#include "text.h"
#include "gun.h"
#include "render.h"

void RUN()
{
    GL_START();

    {   // Textures
        texture_registry_init(state.text);
        state.text->textures[state.text->count++] = *texture_create("Engine/res/ground.png", TEX_FILTER_LINEAR, TEX_WRAP_REPEAT);
        state.text->textures[state.text->count++] = *texture_create("Engine/res/stone.png", TEX_FILTER_LINEAR, TEX_WRAP_REPEAT);
        state.text->textures[state.text->count++] = *texture_create("Engine/res/awesomeface.png", TEX_FILTER_LINEAR, TEX_WRAP_REPEAT);
        state.text->textures[state.text->count++] = *texture_create_solid(255, 255, 255);
        state.text->textures[state.text->count++] = *texture_create("Engine/res/metal_a.png", TEX_FILTER_LINEAR, TEX_WRAP_REPEAT);
        state.text->textures[state.text->count++] = *texture_create("Engine/res/metal_b.png", TEX_FILTER_LINEAR, TEX_WRAP_REPEAT);
        state.text->textures[state.text->count++] = *texture_create("Engine/res/grate.png", TEX_FILTER_LINEAR, TEX_WRAP_REPEAT);
        state.text->textures[state.text->count++] = *texture_create("Engine/res/spider.png", TEX_FILTER_LINEAR, TEX_WRAP_REPEAT);
        state.text->textures[state.text->count++] = *texture_create("Engine/res/banana.png", TEX_FILTER_LINEAR, TEX_WRAP_REPEAT);
        state.text->textures[state.text->count++] = *texture_create("Engine/res/water.png", TEX_FILTER_LINEAR, TEX_WRAP_REPEAT);
        state.text->textures[state.text->count++] = *texture_create("Engine/res/gun_doom.png", TEX_FILTER_LINEAR, TEX_WRAP_REPEAT);
        state.text->textures[state.text->count++] = *texture_create("Engine/res/gun_portal.png", TEX_FILTER_LINEAR, TEX_WRAP_REPEAT);
        state.text->textures[state.text->count++] = *texture_create("Engine/res/hand_shoot_flash.png", TEX_FILTER_LINEAR, TEX_WRAP_REPEAT);
        state.text->textures[state.text->count++] = *texture_create("Engine/res/hand_shoot_3.png", TEX_FILTER_LINEAR, TEX_WRAP_REPEAT);
        state.text->textures[state.text->count++] = *texture_create("Engine/res/hand_shoot_4.png", TEX_FILTER_LINEAR, TEX_WRAP_REPEAT);
        state.text->textures[state.text->count++] = *texture_create("Engine/res/hand_shoot_5.png", TEX_FILTER_LINEAR, TEX_WRAP_REPEAT);
        text_init();
        render_init();
    }

    {   // Weapons
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

    {   // Levels
        state.level_count = 0;
        state.level_id = 0;
        state.levels[state.level_count++] = load_2();
        state.levels[state.level_count++] = load_3();
        state.levels[state.level_count++] = load_1();
        #define REGISTER_SAVED(loader) \
            do { level_data_t lv = loader(); if (lv.sector_count > 0 && state.level_count < MAX_LEVELS) state.levels[state.level_count++] = lv; } while (0)
        REGISTER_SAVED(load_4);
        REGISTER_SAVED(load_5);
        REGISTER_SAVED(load_6);
        REGISTER_SAVED(load_7);
        REGISTER_SAVED(load_8);
        #undef REGISTER_SAVED
    }

    {   // Editor
        state.editor->level = &state.levels[state.level_id];
    }

    {   // Camera
        state.cam->front = (vec3s){0.0f, 0.0f, -1.0f};
        state.cam->up = (vec3s){0.0f, 1.0f, 0.0f};
        state.cam->lastX = (f32)state.fb->ww * 0.5f;
        state.cam->lastY = (f32)state.fb->wh * 0.5f;
        state.cursor_locked = false;
        apply_level_camera(state.cam, &state.levels[state.level_id]);
    }

    while (GL_FRAME())
    {
        state.editor->level = &state.levels[state.level_id];
        if (state.id == STATE_EDITOR) editor_update();
    }

    gun_shutdown();
    render_shutdown();
    editor_save(state.editor->level);
    GL_END();
}

void RENDER()
{
    i32 fbw, fbh;
    glfwGetFramebufferSize(state.win, &fbw, &fbh);
    glfwGetWindowSize(state.win, &state.fb->ww, &state.fb->wh);

    const f32 aspect = (f32)fbw / (f32)fbh;
    const i32 rw = RENDER_BASE_W;
    const i32 rh = (i32)((f32)RENDER_BASE_W / aspect);

    render_main(rw, rh);
    post_blit(rw, rh, fbw, fbh);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glViewport(0, 0, fbw, fbh);
    glDisable(GL_DEPTH_TEST);

    imgui_newframe();

    text_begin();
    text_draw((vec2s){(f32)state.fb->ww * 0.5f - 5.0f, (f32)state.fb->wh * 0.5f - 10.0f}, "+");
    text_flush(state.fb->ww, state.fb->wh);

    if (state.id == STATE_EDITOR) editor_ui();

    imgui_render();

    glEnable(GL_DEPTH_TEST);
}

void INPUT()
{
    glfwSetInputMode(state.win, GLFW_CURSOR, state.cursor_locked ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
    imgui_set_mouse_enabled(!state.cursor_locked);

    bool shift_held = glfwGetKey(state.win, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || glfwGetKey(state.win, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS;
    bool no_ui = !imgui_want_capture_keyboard();

    { // ESC - Exit
        if (glfwGetKey(state.win, GLFW_KEY_ESCAPE) == GLFW_PRESS) state.id = STATE_EXIT;
    }

    { // H - Toggle debug bullet visualization
        static bool h_pressed = false;
        if (glfwGetKey(state.win, GLFW_KEY_H) == GLFW_PRESS) {
            if (!h_pressed) { state.debug_visible = !state.debug_visible; h_pressed = true; }
        } else h_pressed = false;
    }

    { // TAB - Toggle cursor lock
        static bool tab_pressed = false;
        if (glfwGetKey(state.win, GLFW_KEY_TAB) == GLFW_PRESS) {
            if (!tab_pressed) {
                state.cursor_locked = !state.cursor_locked;
                if (state.cursor_locked) {
                    glfwSetCursorPos(state.win, state.fb->ww * 0.5f, state.fb->wh * 0.5f);
                    state.cam->firstMouse = true;
                } tab_pressed = true;
            }
        } else tab_pressed = false;
    }

    { // Mouse - Shoot
        static bool shoot_pressed = false;
        if (state.id == STATE_PLAYING && state.cursor_locked) {
            if (glfwGetMouseButton(state.win, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
                if (!shoot_pressed) { shoot_bullet(); gun_shot(); shoot_pressed = true; }
            } else shoot_pressed = false;
        } else shoot_pressed = false;
    }

    { // G - Switch weapon
        static bool g_pressed = false;
        if (state.id == STATE_PLAYING) {
            if (glfwGetKey(state.win, GLFW_KEY_G) == GLFW_PRESS) {
                if (!g_pressed) { gun_next(); g_pressed = true; }
            } else g_pressed = false;
        }
    }

    { // E - Toggle editor/playing
        static bool e_pressed = false;
        if (glfwGetKey(state.win, GLFW_KEY_E) == GLFW_PRESS) {
            if (!e_pressed) {
                state.id = state.id == STATE_EDITOR ? STATE_PLAYING : STATE_EDITOR;
                e_pressed = true;
            }
        } else e_pressed = false;
    }

    { // N - New quad
        static bool n_pressed = false;
        if (no_ui && glfwGetKey(state.win, GLFW_KEY_N) == GLFW_PRESS) {
            if (!n_pressed) {
                i32 s_idx = state.editor->template_quad.sector_id;
                if (s_idx < 0 || s_idx >= state.editor->level->sector_count) s_idx = 0;
                editor_add_quad(&state.editor->level->sectors[s_idx], NULL);
                n_pressed = true;
            }
        } else n_pressed = false;
    }

    { // X - Delete quad
        static bool x_pressed = false;
        if (no_ui && (glfwGetKey(state.win, GLFW_KEY_X) == GLFW_PRESS || glfwGetKey(state.win, GLFW_KEY_DELETE) == GLFW_PRESS || glfwGetKey(state.win, GLFW_KEY_BACKSLASH) == GLFW_PRESS)) {
            if (!x_pressed && state.editor->selected_quad) {
                editor_delete_quad(state.editor->selected_sector, state.editor->selected_wall_idx);
                state.editor->selected_quad = NULL;
                x_pressed = true;
            }
        } else x_pressed = false;
    }

    { // R - Reset quad
        static bool r_pressed = false;
        if (no_ui && glfwGetKey(state.win, GLFW_KEY_R) == GLFW_PRESS) {
            if (!r_pressed && state.editor->selected_quad) {
                *state.editor->selected_quad = get_default_quad(state.cam);
                state.editor->selected_quad->sector_id = state.editor->selected_sector ? state.editor->selected_sector->id : 0;
                r_pressed = true;
            }
        } else r_pressed = false;
    }

    { // 7-9 - Adjust rotation
        static bool adj_pressed[9] = {0};
        static f32 adj_timer[9] = {0};
        static const int adj_keys[] = { GLFW_KEY_7, GLFW_KEY_8, GLFW_KEY_9 };
        for (int i = 0; i < 3; i++) {
            bool triggered = false;
            int key = adj_keys[i];
            if (no_ui && glfwGetKey(state.win, key) == GLFW_PRESS) {
                if (!adj_pressed[i]) { triggered = true; adj_pressed[i] = true; adj_timer[i] = 0.3f; }
                if (adj_pressed[i]) { adj_timer[i] -= state.dt; if (adj_timer[i] <= 0) { triggered = true; adj_timer[i] = 0.05f; } }
            }
            if (glfwGetKey(state.win, key) == GLFW_RELEASE) adj_pressed[i] = false;

            if (triggered) {
                level_quad_t* q = state.editor->selected_quad ? state.editor->selected_quad : &state.editor->template_quad;
                f32* val = (i == 0) ? &q->rot.x : (i == 1) ? &q->rot.y : &q->rot.z;
                f32 step = shift_held ? -1.0f : 1.0f;
                *val += step;
                if (*val >= 360.0f) *val = 0.0f;
                if (*val < 0.0f)    *val = 359.0f;
                *val = roundf(*val);

                if (state.editor->selected_quad) {
                    state.editor->template_quad = *state.editor->selected_quad;
                    state.editor->template_mods = EDITOR_MOD_ALL;
                } else {
                    state.editor->template_mods |= EDITOR_MOD_ROTATION;
                }
            }
        }
    }

    { // V - Toggle paint mode
        static bool v_pressed = false;
        if (no_ui && glfwGetKey(state.win, GLFW_KEY_V) == GLFW_PRESS) {
            if (!v_pressed) {
                if (state.editor->id == EDITOR_PAINT) state.editor->id = EDITOR_IDLE;
                else state.editor->id = EDITOR_PAINT;
                v_pressed = true;
            }
        } else v_pressed = false;
    }

    { // ENTER - Deselect
        static bool enter_pressed = false;
        if (no_ui && glfwGetKey(state.win, GLFW_KEY_ENTER) == GLFW_PRESS) {
            if (!enter_pressed) {
                state.editor->selected_quad = NULL;
                state.editor->template_quad = get_default_quad(state.cam);
                state.editor->template_mods = EDITOR_MOD_NONE;
                if (state.editor->id == EDITOR_PAINT) state.editor->id = EDITOR_IDLE;
                enter_pressed = true;
            }
        } else enter_pressed = false;
    }

    { // Movement
        const f32 speed = 18.5f * state.dt;
        const vec3s right = vec3_normalize(vec3_cross(state.cam->front, state.cam->up));

        if (state.id == STATE_PLAYING)
        {
            vec3s prev_pos = state.cam->pos;
            vec3s move = {0, 0, 0}, forward = {state.cam->front.x, 0, state.cam->front.z};
            if (vec3_magnitude(forward) > 0.0001f) forward = vec3_normalize(forward);

            if (glfwGetKey(state.win, GLFW_KEY_W) == GLFW_PRESS) move = vec3_add(move, forward);
            if (glfwGetKey(state.win, GLFW_KEY_S) == GLFW_PRESS) move = vec3_sub(move, forward);
            if (glfwGetKey(state.win, GLFW_KEY_A) == GLFW_PRESS) move = vec3_sub(move, right);
            if (glfwGetKey(state.win, GLFW_KEY_D) == GLFW_PRESS) move = vec3_add(move, right);

            if (vec3_magnitude(move) > 0.0001f) {
                move = vec3_normalize(move);
                state.cam->pos = vec3_add(state.cam->pos, vec3_scale(move, speed));
                portal_try_teleport(state.editor->level, prev_pos, state.cam);
                player_collide_quads(state.editor->level, state.cam);
            }

            f32 h;
            if (level_get_height(state.editor->level, state.cam->pos, &h)) state.cam->pos.y = lerp(state.cam->pos.y, h + 4.5f, 0.04f);
            else state.cam->pos = vec3_lerp(state.cam->pos, prev_pos, 0.04f);
        }

        if (state.id == STATE_EDITOR)
        {
            if (glfwGetKey(state.win, GLFW_KEY_W) == GLFW_PRESS) state.cam->pos = vec3_add(state.cam->pos, vec3_scale(state.cam->front, speed));
            if (glfwGetKey(state.win, GLFW_KEY_S) == GLFW_PRESS) state.cam->pos = vec3_sub(state.cam->pos, vec3_scale(state.cam->front, speed));
            if (glfwGetKey(state.win, GLFW_KEY_A) == GLFW_PRESS) state.cam->pos = vec3_sub(state.cam->pos, vec3_scale(right, speed));
            if (glfwGetKey(state.win, GLFW_KEY_D) == GLFW_PRESS) state.cam->pos = vec3_add(state.cam->pos, vec3_scale(right, speed));
        }
    }
}

ENGINE_ENTRY_POINT
