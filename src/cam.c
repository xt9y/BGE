#include "cam.h"
#include "state.h"
#include "util/math.h"
#include <math.h>

void update_camera_vectors();

void init_camera()
{
    state.cam_pos = (vec3s){0.0f, 0.0f, 3.0f};
    state.cam_front = (vec3s){0.0f, 0.0f, -1.0f};
    state.cam_up = (vec3s){0.0f, 1.0f, 0.0f};
    state.yaw = -90.0f;
    state.pitch = 0.0f;
    state.lastX = 800 / 2.0f;
    state.lastY = 600 / 2.0f;
    state.firstMouse = true;
    update_camera_vectors();
}

void update_camera_vectors()
{
    vec3s front;
    front.x = cosf(state.yaw * 3.1415926535f / 180.0f) * cosf(state.pitch * 3.1415926535f / 180.0f);
    front.y = sinf(state.pitch * 3.1415926535f / 180.0f);
    front.z = sinf(state.yaw * 3.1415926535f / 180.0f) * cosf(state.pitch * 3.1415926535f / 180.0f);
    state.cam_front = vec3_normalize(front);
}

void update_camera()
{
    f32 speed = 2.5f * state.dt;
    if (glfwGetKey(state.win, GLFW_KEY_W) == GLFW_PRESS) state.cam_pos = vec3_add(state.cam_pos, vec3_scale(state.cam_front, speed));
    if (glfwGetKey(state.win, GLFW_KEY_S) == GLFW_PRESS) state.cam_pos = vec3_sub(state.cam_pos, vec3_scale(state.cam_front, speed));
    if (glfwGetKey(state.win, GLFW_KEY_A) == GLFW_PRESS) state.cam_pos = vec3_sub(state.cam_pos, vec3_scale(vec3_normalize(vec3_cross(state.cam_front, state.cam_up)), speed));
    if (glfwGetKey(state.win, GLFW_KEY_D) == GLFW_PRESS) state.cam_pos = vec3_add(state.cam_pos, vec3_scale(vec3_normalize(vec3_cross(state.cam_front, state.cam_up)), speed));
}

void camera_mouse_callback(f64 xpos, f64 ypos)
{
    if (state.firstMouse) {
        state.lastX = (f32)xpos;
        state.lastY = (f32)ypos;
        state.firstMouse = false;
    }

    f32 xoffset = (f32)xpos - state.lastX;
    f32 yoffset = state.lastY - (f32)ypos;
    state.lastX = (f32)xpos;
    state.lastY = (f32)ypos;

    f32 sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    state.yaw   += xoffset;
    state.pitch += yoffset;

    if (state.pitch > 89.0f) state.pitch = 89.0f;
    if (state.pitch < -89.0f) state.pitch = -89.0f;

    update_camera_vectors();
}
