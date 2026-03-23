#ifndef CAM_H
#define CAM_H

#include "util/types.h"

typedef struct GLFWwindow GLFWwindow;

typedef struct {
    vec3s pos, front, up;
    f32 yaw, pitch, lastX, lastY;
    bool firstMouse;
} camera_t;

void update_camera_vectors(camera_t* cam);
void camera_mouse_callback(camera_t* cam, f64 xpos, f64 ypos);

#endif
