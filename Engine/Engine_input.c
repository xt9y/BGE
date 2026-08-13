#include "runtime.h"

#include "editor.h"
#include "imgui_c.h"
#include "state.h"

static bool key_down(const int key)
{
    return state.win && glfwGetKey(state.win, key) == GLFW_PRESS;
}

static void update_rotation_hotkeys(engine_runtime* runtime, const bool no_ui, const bool shift)
{
    static const int keys[3] = { GLFW_KEY_7, GLFW_KEY_8, GLFW_KEY_9 };
    for (i32 axis = 0; axis < 3; ++axis) {
        bool trigger = false;
        if (no_ui && state.id == STATE_EDITOR && key_down(keys[axis])) {
            if (!runtime->adj_pressed[axis]) {
                runtime->adj_pressed[axis] = true;
                runtime->adj_timer[axis] = 0.30f;
                trigger = true;
            } else {
                runtime->adj_timer[axis] -= state.dt;
                if (runtime->adj_timer[axis] <= 0.0f) {
                    runtime->adj_timer[axis] = 0.05f;
                    trigger = true;
                }
            }
        } else {
            runtime->adj_pressed[axis] = false;
            runtime->adj_timer[axis] = 0.0f;
        }

        if (trigger)
            editor_rotate_selected_or_template(axis, shift ? -1.0f : 1.0f);
    }
}

static void update_editor_camera(void)
{
    if (state.id != STATE_EDITOR || !state.cam || !state.win) return;

    const f32 speed = 18.5f * state.dt;
    const vec3s right = vec3_normalize(vec3_cross(state.cam->front, state.cam->up));
    if (key_down(GLFW_KEY_W)) state.cam->pos = vec3_add(state.cam->pos, vec3_scale(state.cam->front, speed));
    if (key_down(GLFW_KEY_S)) state.cam->pos = vec3_sub(state.cam->pos, vec3_scale(state.cam->front, speed));
    if (key_down(GLFW_KEY_A)) state.cam->pos = vec3_sub(state.cam->pos, vec3_scale(right, speed));
    if (key_down(GLFW_KEY_D)) state.cam->pos = vec3_add(state.cam->pos, vec3_scale(right, speed));
}

void engine_input(void)
{
    engine_runtime* in = bge_runtime;
    if (!in || !g_state || !state.win || !state.editor || !state.cam || !state.fb) return;

    glfwSetInputMode(state.win, GLFW_CURSOR,
        state.cursor_locked ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
    imgui_set_mouse_enabled(!state.cursor_locked);

    const bool no_ui = !imgui_want_capture_keyboard();
    const bool shift = key_down(GLFW_KEY_LEFT_SHIFT) || key_down(GLFW_KEY_RIGHT_SHIFT);

    if (key_down(GLFW_KEY_ESCAPE)) state.id = STATE_EXIT;

    if (key_down(GLFW_KEY_H)) {
        if (!in->h_pressed) {
            state.debug_visible = !state.debug_visible;
            in->h_pressed = true;
        }
    } else in->h_pressed = false;

    if (key_down(GLFW_KEY_TAB)) {
        if (!in->tab_pressed) {
            state.cursor_locked = !state.cursor_locked;
            if (state.cursor_locked) {
                glfwSetCursorPos(state.win, (f64)state.fb->ww * 0.5, (f64)state.fb->wh * 0.5);
                state.cam->firstMouse = true;
            }
            in->tab_pressed = true;
        }
    } else in->tab_pressed = false;

    if (no_ui && key_down(GLFW_KEY_E)) {
        if (!in->e_pressed) {
            state.id = state.id == STATE_EDITOR ? STATE_PLAYING : STATE_EDITOR;
            if (state.id != STATE_EDITOR) editor_clear_selection();
            in->e_pressed = true;
        }
    } else in->e_pressed = false;

    if (state.id == STATE_EDITOR && no_ui && key_down(GLFW_KEY_N)) {
        if (!in->n_pressed) {
            (void)editor_add_template_quad();
            in->n_pressed = true;
        }
    } else in->n_pressed = false;

    if (state.id == STATE_EDITOR && no_ui &&
        (key_down(GLFW_KEY_X) || key_down(GLFW_KEY_DELETE) || key_down(GLFW_KEY_BACKSLASH))) {
        if (!in->x_pressed) {
            (void)editor_delete_selected_quad();
            in->x_pressed = true;
        }
    } else in->x_pressed = false;

    if (state.id == STATE_EDITOR && no_ui && key_down(GLFW_KEY_R)) {
        if (!in->r_pressed) {
            editor_reset_selected_quad();
            in->r_pressed = true;
        }
    } else in->r_pressed = false;

    update_rotation_hotkeys(in, no_ui, shift);

    if (state.id == STATE_EDITOR && no_ui && key_down(GLFW_KEY_V)) {
        if (!in->v_pressed) {
            editor_toggle_paint_mode();
            in->v_pressed = true;
        }
    } else in->v_pressed = false;

    if (state.id == STATE_EDITOR && no_ui && key_down(GLFW_KEY_ENTER)) {
        if (!in->enter_pressed) {
            if (state.editor->id == EDITOR_PAINT) editor_toggle_paint_mode();
            editor_clear_selection();
            in->enter_pressed = true;
        }
    } else in->enter_pressed = false;

    update_editor_camera();
}
