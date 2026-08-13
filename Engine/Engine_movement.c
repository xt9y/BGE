#include "runtime.h"
#include "portal.h"
#include "render.h"
#include "state.h"

void engine_update_movement(void)
{
    const f32 speed = 18.5f * state.dt;
    const vec3s right = vec3_normalize(vec3_cross(state.cam->front, state.cam->up));

    if (state.id == STATE_PLAYING) {
        vec3s previous = state.cam->pos;
        vec3s move = {0.0f, 0.0f, 0.0f};
        vec3s forward = {state.cam->front.x, 0.0f, state.cam->front.z};

        if (vec3_magnitude(forward) > 0.0001f)
            forward = vec3_normalize(forward);

        if (glfwGetKey(state.win, GLFW_KEY_W) == GLFW_PRESS) move = vec3_add(move, forward);
        if (glfwGetKey(state.win, GLFW_KEY_S) == GLFW_PRESS) move = vec3_sub(move, forward);
        if (glfwGetKey(state.win, GLFW_KEY_A) == GLFW_PRESS) move = vec3_sub(move, right);
        if (glfwGetKey(state.win, GLFW_KEY_D) == GLFW_PRESS) move = vec3_add(move, right);

        if (vec3_magnitude(move) > 0.0001f) {
            move = vec3_normalize(move);
            state.cam->pos = vec3_add(state.cam->pos, vec3_scale(move, speed));
            portal_try_teleport(state.editor->level, previous, state.cam);
            player_collide_quads(state.editor->level, state.cam);
        }

        f32 height = 0.0f;
        if (level_get_height(state.editor->level, state.cam->pos, &height))
            state.cam->pos.y = lerp(state.cam->pos.y, height + 4.5f, 0.04f);
        else
            state.cam->pos = vec3_lerp(state.cam->pos, previous, 0.04f);
    } else if (state.id == STATE_EDITOR) {
        if (glfwGetKey(state.win, GLFW_KEY_W) == GLFW_PRESS)
            state.cam->pos = vec3_add(state.cam->pos, vec3_scale(state.cam->front, speed));
        if (glfwGetKey(state.win, GLFW_KEY_S) == GLFW_PRESS)
            state.cam->pos = vec3_sub(state.cam->pos, vec3_scale(state.cam->front, speed));
        if (glfwGetKey(state.win, GLFW_KEY_A) == GLFW_PRESS)
            state.cam->pos = vec3_sub(state.cam->pos, vec3_scale(right, speed));
        if (glfwGetKey(state.win, GLFW_KEY_D) == GLFW_PRESS)
            state.cam->pos = vec3_add(state.cam->pos, vec3_scale(right, speed));
    }
}
