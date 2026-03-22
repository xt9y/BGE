#include "cam.h"
#include "state.h"
#include "util/math.h"
#include <math.h>

void update_camera_vectors();

void camera_init()
{
    state.cam_pos = (vec3s){0.0f, 0.0f, 3.0f};
    state.cam_front = (vec3s){0.0f, 0.0f, -1.0f};
    state.cam_up = (vec3s){0.0f, 1.0f, 0.0f};
    state.yaw   = -90.0f;
    state.pitch = 0.0f;
    state.lastX = 800 / 2.0f;
    state.lastY = 600 / 2.0f;
    state.firstMouse = true;
    update_camera_vectors();
}

void update_camera_vectors()
{
    vec3s front;
    front.x = cosf(DEG2RAD(state.yaw)) * cosf(DEG2RAD(state.pitch));
    front.y = sinf(DEG2RAD(state.pitch));
    front.z = sinf(DEG2RAD(state.yaw)) * cosf(DEG2RAD(state.pitch));
    state.cam_front = vec3_normalize(front);
}

void camera_update()
{
    const f32 speed = 2.5f * state.dt;
    const vec3s right = vec3_normalize(vec3_cross(state.cam_front, state.cam_up));
    
    if (glfwGetKey(state.win, GLFW_KEY_W) == GLFW_PRESS)
        state.cam_pos = vec3_add(state.cam_pos, vec3_scale(state.cam_front, speed));
    if (glfwGetKey(state.win, GLFW_KEY_S) == GLFW_PRESS)
        state.cam_pos = vec3_sub(state.cam_pos, vec3_scale(state.cam_front, speed));
    if (glfwGetKey(state.win, GLFW_KEY_A) == GLFW_PRESS)
        state.cam_pos = vec3_sub(state.cam_pos, vec3_scale(right, speed));
    if (glfwGetKey(state.win, GLFW_KEY_D) == GLFW_PRESS)
        state.cam_pos = vec3_add(state.cam_pos, vec3_scale(right, speed));
}

void camera_mouse_callback(const f64 xpos, const f64 ypos)
{
    if (state.firstMouse) {
        state.lastX = (f32)xpos;
        state.lastY = (f32)ypos;
        state.firstMouse = false;
    }

    const f32 xoffset = ((f32)xpos - state.lastX) * 0.1f;
    const f32 yoffset = (state.lastY - (f32)ypos) * 0.1f;
    state.lastX = (f32)xpos;
    state.lastY = (f32)ypos;

    state.yaw   += xoffset;
    state.pitch += yoffset;

    if (state.pitch > 89.0f) state.pitch = 89.0f;
    if (state.pitch < -89.0f) state.pitch = -89.0f;

    update_camera_vectors();
}
