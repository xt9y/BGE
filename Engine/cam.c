#include "cam.h"
#include "util/math.h"
#include "opengl.h"
#include <math.h>

void update_camera_vectors(camera_t* cam)
{
    vec3s front;
    front.x = cosf(DEG2RAD(cam->yaw)) * cosf(DEG2RAD(cam->pitch));
    front.y = sinf(DEG2RAD(cam->pitch));
    front.z = sinf(DEG2RAD(cam->yaw)) * cosf(DEG2RAD(cam->pitch));
    cam->front = vec3_normalize(front);
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
