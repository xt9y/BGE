#ifndef CAM_H
#define CAM_H

#include "util/types.h"

typedef struct GLFWwindow GLFWwindow;

typedef struct {
    vec3s cam_pos, cam_front, cam_up;
    f32 yaw, pitch, lastX, lastY;
    bool firstMouse;
} camera_t;

void camera_init(camera_t* cam, f32 viewport_width, f32 viewport_height);
void camera_update(camera_t* cam, GLFWwindow* window, f32 dt);
void camera_mouse_callback(camera_t* cam, f64 xpos, f64 ypos);

#endif
