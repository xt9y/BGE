#include "runtime.h"
#include "editor.h"
#include "gun.h"
#include "imgui_c.h"
#include "state.h"

void engine_update_rotation_hotkeys(engine_runtime* runtime, bool no_ui, bool shift_held);
void engine_update_movement(void);

void engine_input(void)
{
    engine_runtime* in = bge_runtime;
    if (!in || !state.win || !state.editor || !state.cam) return;

    glfwSetInputMode(state.win, GLFW_CURSOR,
        state.cursor_locked ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
    imgui_set_mouse_enabled(!state.cursor_locked);

    const bool shift_held =
        glfwGetKey(state.win, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS ||
        glfwGetKey(state.win, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS;
    const bool no_ui = !imgui_want_capture_keyboard();

    if (glfwGetKey(state.win, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        state.id = STATE_EXIT;

    if (glfwGetKey(state.win, GLFW_KEY_H) == GLFW_PRESS) {
        if (!in->h_pressed) {
            state.debug_visible = !state.debug_visible;
            in->h_pressed = true;
        }
    } else in->h_pressed = false;

    if (glfwGetKey(state.win, GLFW_KEY_TAB) == GLFW_PRESS) {
        if (!in->tab_pressed) {
            state.cursor_locked = !state.cursor_locked;
            if (state.cursor_locked) {
                glfwSetCursorPos(state.win,
                    (f64)state.fb->ww * 0.5,
                    (f64)state.fb->wh * 0.5);
                state.cam->firstMouse = true;
            }
            in->tab_pressed = true;
        }
    } else in->tab_pressed = false;

    if (state.id == STATE_PLAYING && state.cursor_locked) {
        if (glfwGetMouseButton(state.win, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
            if (!in->shoot_pressed) {
                shoot_bullet();
                gun_shot();
                in->shoot_pressed = true;
            }
        } else in->shoot_pressed = false;
    } else in->shoot_pressed = false;

    if (state.id == STATE_PLAYING && glfwGetKey(state.win, GLFW_KEY_G) == GLFW_PRESS) {
        if (!in->g_pressed) {
            gun_next();
            in->g_pressed = true;
        }
    } else in->g_pressed = false;

    if (glfwGetKey(state.win, GLFW_KEY_E) == GLFW_PRESS) {
        if (!in->e_pressed) {
            state.id = state.id == STATE_EDITOR ? STATE_PLAYING : STATE_EDITOR;
            in->e_pressed = true;
        }
    } else in->e_pressed = false;

    if (no_ui && glfwGetKey(state.win, GLFW_KEY_N) == GLFW_PRESS) {
        if (!in->n_pressed && state.editor->level->sector_count > 0) {
            i32 sector_index = state.editor->template_quad.sector_id;
            if (sector_index < 0 || sector_index >= state.editor->level->sector_count)
                sector_index = 0;
            editor_add_quad(&state.editor->level->sectors[sector_index], NULL);
            in->n_pressed = true;
        }
    } else in->n_pressed = false;

    if (no_ui &&
        (glfwGetKey(state.win, GLFW_KEY_X) == GLFW_PRESS ||
         glfwGetKey(state.win, GLFW_KEY_DELETE) == GLFW_PRESS ||
         glfwGetKey(state.win, GLFW_KEY_BACKSLASH) == GLFW_PRESS)) {
        if (!in->x_pressed && state.editor->selected_quad) {
            editor_delete_quad(state.editor->selected_sector, state.editor->selected_wall_idx);
            state.editor->selected_quad = NULL;
            in->x_pressed = true;
        }
    } else in->x_pressed = false;

    if (no_ui && glfwGetKey(state.win, GLFW_KEY_R) == GLFW_PRESS) {
        if (!in->r_pressed && state.editor->selected_quad) {
            *state.editor->selected_quad = get_default_quad(state.cam);
            state.editor->selected_quad->sector_id =
                state.editor->selected_sector ? state.editor->selected_sector->id : 0;
            in->r_pressed = true;
        }
    } else in->r_pressed = false;

    engine_update_rotation_hotkeys(in, no_ui, shift_held);

    if (no_ui && glfwGetKey(state.win, GLFW_KEY_V) == GLFW_PRESS) {
        if (!in->v_pressed) {
            state.editor->id = state.editor->id == EDITOR_PAINT ? EDITOR_IDLE : EDITOR_PAINT;
            in->v_pressed = true;
        }
    } else in->v_pressed = false;

    if (no_ui && glfwGetKey(state.win, GLFW_KEY_ENTER) == GLFW_PRESS) {
        if (!in->enter_pressed) {
            state.editor->selected_quad = NULL;
            state.editor->template_quad = get_default_quad(state.cam);
            state.editor->template_mods = EDITOR_MOD_NONE;
            if (state.editor->id == EDITOR_PAINT)
                state.editor->id = EDITOR_IDLE;
            in->enter_pressed = true;
        }
    } else in->enter_pressed = false;

    engine_update_movement();
}
