#include "cam.h"
#include "util/math.h"
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <math.h>

void update_camera_vectors(camera_t* cam)
{
    vec3s front;
    front.x = cosf(DEG2RAD(cam->yaw)) * cosf(DEG2RAD(cam->pitch));
    front.y = sinf(DEG2RAD(cam->pitch));
    front.z = sinf(DEG2RAD(cam->yaw)) * cosf(DEG2RAD(cam->pitch));
    cam->front = vec3_normalize(front);
}

void camera_update(camera_t* cam, GLFWwindow* window, const f32 dt)
{
    if (!cam || !window) return;
    const f32 speed = 2.5f * dt;
    const vec3s right = vec3_normalize(vec3_cross(cam->front, cam->up));
    
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) cam->pos = vec3_add(cam->pos, vec3_scale(cam->front, speed));
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) cam->pos = vec3_sub(cam->pos, vec3_scale(cam->front, speed));
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) cam->pos = vec3_sub(cam->pos, vec3_scale(right, speed));
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) cam->pos = vec3_add(cam->pos, vec3_scale(right, speed));
}

void camera_mouse_callback(camera_t* cam, const f64 xpos, const f64 ypos)
{
    if (!cam) return;
    if (cam->firstMouse) {
        cam->lastX = (f32)xpos;
        cam->lastY = (f32)ypos;
        cam->firstMouse = false;
    }

    const f32 xoffset = ((f32)xpos - cam->lastX) * 0.1f;
    const f32 yoffset = (cam->lastY - (f32)ypos) * 0.1f;
    cam->lastX = (f32)xpos;
    cam->lastY = (f32)ypos;

    cam->yaw   += xoffset;
    cam->pitch += yoffset;

    if (cam->pitch > 89.0f) cam->pitch = 89.0f;
    if (cam->pitch < -89.0f) cam->pitch = -89.0f;

    update_camera_vectors(cam);
}
