#include "Engine/level.h"
#include "Engine/editor.h"
#include "Engine/App.h"
#include "Engine/portal.h"

#include "Engine/res/level1.h"
#include "Engine/res/level2.h"
#include "Engine/res/level3.h"
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

    text_begin();
    text_draw((vec2s){10.0f, 10.0f}, "FPS %.1f", GL_GETFPS());
    text_draw((vec2s){(f32)state.fb->ww * 0.5f - 5.0f, (f32)state.fb->wh * 0.5f - 10.0f}, "+");
    if (state.id == STATE_EDITOR) editor_render_info();
    text_flush(state.fb->ww, state.fb->wh);

    glEnable(GL_DEPTH_TEST);
}

void INPUT()
{
    glfwSetInputMode(state.win, GLFW_CURSOR, state.cursor_locked ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);

    bool shift_held = glfwGetKey(state.win, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || glfwGetKey(state.win, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS;
    bool ctrl_held = glfwGetKey(state.win, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS || glfwGetKey(state.win, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS;

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

    { // B - Next level (Shift+B - toggle billboard)
        static bool b_pressed = false;
        if (glfwGetKey(state.win, GLFW_KEY_B) == GLFW_PRESS) {
            if (!b_pressed) {
                if (shift_held && state.editor->selected_quad) {
                    state.editor->selected_quad->is_billboard = !state.editor->selected_quad->is_billboard;
                } else {
                    editor_save(state.editor->level);
                    state.level_id = (state.level_id + 1) % state.level_count;
                    apply_level_camera(state.cam, &state.levels[state.level_id]);
                    state.cam->firstMouse = true;
                    state.editor->selected_quad = NULL;
                    state.editor->selected_sector = NULL;
                    state.editor->template_quad = get_default_quad(state.cam);
                    state.editor->template_mods = EDITOR_MOD_NONE;
                } b_pressed = true;
            }
        } else b_pressed = false;
    }

    { // I - Toggle solid (Shift+I - toggle invisible)
        static bool i_pressed = false;
        if (glfwGetKey(state.win, GLFW_KEY_I) == GLFW_PRESS) {
            if (!i_pressed && state.editor->selected_quad) {
                if (shift_held) {
                    state.editor->selected_quad->is_invisible = !state.editor->selected_quad->is_invisible;
                } else {
                    state.editor->selected_quad->is_solid = !state.editor->selected_quad->is_solid;
                } i_pressed = true;
            }
        } else i_pressed = false;
    }

    { // N - New quad
        static bool n_pressed = false;
        if (glfwGetKey(state.win, GLFW_KEY_N) == GLFW_PRESS) {
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
        if (glfwGetKey(state.win, GLFW_KEY_X) == GLFW_PRESS || glfwGetKey(state.win, GLFW_KEY_DELETE) == GLFW_PRESS) {
            if (!x_pressed && state.editor->selected_quad) {
                editor_delete_quad(state.editor->selected_sector, state.editor->selected_wall_idx);
                state.editor->selected_quad = NULL;
                x_pressed = true;
            }
        } else x_pressed = false;
    }

    { // R - Reset quad
        static bool r_pressed = false;
        if (glfwGetKey(state.win, GLFW_KEY_R) == GLFW_PRESS) {
            if (!r_pressed && state.editor->selected_quad) {
                *state.editor->selected_quad = get_default_quad(state.cam);
                r_pressed = true;
            }
        } else r_pressed = false;
    }

    { // 0 - Cycle texture
        static f32 tex_timer = 0;
        if (glfwGetKey(state.win, GLFW_KEY_0) == GLFW_PRESS) {
            if (tex_timer <= 0) {
                level_quad_t* q = state.editor->selected_quad ? state.editor->selected_quad : &state.editor->template_quad;
                q->tex_id++;
                if (q->tex_id >= state.text->count) q->tex_id = -1;
                if (state.editor->selected_quad) {
                    state.editor->template_quad = *state.editor->selected_quad;
                    state.editor->template_mods = EDITOR_MOD_ALL;
                } else state.editor->template_mods |= EDITOR_MOD_TEXTURE;
                tex_timer = 0.15f;
            } tex_timer -= state.dt;
        } else tex_timer = 0;
    }

    { // 1-9 - Adjust color/light/rotation
        static bool adj_pressed[9] = {0};
        static f32 adj_timer[9] = {0};
        static const int adj_keys[] = { GLFW_KEY_1, GLFW_KEY_2, GLFW_KEY_3, GLFW_KEY_4, GLFW_KEY_5, GLFW_KEY_6, GLFW_KEY_7, GLFW_KEY_8, GLFW_KEY_9 };
        for (int i = 0; i < 9; i++) {
            bool triggered = false;
            if (glfwGetKey(state.win, adj_keys[i]) == GLFW_PRESS) {
                if (!adj_pressed[i]) { triggered = true; adj_pressed[i] = true; adj_timer[i] = 0.3f; }
                if (adj_pressed[i] && i >= 6) { adj_timer[i] -= state.dt; if (adj_timer[i] <= 0) { triggered = true; adj_timer[i] = 0.05f; } }
            }
            if (glfwGetKey(state.win, adj_keys[i]) == GLFW_RELEASE) adj_pressed[i] = false;

            if (triggered) {
                f32* val = NULL;
                level_quad_t* q = state.editor->selected_quad ? state.editor->selected_quad : &state.editor->template_quad;
                level_sector_data_t* s = state.editor->selected_sector;

                if (i == 0) val = &q->color.x;
                if (i == 1) val = &q->color.y;
                if (i == 2) val = &q->color.z;
                if (s && i == 3) val = &s->light.x;
                if (s && i == 4) val = &s->light.y;
                if (s && i == 5) val = &s->light.z;
                if (i == 6) val = &q->rot.x;
                if (i == 7) val = &q->rot.y;
                if (i == 8) val = &q->rot.z;

                if (val) {
                    f32 step = (i < 6) ? 0.1f : 1.0f;
                    if (i >= 6 && shift_held) step = -step;
                    *val += step;
                    if (i < 6 && *val > 1.05f)  *val = 0.0f;
                    if (i < 6 && *val < -0.05f) *val = 1.0f;
                    if (i < 6) *val = roundf(*val * 10.0f) / 10.0f;
                    if (i >= 6 && *val >= 360.0f) *val = 0.0f;
                    if (i >= 6 && *val < 0.0f)    *val = 359.0f;
                    if (i >= 6) *val = roundf(*val);

                    if (state.editor->selected_quad) {
                        state.editor->template_quad = *state.editor->selected_quad;
                        state.editor->template_mods = EDITOR_MOD_ALL;
                    } else {
                        if (i < 3) state.editor->template_mods |= EDITOR_MOD_COLOR;
                        if (i >= 6) state.editor->template_mods |= EDITOR_MOD_ROTATION;
                    }
                }
            }
        }
    }

    { // V - Toggle paint mode
        static bool v_pressed = false;
        if (glfwGetKey(state.win, GLFW_KEY_V) == GLFW_PRESS) {
            if (!v_pressed) {
                if (state.editor->id == EDITOR_PAINT) state.editor->id = EDITOR_IDLE;
                else state.editor->id = EDITOR_PAINT;
                v_pressed = true;
            }
        } else v_pressed = false;
    }

    { // Q - Cycle sectors
        static bool q_pressed = false;
        if (glfwGetKey(state.win, GLFW_KEY_Q) == GLFW_PRESS) {
            if (!q_pressed) {
                i32 current_idx = 0;
                for (i32 i = 0; i < state.editor->level->sector_count; i++)
                    if (state.editor->level->sectors[i].id == state.editor->template_quad.sector_id) { current_idx = i; break; }

                i32 target_idx;
                if (shift_held) target_idx = (current_idx - 1 + state.editor->level->sector_count) % state.editor->level->sector_count;
                else {
                    if (current_idx == state.editor->level->sector_count - 1) editor_add_sector(state.editor->level);
                    target_idx = (current_idx + 1) % state.editor->level->sector_count;
                }

                if (state.editor->selected_quad) editor_move_quad_to_sector(state.editor->selected_sector, &state.editor->level->sectors[target_idx], state.editor->selected_wall_idx);

                state.editor->template_quad.sector_id = state.editor->level->sectors[target_idx].id;
                state.editor->template_mods |= EDITOR_MOD_SECTOR;
                q_pressed = true;
            }
        } else q_pressed = false;
    }

    { // P - Adjust portal
        static bool p_pressed = false;
        if (glfwGetKey(state.win, GLFW_KEY_P) == GLFW_PRESS && state.editor->id != EDITOR_PAINT) {
            if (!p_pressed) {
                level_quad_t* q = state.editor->selected_quad ? state.editor->selected_quad : &state.editor->template_quad;
                if (ctrl_held) q->portal_side_flip = !q->portal_side_flip;
                else if (shift_held)
                    do { q->portal_id--; if (q->portal_id < 0) { q->portal_id = 0; break; }
                    } while (q->portal_id > 0 && count_portal_quads(state.editor->level, q->portal_id) >= 2);
                else {
                    u8 limit = 256; do { q->portal_id++;
                    } while (count_portal_quads(state.editor->level, q->portal_id) > 2 && --limit > 0);
                }
                if (state.editor->selected_quad) {
                    state.editor->template_quad = *state.editor->selected_quad;
                    state.editor->template_mods = EDITOR_MOD_ALL;
                }
                p_pressed = true;
            }
        } else p_pressed = false;
    }

    { // ENTER - Deselect
        static bool enter_pressed = false;
        if (glfwGetKey(state.win, GLFW_KEY_ENTER) == GLFW_PRESS) {
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
