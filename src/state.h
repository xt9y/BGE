#ifndef STATE_H
#define STATE_H

#include <GLFW/glfw3.h>
#include "text.h"
#include "util/types.h"

#define ASSERT(x) do { \
    if(!(x)) { printf("Assertion failed: %s\n", #x); exit(1); } \
} while(0)

typedef enum {
    STATE_MENU,
    STATE_PLAYING,
    STATE_EXIT
} state_e;

typedef struct {
    u32 vao, vbo, ebo, program;  // Added ebo
} data_t;

typedef struct {
    GLFWwindow* win;
    state_e id;
    f32 dt;
    data_t* data;
    texture_t* text;

    vec3s cam_pos;
    vec3s cam_front;
    vec3s cam_up;
    f32 yaw;
    f32 pitch;
    f32 lastX;
    f32 lastY;
    bool firstMouse;
} state_t;

state_t state;

#endif

