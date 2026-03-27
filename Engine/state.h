#ifndef STATE_H
#define STATE_H

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include "cam.h"
#include "prim.h"
#include "text.h"
#include "util/types.h"

typedef enum {
    STATE_MENU,
    STATE_PLAYING,
    STATE_EXIT
} state_e;

typedef struct {
    u32 vao, vbo, ebo, program;
} data_t;

typedef struct {
    i32 w, h;
} framebuffer_t;

typedef struct {
    bool cursor_locked;
    GLFWwindow* win;
    state_e id;
    f32 dt;
    framebuffer_t* fb;
    data_t* data;
    camera_t* cam;
    texture_registry_t* text;
    primitive_registry_t* prim;
    vertex_t text_vertices[MAX_TEXT_VERTICES];
    u32 text_vertex_count;
} state_t;

extern state_t state;

#endif
