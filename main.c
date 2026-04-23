#include "Engine/level.h"
#include "Engine/editor.h"
#include "Engine/App.h"
#include "Engine/portal.h"

#include "Engine/res/level1.h"
#include "Engine/res/level2.h"
#include "Engine/res/level3.h"

/*
 * TL:DR
 * > start with 'make'
 *
 * > press b to switch between all levels
 *
 * > press tab to toggle between mouse locking
 *
 * > press e to switch betwen editor mode ... :
 *  > at the bottom of the screen is a small legend which button does what 
 *
 *  > click to select / start drag
 *  > ctrl + click near edge to resize
 *  > enter to deselect
 *
 *  > press n to create a new default quad
 *  > press n while a quad is selected duplicates that quad
 *
 *  > press x to delete the quad
 *  > press r to reset the  quad
 *
 *  > press i to toggle solid
 *  > press shift + i to toggle invisible
 *  > press shift + b to toggle billboard
 *
 *  > press p to increase portal id
 *  > press shift + p to decrease portal id
 *  > press ctrl + p to flip the shown portal side
 *
 *  > press q to next sector
 *  > press shift + q to previous sector
 *
 *  > press 0 to cycle texture id
 *
 *  > press 1 2 3 to change quad rgb
 *  > press 4 5 6 to change sector light rgb
 *  > press 7 8 9 to change quad rotation xyz
 *  > press shift + 7 8 9 to reverse rotation xyz
 *
 *  > press v to toggle paint mode
 *
 * > ... and play mode:
 *  > press w a s d to walk around
 *  > use mouse to look around
 */

void apply_level_camera(camera_t *cam, level_data_t *level) {
    cam->pos = level->cam.pos;
    cam->yaw = level->cam.yaw;
    cam->pitch = level->cam.pitch;
    cam->firstMouse = true;
    update_camera_vectors(cam);
}

static void set_camera_uniforms(const camera_t* cam)
{
    f32 view[16], proj[16];
    mat4_lookat(view, cam->pos, vec3_add(cam->pos, cam->front), cam->up);
    mat4_perspective(proj, DEG2RAD(45.0f), (f32)state.fb->w / (f32)state.fb->h, 0.1f, 100.0f);

    glUniformMatrix4fv(glGetUniformLocation(state.data->program, "view"), 1, GL_FALSE, view);
    glUniformMatrix4fv(glGetUniformLocation(state.data->program, "projection"), 1, GL_FALSE, proj);
}

static void render_portals(const level_data_t* level, const camera_t* cam)
{
    for (i32 s = 0; s < level->sector_count; s++) 
    {
        const level_sector_data_t* sector = &level->sectors[s];
        for (i32 q = 0; q < sector->quad_count; q++) 
        {
            const level_quad_t* quad = &sector->quads[q];
            portal_link_t link;
            camera_t portal_cam;

            if (!portal_find_link(level, quad, &link)) continue;
            if (link.src != quad) continue;
            if (!portal_build_camera(link.src, link.dst, cam, &portal_cam)) continue;

            glClear(GL_STENCIL_BUFFER_BIT);

            glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
            glDepthMask(GL_FALSE);
            glDisable(GL_DEPTH_TEST);
            glDisable(GL_CULL_FACE);
            glStencilMask(0xFF);
            glStencilFunc(GL_ALWAYS, 1, 0xFF);
            glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

            level_render_quad(link.src, (vec4s){1.0f, 1.0f, 1.0f, 1.0f});

            glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
            glDepthMask(GL_TRUE);
            glEnable(GL_DEPTH_TEST);
            glStencilMask(0x00);
            glStencilFunc(GL_EQUAL, 1, 0xFF);
            glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

            set_camera_uniforms(&portal_cam);
            level_render(level, &portal_cam);

            glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
            glDepthMask(GL_TRUE);
            glEnable(GL_DEPTH_TEST);
            glDepthFunc(GL_ALWAYS);
            glStencilMask(0x00);
            glStencilFunc(GL_EQUAL, 1, 0xFF);
            level_render_quad(link.src, (vec4s){1.0f, 1.0f, 1.0f, 1.0f});
            glDepthFunc(GL_LESS);
            glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

            set_camera_uniforms(cam);
            glStencilMask(0xFF);
            glStencilFunc(GL_ALWAYS, 0, 0xFF);
            glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
        }
    }
}

void RUN()
{
    GL_START();

    {   // Textures
        texture_registry_init(state.text);
        state.text->textures[state.text->count++] = *texture_create("Engine/res/ground.png", TEX_FILTER_LINEAR, TEX_WRAP_REPEAT);
        state.text->textures[state.text->count++] = *texture_create("Engine/res/stone.png", TEX_FILTER_LINEAR, TEX_WRAP_REPEAT);
        state.text->textures[state.text->count++] = *texture_create("Engine/res/awesomeface.png", TEX_FILTER_LINEAR, TEX_WRAP_REPEAT);
        state.text->textures[state.text->count++] = *texture_create_solid(255, 255, 255);
        text_init();
    }

    {   // Levels 
        state.level_count, state.level_id = 0;
        state.levels[state.level_count++] = load_1();
        state.levels[state.level_count++] = load_2();
        state.levels[state.level_count++] = load_3();
    }

    {   // Editor
        state.editor->level = &state.levels[state.level_id];
    }

    {   // Camera
        state.cam->front = (vec3s){0.0f, 0.0f, -1.0f};
        state.cam->up = (vec3s){0.0f, 1.0f, 0.0f};
        state.cam->lastX = (f32)state.fb->w * 0.5f;
        state.cam->lastY = (f32)state.fb->h * 0.5f;
        state.cursor_locked = false;
        apply_level_camera(state.cam, &state.levels[state.level_id]);
    }

    while (GL_FRAME()) 
    {
        state.editor->level = &state.levels[state.level_id];
        if (state.id == STATE_EDITOR) editor_update();
    }

    editor_save(state.editor->level);
    GL_END();
}

void RENDER()
{
    state.fb->w = 0, state.fb->h = 0;
    glfwGetFramebufferSize(state.win, &state.fb->w, &state.fb->h);
    glViewport(0, 0, state.fb->w, state.fb->h);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f); 
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    glUseProgram(state.data->program);
    
    f32 model[16];
    mat4_identity(model);

    glUniformMatrix4fv(glGetUniformLocation(state.data->program, "model"), 1, GL_FALSE, model);
    set_camera_uniforms(state.cam);

    text_begin();
    render_portals(state.editor->level, state.cam);
    level_render(state.editor->level, state.cam);
    if (state.id == STATE_EDITOR) editor_render();

    text_draw((vec2s){(f32)state.fb->w * 0.5f - 5.0f, (f32)state.fb->h * 0.5f - 10.0f}, "+");
    text_draw((vec2s){10.0f, 10.0f}, "()*+-./ :;<=>? 0123456789 ABCDEFGHIJKLMNOPQRSTUVWXYZ _ abcdefghijklmnopqrstuvwxyz ");
    int ww, wh; glfwGetWindowSize(state.win, &ww, &wh);
    text_draw((vec2s){10.0f, 30.0f}, "FPS %.1f, WIN: %d x %d (FB: %d x %d)", GL_GETFPS(), ww, wh, state.fb->w, state.fb->h);
    text_draw((vec2s){10.0f, 50.0f}, "POS: %.1f %.1f %.1f ; YAW %.1f ; PITCH %.1f", state.cam->pos.x, state.cam->pos.y, state.cam->pos.z, state.cam->yaw, state.cam->pitch);
    text_draw((vec2s){10.0f, 70.0f}, "CURRENT_LVL: %d ; MAX_LVLS: %d", state.level_id + 1, state.level_count);
    if (state.id != STATE_EDITOR) {
        const char* game_modes[] = { "MENU", "PLAYING", "EDITOR", "EXIT" };
        text_draw((vec2s){10.0f, 90.0f}, "STATE: STATE_%s", game_modes[state.id]);
    }
    if (state.id == STATE_EDITOR) {
        const char* editor_modes[] = { "IDLE", "DRAG", "RESIZE_TOP", "RESIZE_RIGHT", "PAINT" };
        text_draw((vec2s){10.0f, 90.0f}, "EDITOR: EDITOR_%s", editor_modes[state.editor->id]);
    }
    text_flush(state.fb->w, state.fb->h);
}

void INPUT()
{
    glfwSetInputMode(state.win, GLFW_CURSOR, state.cursor_locked ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);

    bool shift_held = glfwGetKey(state.win, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || glfwGetKey(state.win, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS;
    bool ctrl_held = glfwGetKey(state.win, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS || glfwGetKey(state.win, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS;

    { // ESC - Exit
        if (glfwGetKey(state.win, GLFW_KEY_ESCAPE) == GLFW_PRESS) state.id = STATE_EXIT;
    }

    { // TAB - Toggle cursor lock
        static bool tab_pressed = false;
        if (glfwGetKey(state.win, GLFW_KEY_TAB) == GLFW_PRESS) {
            if (!tab_pressed) {
                state.cursor_locked = !state.cursor_locked;
                if (state.cursor_locked) {
                    glfwSetCursorPos(state.win, state.fb->w * 0.5f, state.fb->h * 0.5f);
                    state.cam->firstMouse = true;
                } tab_pressed = true;
            }
        } else tab_pressed = false;
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
                if (ctrl_held) {
                    q->portal_side_flip = !q->portal_side_flip;
                } else if (shift_held) {
                    do { q->portal_id--; if (q->portal_id < 0) { q->portal_id = 0; break; }
                    } while (q->portal_id > 0 && count_portal_quads(state.editor->level, q->portal_id) >= 2);
                } else {
                    int limit = 256; do q->portal_id++;
                    while (count_portal_quads(state.editor->level, q->portal_id) > 2 && --limit > 0);
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
            vec3s move = {0, 0, 0}, forward = {state.cam->front.x, 0, state.cam->front.z};
            if (vec3_magnitude(forward) > 0.0001f) forward = vec3_normalize(forward);

            if (glfwGetKey(state.win, GLFW_KEY_W) == GLFW_PRESS) move = vec3_add(move, forward);
            if (glfwGetKey(state.win, GLFW_KEY_S) == GLFW_PRESS) move = vec3_sub(move, forward);
            if (glfwGetKey(state.win, GLFW_KEY_A) == GLFW_PRESS) move = vec3_sub(move, right);
            if (glfwGetKey(state.win, GLFW_KEY_D) == GLFW_PRESS) move = vec3_add(move, right);

            if (vec3_magnitude(move) > 0.0001f) 
            {
                move = vec3_normalize(move);
                state.cam->pos = vec3_add(state.cam->pos, vec3_scale(move, speed));
            }

            f32 h;
            if (level_get_height(state.editor->level, state.cam->pos, &h)) state.cam->pos.y = h + 4.5f;
            else state.id = STATE_EDITOR;
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
