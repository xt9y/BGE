#include "runtime.h"
#include "editor.h"
#include "state.h"
#include <math.h>

void engine_update_rotation_hotkeys(engine_runtime* in, bool no_ui, bool shift_held)
{
    static const int keys[3] = { GLFW_KEY_7, GLFW_KEY_8, GLFW_KEY_9 };
    for (int i = 0; i < 3; ++i) {
        bool triggered = false;
        if (no_ui && glfwGetKey(state.win, keys[i]) == GLFW_PRESS) {
            if (!in->adj_pressed[i]) {
                triggered = true;
                in->adj_pressed[i] = true;
                in->adj_timer[i] = 0.3f;
            } else {
                in->adj_timer[i] -= state.dt;
                if (in->adj_timer[i] <= 0.0f) {
                    triggered = true;
                    in->adj_timer[i] = 0.05f;
                }
            }
        } else {
            in->adj_pressed[i] = false;
        }
        if (!triggered) continue;

        level_quad_t* q = state.editor->selected_quad
            ? state.editor->selected_quad
            : &state.editor->template_quad;
        f32* value = i == 0 ? &q->rot.x : (i == 1 ? &q->rot.y : &q->rot.z);
        *value += shift_held ? -1.0f : 1.0f;
        if (*value >= 360.0f) *value = 0.0f;
        if (*value < 0.0f) *value = 359.0f;
        *value = roundf(*value);

        if (state.editor->selected_quad) {
            state.editor->template_quad = *state.editor->selected_quad;
            state.editor->template_mods = EDITOR_MOD_ALL;
        } else {
            state.editor->template_mods |= EDITOR_MOD_ROTATION;
        }
    }
}
